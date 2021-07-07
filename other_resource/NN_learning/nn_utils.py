import numpy as np
import tensorflow as tf
import tensorflow.compat.v1 as tf1
import random
from myplot import *
import math
import pynput
from pynput.keyboard import Listener
import datetime
from save import *
import copy

def purelin(x):
	return x

def sparse_cross_entropy(y, y_):
	return tf1.reduce_mean(tf1.nn.sparse_softmax_cross_entropy_with_logits(logits = tf.transpose(y), labels = tf.argmax(y_, 0)))

def cross_entropy(y, y_):
	return tf1.reduce_mean(tf1.nn.softmax_cross_entropy_with_logits(logits = tf.transpose(y), labels = tf.argmax(y_, 0)))

class Layer:
	def __init__(self, m_last, m, i, trainParam):
		self.activate = trainParam.valid_activation
		self.n_neurons = m

		if i == 0:
			self.W = tf1.eye(m, name = "W0")
			self.b = tf1.zeros(name = "b0", shape = [m, 1])
			self.W_use = tf1.eye(m, name = "W_use0")
			self.b_use = tf1.zeros(name = "b_use0", shape = [m, 1])
		else:
			self.W = tf1.get_variable(name = "W" + str(i), shape = [m, m_last], dtype = tf1.float32, initializer = trainParam.valid_weights_initializer)
			self.b = tf1.get_variable(name = "b" + str(i), shape = [m, 1], dtype = tf1.float32, initializer = trainParam.valid_weights_initializer)
			
			self.W_use = tf1.get_variable(name = "W_use" + str(i), shape = [m, m_last], dtype = tf1.float32, initializer = tf1.zeros_initializer(), trainable = False)
			self.b_use = tf1.get_variable(name = "b_use" + str(i), shape = [m, 1], dtype = tf1.float32, initializer = tf1.zeros_initializer(), trainable = False)
		
			# tf1.assign(self.W_use, self.W)
			# tf1.assign(self.b_use, self.b)
			self.W_use = self.W
			self.b_use = self.b

		self.out = tf1.get_variable(name = "out" + str(i), shape = [m, 1], dtype = tf1.float32, initializer = tf1.zeros_initializer(), trainable = False)

	def set_activation(self, func):
		self.activate = func

	def size(self):
		return self.n_neurons

	def weights(self):
		return self.W_use.eval()

	def bias(self):
		return self.b_use.eval()

	def set_weights(self, W):
		tf1.assign(self.W, W).eval()
		tf1.assign(self.W_use, W).eval()

	def set_bias(self, b):
		tf1.assign(self.b, b).eval()
		tf1.assign(self.b_use, b).eval()

class TrainParam:
	def __init__(self, goal = 1E-6, epochs = 2000, max_fail = 6, grad_min = 1E-7, \
		               batch_size = 100, test_ratio = 0.2, validation_ratio = 0.2, validation_time = 0.1, \
		               weights_initializer = "randn", activation = "tanh", loss_func = "mse", \
					   optimizer = "grad_des", grad_decay = 0.9, momentum = 0, epsilon = 1E-10, grad_decay2 = 0.999, \
		               learning_rate_func = "exp_decay", learning_rate = 0.8, learning_rate_decay = 0.999, discrete = False, end_learning_rate = 0.0001, learning_power = 2,\
		               map_input = True, map_output = True, input_map_range = [-1, 1], output_map_range = [-1, 1], \
		               regular_flag = False, regularizer = "l2", regular_weight = 0.01, \
	                   mv_avg_flag = False, mv_avg_decay = 0.99, \
	                   plot_flag = False, plot_type = 'normal', plot_compare = True, print_flag = True, interval = 20):
		self.n_samples = 0
		self.global_step = tf1.Variable(0, trainable = False)

		# 终止准则相关
		self.goal = goal
		self.epochs = epochs
		self.max_fail = max_fail
		self.grad_min = grad_min

		# 样本分割相关
		self.batch_size = batch_size
		self.test_ratio = test_ratio
		self.validation_ratio = validation_ratio
		self.validation_time = validation_time

		# 正向传播相关
		self.weights_initializer = weights_initializer
		self.activation = activation
		self.loss_func = loss_func

		self.valid_weights_initializer = None
		self.valid_activation = None
		self.valid_loss_func = None

		# 权值更新相关
		self.optimizer = optimizer
		self.grad_decay = grad_decay
		self.grad_decay2 = grad_decay2
		self.momentum = momentum
		self.epsilon = epsilon

		self.valid_optimizer = None

		# 学习率相关
		self.learning_rate_func = learning_rate_func
		self.learning_rate = learning_rate
		self.end_learning_rate = end_learning_rate
		self.learning_rate_decay = learning_rate_decay
		self.discrete = discrete
		self.learning_power = learning_power

		self.valid_learning_rate = None

		# 归一化相关
		self.map_input = map_input
		self.map_output = map_output
		self.input_map_range = input_map_range
		self.output_map_range = output_map_range

		# 正则化相关
		self.regular_flag = regular_flag
		self.regularizer = regularizer
		self.regular_weight = regular_weight

		self.valid_regularizer = None

		# 滑动平均相关
		self.mv_avg_flag = mv_avg_flag
		self.mv_avg_decay = mv_avg_decay

		self.valid_mv_avg = None

		# 显示状态相关
		self.plot_flag = plot_flag
		self.print_flag = print_flag
		self.interval = interval
		self.plot_compare = plot_compare
		self.plot_type = plot_type

	def apply(self):
		# 权值初始化
		if not isinstance(self.weights_initializer, str):
			self.valid_weights_initializer = self.weights_initializer
		else:
			if self.weights_initializer == "rand":
				self.valid_weights_initializer = tf1.random_uniform_initializer()
			elif self.weights_initializer == "randn":
				self.valid_weights_initializer = tf1.random_normal_initializer()
			elif self.weights_initializer == "trunc_randn":
				self.valid_weights_initializer = tf1.truncated_normal_initializer()
			else:
				print("Unsupported weights initializer:", self.weights_initializer)
				exit()

		# 激活函数
		if not isinstance(self.activation, str):
			self.valid_activation = self.activation
		else:
			activation_name = self.activation.lower()
			if activation_name == "purelin":
				self.valid_activation = purelin
			elif activation_name == "tanh":
				self.valid_activation = tf1.tanh
			elif activation_name == "sigmoid":
				self.valid_activation = tf1.sigmoid
			elif activation_name == "relu":
				self.valid_activation = tf1.nn.relu
			elif activation_name == "relu6":
				self.valid_activation = tf1.nn.relu6
			elif activation_name == "soft+":
				self.valid_activation = tf1.nn.softplus
			elif activation_name == "droupt":
				self.valid_activation = tf1.nn.droupt
			elif activation_name == "bias+":
				self.valid_activation = tf1.nn.bias_add
			else:
				print("Unsupported activation function:", self.activation)
				exit()

		# 损失函数
		if not isinstance(self.loss_func, str):
			self.valid_loss_func = self.loss_func
		else:
			loss_func_name = self.loss_func.lower()
			if loss_func_name == "cross_entropy":
				self.valid_loss_func = cross_entropy
			elif loss_func_name == "sparse_cross_entropy":
				self.valid_loss_func = sparse_cross_entropy
			elif loss_func_name == "mse":
				self.valid_loss_func = tf1.losses.mean_squared_error
			else:
				print("Unsupported loss function:", self.loss_func)
				exit()

		# 正则化
		if not isinstance(self.regularizer, str):
			self.valid_regularizer = self.regularizer
		else:
			regularizer_name = self.regularizer.lower()
			if regularizer_name == "l2":
				self.valid_regularizer = tf.contrib.layers.l2_regularizer(self.regular_weight)
			elif regularizer_name == "l1":
				self.valid_regularizer = tf.contrib.layers.l1_regularizer(self.regular_weight)
			else:
				print("Unsupported regularizer:", self.regularizer)
				exit()

		# 滑动平均
		self.valid_mv_avg = tf1.train.ExponentialMovingAverage(self.mv_avg_decay, self.global_step)

	def set_n_samples(self, n_samples):
		self.n_samples = n_samples
		self.batch_size = max(1, min(int(n_samples*(1-self.test_ratio)*(1-self.validation_ratio)), self.batch_size))
		# 学习率衰减函数
		if isinstance(self.learning_rate_func, float):
			self.valid_learning_rate = tf.constant(self.learning_rate_func)
		elif isinstance(self.learning_rate_func, str):
			learning_rate_func_name = self.learning_rate_func.lower()
			if learning_rate_func_name == "exp_decay":
				self.valid_learning_rate = tf1.train.exponential_decay(self.learning_rate, self.global_step, self.n_samples/self.batch_size, self.learning_rate_decay, self.discrete)
			elif learning_rate_func_name == "inv_time":
				self.valid_learning_rate = tf1.train.inverse_time_decay(self.learning_rate, self.global_step, self.n_samples/self.batch_size, self.learning_rate_decay, self.discrete)
			elif learning_rate_func_name == "natural_exp":
				self.valid_learning_rate = tf1.train.natural_exp_decay(self.learning_rate, self.global_step, self.n_samples/self.batch_size, self.learning_rate_decay, self.discrete)
			elif learning_rate_func_name == "poly":
				self.valid_learning_rate = tf1.train.polynomial_decay(self.learning_rate, self.global_step, self.n_samples/self.batch_size, self.end_learning_rate, self.learning_power, self.discrete)
			else:
				print("Unsupported learning rate decay function:", learning_rate_func_name)
				exit()

		# 权值更新器
		if not isinstance(self.optimizer, str):
			self.valid_optimizer = self.optimizer
		else:
			optimizer_name = self.optimizer.lower()
			if optimizer_name == "grad_des":
				self.valid_optimizer = tf1.train.GradientDescentOptimizer(self.valid_learning_rate)
			elif optimizer_name == "adagrad":
				self.valid_optimizer = tf1.train.AdagradOptimizer(learning_rate = self.valid_learning_rate, initial_accumulator_value = 0.1)
			elif optimizer_name == "rmsprop":
				self.valid_optimizer = tf1.train.RMSPropOptimizer(self.valid_learning_rate, self.grad_decay, self.momentum, self.epsilon)
			elif optimizer_name == "adam":
				self.valid_optimizer = tf1.train.AdamOptimizer(self.valid_learning_rate, self.grad_decay, self.grad_decay2, self.epsilon)
			else:
				print("Unsupported optimizer:", self.optimizer)
				exit()

	def save(self, name):
		mkdir(name)

		# 终止准则相关
		save(name + '/goal.pickle', self.goal)
		save(name + '/epochs.pickle', self.epochs)
		save(name + '/max_fail.pickle', self.max_fail)
		save(name + '/grad_min.pickle', self.grad_min)

		# 样本分割相关
		save(name + '/batch_size.pickle', self.batch_size)
		save(name + '/test_ratio.pickle', self.test_ratio)
		save(name + '/validation_ratio.pickle', self.validation_ratio)
		save(name + '/validation_time.pickle', self.validation_time)

		# 正向传播相关
		save(name + '/weights_initializer.pickle', self.weights_initializer)
		save(name + '/activation.pickle', self.activation)
		save(name + '/loss_func.pickle', self.loss_func)

		# 权值更新相关
		save(name + '/optimizer.pickle', self.optimizer)
		save(name + '/grad_decay.pickle', self.grad_decay)
		save(name + '/grad_decay2.pickle', self.grad_decay2)
		save(name + '/momentum.pickle', self.momentum)
		save(name + '/epsilon.pickle', self.epsilon)

		# 学习率相关
		save(name + '/learning_rate_func.pickle', self.learning_rate_func)
		save(name + '/learning_rate.pickle', self.learning_rate)
		save(name + '/end_learning_rate.pickle', self.end_learning_rate)
		save(name + '/learning_rate_decay.pickle', self.learning_rate_decay)
		save(name + '/discrete.pickle', self.discrete)
		save(name + '/learning_power.pickle', self.learning_power)

		# 归一化相关
		save(name + '/map_input.pickle', self.map_input)
		save(name + '/map_output.pickle', self.map_output)
		save(name + '/input_map_range.pickle', self.input_map_range)
		save(name + '/output_map_range.pickle', self.output_map_range)

		# 正则化相关
		save(name + '/regular_flag.pickle', self.regular_flag)
		save(name + '/regularizer.pickle', self.regularizer)
		save(name + '/regular_weight.pickle', self.regular_weight)

		# 滑动平均相关
		save(name + '/mv_avg_flag.pickle', self.mv_avg_flag)
		save(name + '/mv_avg_decay.pickle', self.mv_avg_decay)

		# 显示状态相关
		save(name + '/plot_flag.pickle', self.plot_flag)
		save(name + '/print_flag.pickle', self.print_flag)
		save(name + '/interval.pickle', self.interval)
		save(name + '/plot_compare.pickle', self.plot_compare)
		save(name + '/plot_type.pickle', self.plot_type)

	def load(self, name):
		# 终止准则相关
		self.goal = load(name + '/goal.pickle')
		self.epochs = load(name + '/epochs.pickle')
		self.max_fail = load(name + '/max_fail.pickle')
		self.grad_min = load(name + '/grad_min.pickle')

		# 样本分割相关
		self.batch_size = load(name + '/batch_size.pickle')
		self.test_ratio = load(name + '/test_ratio.pickle')
		self.validation_ratio = load(name + '/validation_ratio.pickle')
		self.validation_time = load(name + '/validation_time.pickle')

		# 正向传播相关
		self.weights_initializer = load(name + '/weights_initializer.pickle')
		self.activation = load(name + '/activation.pickle')
		self.loss_func = load(name + '/loss_func.pickle')

		# 权值更新相关
		self.optimizer = load(name + '/optimizer.pickle')
		self.grad_decay = load(name + '/grad_decay.pickle')
		self.grad_decay2 = load(name + '/grad_decay2.pickle')
		self.momentum = load(name + '/momentum.pickle')
		self.epsilon = load(name + '/epsilon.pickle')

		# 学习率相关
		self.learning_rate_func = load(name + '/learning_rate_func.pickle')
		self.learning_rate = load(name + '/learning_rate.pickle')
		self.end_learning_rate = load(name + '/end_learning_rate.pickle')
		self.learning_rate_decay = load(name + '/learning_rate_decay.pickle')
		self.discrete = load(name + '/discrete.pickle')
		self.learning_power = load(name + '/learning_power.pickle')

		# 归一化相关
		self.map_input = load(name + '/map_input.pickle')
		self.map_output = load(name + '/map_output.pickle')
		self.input_map_range = load(name + '/input_map_range.pickle')
		self.output_map_range = load(name + '/output_map_range.pickle')

		# 正则化相关
		self.regular_flag = load(name + '/regular_flag.pickle')
		self.regularizer = load(name + '/regularizer.pickle')
		self.regular_weight = load(name + '/regular_weight.pickle')

		# 滑动平均相关
		self.mv_avg_flag = load(name + '/mv_avg_flag.pickle')
		self.mv_avg_decay = load(name + '/mv_avg_decay.pickle')

		# 显示状态相关
		self.plot_flag = load(name + '/plot_flag.pickle')
		self.print_flag = load(name + '/print_flag.pickle')
		self.interval = load(name + '/interval.pickle')
		self.plot_compare = load(name + '/plot_compare.pickle')
		self.plot_type = load(name + '/plot_type.pickle')

class mapminmax:
	def __init__(self, data = None, map_range = [-1, 1]):
		self.map_range = map_range
		if data is None:
			self.min_vec = None
			self.max_vec = None
			self.delta = None
		else:
			self.min_vec = np.expand_dims(data.min(1), 1)
			self.max_vec = np.expand_dims(data.max(1), 1)
			self.delta = self.max_vec - self.min_vec

	def __call__(self, data, map_range = None):
		if not map_range is None:
			self.map_range = map_range

		self.min_vec = np.expand_dims(data.min(1), 1)
		self.max_vec = np.expand_dims(data.max(1), 1)
		self.delta = self.max_vec - self.min_vec

		return (data-self.min_vec)/self.delta*(self.map_range[1]-self.map_range[0]) + self.map_range[0]

	def apply(self, data):
		if self.min_vec is None:
			return data
		else:
			return (data-self.min_vec)/self.delta*(self.map_range[1]-self.map_range[0]) + self.map_range[0]

	def reverse(self, data):
		if self.min_vec is None:
			return data
		else:
			return (data-self.map_range[0])/(self.map_range[1]-self.map_range[0])*self.delta+self.min_vec


def peaks_train_data():
	x, y, z = peaks()
	P = np.vstack((x.flat, y.flat))
	T = z.flat

	return P, T

def sin_train_data():
	x = np.linspace(-np.pi, np.pi)
	y = np.sin(x)
	return x, y

class SurfFormatter:
	def __init__(self, x = None):
		if x is None:
			self.n_cols = None
			self.n_rows = None
			return

		for i in range(1, x.shape[1]):
			if x[0, i] < x[0, i-1]:
				self.n_cols = i
				self.n_rows = x.shape[1]/i
				break

	def __call__(self, x):
		if x.ndim == 1:
			x = np.expand_dims(x, 0)

		for i in range(1, x.shape[1]):
			if x[0, i] < x[0, i-1]:
				self.n_cols = i
				self.n_rows = int(x.shape[1]/i)
				break
		return np.reshape(x, (self.n_rows, self.n_cols))

	def apply1(self, x):
		if self.n_rows is None:
			return x

		if x.ndim == 1:
			x = np.expand_dims(x, 0)

		if x.shape[0] == 1:
			return np.reshape(x, (self.n_rows, self.n_cols))

		result = []
		for i in range(x.shape[0]):
			result.append(np.reshape(x[i, :], (self.n_rows, self.n_cols)))

		return result

	def apply(self, x):
		if self.n_rows is None:
			return x

		if x.ndim == 1:
			x = np.expand_dims(x, 0)

		# if x.shape[0] == 1:
		# 	return np.reshape(x, self.n_rows, self.n_cols)

		result = []
		for i in range(x.shape[0]):
			result.append(np.reshape(x[i, :], (self.n_rows, self.n_cols)))

		return result

class TrainData:
	def __init__(self):
		self.P = None
		self.T = None
		self.P_init = None
		self.T_init = None
		self.train_set = None
		self.validation_set = None
		self.input_mapper = mapminmax()
		self.output_mapper = mapminmax()

class Operations:
	def __init__(self):
		self.train_op = None
		self.average_op = None
		self.one_step_op = None

class Plot:
	def __init__(self):
		# plotting figure
		self.fig = []
		self.ax_curve = []
		self.ax_surf = []
		self.ax_error = None
		self.ax_learning = None

		# plotting data
		self.iteration = []
		self.error = []
		self.learning_rate = []
		self.x_curve = None
		self.y_real = []
		self.y_target = []
		self.x_surf = None
		self.y_surf = None
		self.z_target = []
		self.z_real = []

		# plotting objects
		self.error_curve = None
		self.learning_rate_curve = None
		self.target_curve = []
		self.real_curve = []
		self.target_surf = []
		self.real_surf = []

		# assistant:
		self.formatter = SurfFormatter()

def now():
	return datetime.datetime.strftime(datetime.datetime.now(),'%Y%m%d%H%M%S')