from nn_utils import *

class BPNN:
	def __init__(self, layers_shape = None, trainParam = None):
		if layers_shape is None:
			self._empty_init()
		elif isinstance(layers_shape, str):
			self._empty_init()
			self.load(layers_shape)
		else:
			self.structure = layers_shape
			self.trainData = TrainData()
			self.operations = Operations()
			self.plot = Plot()
			self.listener = Listener(on_press = self._detect_ctrl_c)
			self.continue_train = True
			
			if trainParam is None or not isinstance(trainParam, TrainParam):
				self.trainParam = TrainParam()
			else:
				self.trainParam = trainParam

			self._construct()

	def _empty_init(self):
		self.structure = None
		self.trainData = TrainData()
		self.operations = Operations()
		self.plot = Plot()
		self.continue_train = True
		
		self.trainParam = TrainParam()

		self._n_layers = 0
		self._n_inputs = 0
		self._n_outputs = 0

		self.input = None
		self.layers = None
		self.output = None
		self.expected_output = None

		self.session = None

	def _construct(self):
		self.trainParam.apply()

		self._n_layers = len(self.structure) - 1
		self._n_inputs = self.structure[0]
		self._n_outputs = self.structure[-1]

		self.input = tf1.placeholder(shape=[self.structure[0], None], dtype=tf1.float32, name="input")

		self.layers = [Layer(0, self.structure[0], 0, self.trainParam)]
		for i in range(1, len(self.structure)):
			self.layers.append(Layer(self.structure[i-1], self.structure[i], i, self.trainParam))
		self.layers[-1].set_activation(purelin);

		self.output = tf1.get_variable(dtype=tf1.float32, shape=[self.structure[-1], 1], name="output", initializer=tf1.zeros_initializer(), trainable = False)
		self.expected_output = tf1.placeholder(dtype=tf1.float32, shape=[self.structure[-1], None], name="expected_output")

		self._apply_FP()
		self.session = tf1.InteractiveSession()

	def __del__(self):
		self.listener.join()
		self.session.close()

	def _apply_FP(self):
		if self.trainParam.mv_avg_flag:
			self.operations.average_op = self.trainParam.valid_mv_avg.apply(tf1.trainable_variables())
			for i in range(1, self._n_layers+1):
				self.layers[i].W_use = self.trainParam.valid_mv_avg.average(self.layers[i].W)
				self.layers[i].b_use = self.trainParam.valid_mv_avg.average(self.layers[i].b)
		else:
			for i in range(1, self._n_layers+1):
				self.layers[i].W_use = self.layers[i].W
				self.layers[i].b_use = self.layers[i].b

		self.layers[0].out = self.input
		for i in range(1, self._n_layers+1):
			self.layers[i].out = self.layers[i].activate(tf1.matmul(self.layers[i].W_use, self.layers[i-1].out) + self.layers[i].b_use);
		self.output = self.layers[-1].out

	def _apply_BP(self):
		self.error = self.trainParam.valid_loss_func(self.output, self.expected_output)
		self.loss = self.error
		if self.trainParam.regular_flag:
			regularization = self.trainParam.valid_regularizer(self.layers[1].W)
			for i in range(2, self._n_layers+1):
				regularization += self.trainParam.valid_regularizer(self.layers[i].W)
			self.loss += regularization

		self.operations.train_op = self.trainParam.valid_optimizer.minimize(self.loss, global_step = self.trainParam.global_step)
		
		if self.trainParam.mv_avg_flag:
			self.operations.one_step_op = tf1.group(self.operations.train_op, self.operations.average_op)
		else:
			self.operations.one_step_op = self.operations.train_op

	def train(self, P, T):
		self._format_samples(P, T)
		if self.trainParam.map_input:
			self.trainData.P = self.trainData.input_mapper(self.trainData.P_init, self.trainParam.input_map_range)
		else:
			self.trainData.P = self.trainData.P_init

		if self.trainParam.map_output:
			self.trainData.T = self.trainData.output_mapper(self.trainData.T_init, self.trainParam.output_map_range)
		else:
			self.trainData.T = self.trainData.T_init

		self.trainParam.set_n_samples(self.trainData.P.shape[1])
		self.trainData.train_set = list(range(self.trainData.P.shape[1]))
		self.trainData.train_set, self.trainData.test_set = self.split_sample(self.trainData.train_set, self.trainParam.test_ratio)
		self.trainData.train_set, self.trainData.validation_set = self.split_sample(self.trainData.train_set, self.trainParam.validation_ratio)

		self._apply_BP()
		self.session.run(tf1.global_variables_initializer())

		self.continue_train = True
		self.listener.start()

		for epoch in range(self.trainParam.epochs):
			if not self.continue_train:
				break

			batch_P, batch_T = self.get_batch()
			self.session.run(self.operations.one_step_op, feed_dict={self.input : batch_P, self.expected_output : batch_T})

			self.plot.iteration.append(epoch)
			error = self.error.eval(feed_dict={self.input : batch_P, self.expected_output : batch_T})
			self.plot.error.append(error)
			self.plot.learning_rate.append(self.trainParam.valid_learning_rate.eval())

			if self.trainParam.plot_flag:
				if epoch == 0:
					if self.trainParam.print_flag:
						self._print_state()
					if self._n_inputs == 1 and self.trainParam.plot_compare and self.trainParam.plot_type != 'classify':
						self._plot2D_init()
					elif self._n_inputs == 2 and self.trainParam.plot_compare and self.trainParam.plot_type != 'classify':
						self._plot3D_init()
					elif self._n_inputs == 2 and self.trainParam.plot_compare and self.trainParam.plot_type == 'classify':
						self._plot_2D_classify_init()
					else:
						self._plot_error_init()
				elif epoch/self.trainParam.interval == int(epoch/self.trainParam.interval):
					if self.trainParam.print_flag:
						self._print_state()
					if self._n_inputs == 1 and self.trainParam.plot_compare and self.trainParam.plot_type != 'classify':
						self._plot2D()
					elif self._n_inputs == 2 and self.trainParam.plot_compare and self.trainParam.plot_type != 'classify':
						self._plot3D()
					elif self._n_inputs == 2 and self.trainParam.plot_compare and self.trainParam.plot_type == 'classify':
						self._plot_2D_classify()
					else:
						self._plot_error()

		if self.trainParam.plot_flag:
			plt.show()

	def _print_state(self):
		step = self.trainParam.global_step.eval()
		epochs = self.trainParam.epochs
		print(str(step) + '/' + str(epochs) + ' = ' + str(round(step/epochs*100, 2)) + '%, MSE = ' + str(self.plot.error[-1]) + ', learning_rate = ' + str(self.plot.learning_rate[-1]))

	def _plot3D_init(self):
		self.plot.fig = plt.figure(figsize=(13, 5))
		self.plot.x_surf = self.trainData.P_init[0, :]
		self.plot.y_surf = self.trainData.P_init[1, :]
		self.plot.z_target = self.trainData.T_init
		self.plot.z_real = self.__call__(self.trainData.P_init)

		self.plot.x_surf = self.plot.formatter(self.plot.x_surf)
		self.plot.y_surf = self.plot.formatter.apply1(self.plot.y_surf)
		self.plot.z_target = self.plot.formatter.apply(self.plot.z_target)
		self.plot.z_real = self.plot.formatter.apply(self.plot.z_real)
		
		for i in range(self.n_outputs()):
			self.plot.ax_surf.append(self.plot.fig.add_subplot(self.n_outputs(), 2, 2*i+1, projection='3d'))
			target_surf = self.plot.ax_surf[i].plot_surface(self.plot.x_surf, self.plot.y_surf, self.plot.z_target[i], label = 'expected output' + str(i+1))
			real_surf   = self.plot.ax_surf[i].plot_surface(self.plot.x_surf, self.plot.y_surf, self.plot.z_real[i], label = 'current output' + str(i+1), color = [1.00, 0.58, 0.21])
			self.plot.target_surf.append(target_surf)
			self.plot.real_surf.append(real_surf)

			zmin = self.plot.z_target[i].min()
			zmax = self.plot.z_target[i].max()
			Dz = zmax - zmin
			self.plot.ax_surf[i].set_zlim3d(zmin-Dz/10, zmax+Dz/10)
			self.plot.ax_surf[i].set_xlabel('net input 1')
			self.plot.ax_surf[i].set_ylabel('net input 2')
			self.plot.ax_surf[i].set_zlabel('net output ' + str(i+1))

		self.plot.ax_error = self.plot.fig.add_subplot(2, 2, 2)
		self.plot.error_curve, = self.plot.ax_error.plot(self.plot.iteration, self.plot.error)
		self.plot.ax_error.set_xlim(0, self.trainParam.epochs-1)
		self.plot.ax_error.set_ylim(0, self.plot.error[0])
		self.plot.ax_error.set_xlabel('iteration')
		self.plot.ax_error.set_ylabel('Error(MSE)')

		self.plot.ax_learning = self.plot.fig.add_subplot(2, 2, 4)
		self.plot.learning_rate_curve, = self.plot.ax_learning.plot(self.plot.iteration, self.plot.learning_rate)
		self.plot.ax_learning.set_xlim(0, self.trainParam.epochs-1)
		self.plot.ax_learning.set_ylim(0, 1.1*self.trainParam.learning_rate)
		self.plot.ax_learning.set_xlabel('iteration')
		self.plot.ax_learning.set_ylabel('Learning Rate')

		plt.tight_layout()

	def _plot3D(self):
		self.plot.z_real = self.plot.formatter.apply(self.__call__(self.trainData.P_init))
		
		for i in range(self._n_outputs):
			self.plot.real_surf[i].remove()
			self.plot.real_surf[i] = self.plot.ax_surf[i].plot_surface(self.plot.x_surf, self.plot.y_surf, self.plot.z_real[i], label = 'current output' + str(i+1), color=[1.00, 0.58, 0.21])

		self.plot.error_curve.set_data(self.plot.iteration, self.plot.error)
		self.plot.learning_rate_curve.set_data(self.plot.iteration, self.plot.learning_rate)

		if self.plot.error[-1] != 0 and self.plot.error[-1] < self.plot.ax_error.get_ylim()[1]/50:
			self.plot.ax_error.set_ylim(0, self.plot.ax_error.get_ylim()[1]/2)

		plt.pause(0.001)
		# plt.plot()

	def _plot2D_init(self):
		self.plot.fig = plt.figure(figsize=(13, 5))
		self.plot.x_curve  = self.trainData.P_init.flatten()
		self.plot.y_target = self.trainData.T_init
		self.plot.y_real = self.__call__(self.trainData.P_init)

		for i in range(self._n_outputs):
			self.plot.ax_curve.append(self.plot.fig.add_subplot(self.n_outputs(), 2, 2*i+1))
			target_curve, = self.plot.ax_curve[i].plot(self.plot.x_curve, self.plot.y_target[i, :], label = 'expected output' + str(i+1))
			real_curve,   = self.plot.ax_curve[i].plot(self.plot.x_curve, self.plot.y_real[i, :],  label = 'current output' + str(i+1))
			self.plot.target_curve.append(target_curve)
			self.plot.real_curve.append(real_curve)

			ymin = self.plot.y_target[i, :].min()
			ymax = self.plot.y_target[i, :].max()
			Dy = ymax - ymin
			self.plot.ax_curve[i].set_ylim(ymin-Dy/10, ymax+Dy/10)
			self.plot.ax_curve[i].set_xlabel('net input')
			self.plot.ax_curve[i].set_ylabel('net output')
			self.plot.ax_curve[i].legend(loc='upper left')

		self.plot.ax_error = self.plot.fig.add_subplot(2, 2, 2)
		self.plot.error_curve, = self.plot.ax_error.plot(self.plot.iteration, self.plot.error)
		self.plot.ax_error.set_xlim(0, self.trainParam.epochs-1)
		self.plot.ax_error.set_ylim(0, self.plot.error[0])
		self.plot.ax_error.set_xlabel('iteration')
		self.plot.ax_error.set_ylabel('Error(MSE)')

		self.plot.ax_learning = self.plot.fig.add_subplot(2, 2, 4)
		self.plot.learning_rate_curve, = self.plot.ax_learning.plot(self.plot.iteration, self.plot.learning_rate)
		self.plot.ax_learning.set_xlim(0, self.trainParam.epochs-1)
		self.plot.ax_learning.set_ylim(0, 1.1*self.trainParam.learning_rate)
		self.plot.ax_learning.set_xlabel('iteration')
		self.plot.ax_learning.set_ylabel('Learning Rate')

		plt.tight_layout()

	def _plot2D(self):
		self.plot.y_real = self.__call__(self.trainData.P_init)
	
		for i in range(self.n_outputs()):
			self.plot.real_curve[i].set_data(self.plot.x_curve, self.plot.y_real[i, :])

		self.plot.error_curve.set_data(self.plot.iteration, self.plot.error)
		self.plot.learning_rate_curve.set_data(self.plot.iteration, self.plot.learning_rate)

		if self.plot.error[-1] != 0 and self.plot.error[-1] < self.plot.ax_error.get_ylim()[1]/50:
			self.plot.ax_error.set_ylim(0, self.plot.ax_error.get_ylim()[1]/2)

		plt.pause(0.001)
		# plt.plot()

	def _plot_error_init(self):
		self.plot.fig = plt.figure(figsize=(7, 6))
		
		self.plot.ax_error = self.plot.fig.add_subplot(2, 1, 1)
		self.plot.error_curve, = self.plot.ax_error.plot(self.plot.iteration, self.plot.error)
		self.plot.ax_error.set_xlim(0, self.trainParam.epochs-1)
		self.plot.ax_error.set_ylim(0, self.plot.error[0])
		self.plot.ax_error.set_xlabel('iteration')
		self.plot.ax_error.set_ylabel('Error')

		self.plot.ax_learning = self.plot.fig.add_subplot(2, 1, 2)
		self.plot.learning_rate_curve, = self.plot.ax_learning.plot(self.plot.iteration, self.plot.learning_rate)
		self.plot.ax_learning.set_xlim(0, self.trainParam.epochs-1)
		self.plot.ax_learning.set_ylim(0, 1.1*self.trainParam.learning_rate)
		self.plot.ax_learning.set_xlabel('iteration')
		self.plot.ax_learning.set_ylabel('Learning Rate')

		plt.tight_layout()

	def _plot_error(self):
		self.plot.error_curve.set_data(self.plot.iteration, self.plot.error)
		self.plot.learning_rate_curve.set_data(self.plot.iteration, self.plot.learning_rate)

		if self.plot.error[-1] != 0 and self.plot.error[-1] < self.plot.ax_error.get_ylim()[1]/50:
			self.plot.ax_error.set_ylim(0, self.plot.ax_error.get_ylim()[1]/2)

		plt.pause(0.001)
		# plt.plot()

	def _plot_2D_classify_init(self):
		self.plot.fig = plt.figure(figsize=(13, 5))
		self.plot.x_points = self.trainData.P_init[0, :]
		self.plot.y_points = self.trainData.P_init[1, :]
		self.plot.it_groups = []
		it_rows, it_cols = np.where(self.trainData.T_init == 1)
		self.plot.ax_curve = self.plot.fig.add_subplot(1, 2, 1)

		for i in range(self._n_outputs):
			self.plot.it_groups.append(it_cols[np.where(it_rows == i)])
			self.plot.ax_curve.plot(self.plot.x_points[self.plot.it_groups[-1]], self.plot.y_points[self.plot.it_groups[-1]], '.')

		self.plot.x_surf = np.linspace(self.trainData.P_init[0, :].min(), self.trainData.P_init[0, :].max())
		self.plot.y_surf = np.linspace(self.trainData.P_init[1, :].min(), self.trainData.P_init[1, :].max())
		self.plot.x_surf, self.plot.y_surf = np.meshgrid(self.plot.x_surf, self.plot.y_surf)
		self.plot.P_surf = np.vstack((self.plot.x_surf.flatten(), self.plot.y_surf.flatten()))
		T_surf = self.__call__(self.plot.P_surf)
		self.plot.z_surf = np.reshape(np.argmax(T_surf, axis=0), self.plot.x_surf.shape)
		self.plot.contour_levels = np.array(range(self._n_outputs + 1)) - 0.5
		self.plot.contour = self.plot.ax_curve.contour(self.plot.x_surf, self.plot.y_surf, self.plot.z_surf, levels = self.plot.contour_levels, colors='k', linewidths=1)
		self.plot.contourf = self.plot.ax_curve.contourf(self.plot.x_surf, self.plot.y_surf, self.plot.z_surf, levels = self.plot.contour_levels, alpha=0.4, cmap=plt.cm.terrain)

		self.plot.ax_error = self.plot.fig.add_subplot(2, 2, 2)
		self.plot.error_curve, = self.plot.ax_error.plot(self.plot.iteration, self.plot.error)
		self.plot.ax_error.set_xlim(0, self.trainParam.epochs-1)
		self.plot.ax_error.set_ylim(0, self.plot.error[0])
		self.plot.ax_error.set_xlabel('iteration')
		self.plot.ax_error.set_ylabel('Error')

		self.plot.ax_learning = self.plot.fig.add_subplot(2, 2, 4)
		self.plot.learning_rate_curve, = self.plot.ax_learning.plot(self.plot.iteration, self.plot.learning_rate)
		self.plot.ax_learning.set_xlim(0, self.trainParam.epochs-1)
		self.plot.ax_learning.set_ylim(0, 1.1*self.trainParam.learning_rate)
		self.plot.ax_learning.set_xlabel('iteration')
		self.plot.ax_learning.set_ylabel('Learning Rate')

		plt.tight_layout()

	def max2(self, A_):
		A = copy.copy(A_)
		it_max_row = np.argmax(A, axis = 0)
		Amin = np.min(A, axis = 0)
		for j in range(A.shape[1]):
			A[it_max_row[j], j] = Amin[j]-1

		Amax2 = np.max(A, axis = 0)
		it_max2 = np.argmax(A, axis = 0)
		return Amax2, it_max2

	def _plot_2D_classify(self):
		T_surf = self.__call__(self.plot.P_surf)
		self.plot.z_surf = np.reshape(np.argmax(T_surf, axis=0), self.plot.x_surf.shape)
		for coll in self.plot.contour.collections:
			coll.remove()
		for coll in self.plot.contourf.collections:
			coll.remove()
		self.plot.contour = self.plot.ax_curve.contour(self.plot.x_surf, self.plot.y_surf, self.plot.z_surf, levels = self.plot.contour_levels, colors='k', linewidths=1)
		self.plot.contourf = self.plot.ax_curve.contourf(self.plot.x_surf, self.plot.y_surf, self.plot.z_surf, levels = self.plot.contour_levels, alpha=0.4, cmap=plt.cm.terrain)

		self.plot.error_curve.set_data(self.plot.iteration, self.plot.error)
		self.plot.learning_rate_curve.set_data(self.plot.iteration, self.plot.learning_rate)

		if self.plot.error[-1] != 0 and self.plot.error[-1] < self.plot.ax_error.get_ylim()[1]/50:
			self.plot.ax_error.set_ylim(0, self.plot.ax_error.get_ylim()[1]/2)

		plt.pause(0.001)

	def _format_samples(self, P, T):
		self.trainData.P_init = np.array(P, dtype=np.float64)
		self.trainData.T_init = np.array(T, dtype=np.float64)

		if self.trainData.P_init.ndim == 1:
			self.trainData.P_init = np.expand_dims(self.trainData.P_init, 1)

		if self.trainData.T_init.ndim == 1:
			self.trainData.T_init = np.expand_dims(self.trainData.T_init, 1)

		if self.trainData.P_init.shape[0] != self.n_inputs() and self.trainData.P_init.shape[1] == self.n_inputs():
			self.trainData.P_init = self.trainData.P_init.T

		if self.trainData.T_init.shape[0] != self.n_outputs() and self.trainData.T_init.shape[1] == self.n_outputs():
			self.trainData.T_init = self.trainData.T_init.T

		if self.trainData.P_init.shape[0] != self.n_inputs():
			print("Error using BPNN.train(self, P, T):")
			print("P.shape[0] must have the same size with net inputs!")
			exit()

		if self.trainData.T_init.shape[0] != self.n_outputs():
			print("Error using BPNN.train(self, P, T):")
			print("T.shape[0] must have the same size with net outputs!")
			exit()

		if self.trainData.P_init.shape[1] != self.trainData.T_init.shape[1]:
			print("Error using BPNN.train(self, P, T):")
			print("P.shape[1] must be same as T.shape[1]!")
			exit()

	def split_sample(self, samples, ratio):
		test_set = random.sample(samples, int(ratio*len(samples)))
		train_set = list(set(samples).difference(set(test_set)))

		return train_set, test_set

	def get_batch(self):
		batch_it = random.sample(self.trainData.train_set, self.trainParam.batch_size)
		return self.trainData.P[:, batch_it], self.trainData.T[:, batch_it]

	def _raw_call(self, x):
		return self.session.run(self.output, feed_dict={self.input : x})

	def _detect_ctrl_c(self, key):
		if isinstance(key, pynput.keyboard._win32.KeyCode) and key.char == '\x03':
			self.continue_train = False
			return False

	def __call__(self, x):
		input_is_list = False
		input_is_one_dim = False
		input_need_transpose = False

		if not isinstance(x, np.ndarray):
			input_is_list = True
			x = np.array(x)

		if x.ndim == 1:
			input_is_one_dim = True
			x = np.expand_dims(x, 1)

		if x.shape[0] != self.n_inputs():
			input_need_transpose = True
			x = x.T

		x = self.trainData.input_mapper.apply(x)
		y = self.trainData.output_mapper.reverse(self._raw_call(x))

		if input_need_transpose:
			y = y.T

		if input_is_one_dim:
			y = y.flatten()

		if input_is_list:
			y = y.tolist()

		return y

	def n_inputs(self):
		return self._n_inputs

	def n_outputs(self):
		return self._n_outputs

	def set_activation(self, func):
		for i in range(1, len(self.layers)):
			self.layers[i].set_activation(func)

	def show(self, r = 0.4):
		fig = plt.figure()
		ax = fig.add_subplot(111)

		n_neurons_max = self.layers[0].size()
		i_max = 0
		for i in range(1, self._n_layers + 1):
			if self.layers[i].size() > n_neurons_max:
				n_neurons_max = self.layers[i].size()
				i_max = i

		length_max = max(5, n_neurons_max - 1)
		dx = max(2, length_max / self._n_layers*1.3)
		dy = 0
		x = 0
		y = 0
		color = [0.50, 0.50, 1.00]
		X = []
		Y = []
		y_group = []
		for i in range(self._n_layers + 1):
			X.append(x)
			if i == 0:
				color = [0.50, 1.00, 0.50]
			elif i == self._n_layers:
				color = [1.00, 0.50, 0.50]
			else:
				color = [0.50, 0.50, 1.00]

			if self.layers[i].size() != n_neurons_max:
				dy = length_max / self.layers[i].size()
				y = dy/2
			else:
				dy = length_max / (self.layers[i].size() - 1)
				y = 0

			for j in range(self.layers[i].size()):
				y_group.append(y)
				ax.add_patch(Circle(xy = (x, y), radius = r, color=color))
				y += dy

			x += dx
			Y.append(y_group)
			y_group = []

		
		for i in range(self._n_layers):
			for j1 in range(self.layers[i].size()):
				for j2 in range(self.layers[i+1].size()):
					self._link((X[i], Y[i][j1]), (X[i+1], Y[i+1][j2]), r)

		plt.axis('equal')
		plt.axis('off')
		plt.tight_layout()
		plt.show()

	def _link(self, point1, point2, r):
		x1 = point1[0]
		y1 = point1[1]
		x2 = point2[0]
		y2 = point2[1]
		d = math.sqrt(math.pow(x2-x1, 2) + math.pow(y2-y1, 2))
		x_rest = r*(x2-x1)/d
		y_rest = r*(y2-y1)/d
		x = x1 + x_rest
		y = y1 + y_rest
		dx = x2-x1-2*x_rest
		dy = y2-y1-2*y_rest
		gray = [0.50, 0.50, 0.50]
		plt.arrow(x, y, dx, dy, antialiased = True, head_width=0.08, head_length=0.12, fc = gray, ec = gray, length_includes_head = True)

	def save(self, name = None):
		if name is None:
			name = 'data/autosave/net_' + now()

		mkdir('data')
		last_net_file = open('data/last_net.txt', 'w')
		last_net_file.write(name)
		last_net_file.close()

		mkdir(name)
		save(name + '/structure.pickle', self.structure)
		self.trainParam.save(name + '/trainParam')
		save(name + '/trainData.pickle', self.trainData)
		for i in range(1, self._n_layers+1):
			layer_folder_name = name + '/layer' + str(i)
			mkdir(layer_folder_name)
			save(layer_folder_name + '/W.pickle', self.layers[i].weights())
			save(layer_folder_name + '/b.pickle', self.layers[i].bias())

	def load(self, name = None):
		if name is None:
			last_net_file = open('data/last_net.txt', 'r')
			name = last_net_file.read()
			last_net_file.close()

		self.clear()
		self.structure = load(name + '/structure.pickle')
		self.trainParam.load(name + '/trainParam')
		self.trainData = load(name + '/trainData.pickle')
		self._construct()

		for i in range(1, self._n_layers+1):
			layer_folder_name = name + '/layer' + str(i)
			self.layers[i].set_weights(load(layer_folder_name + '/W.pickle'))
			self.layers[i].set_bias(load(layer_folder_name + '/b.pickle'))

	def clear(self):
		tf1.reset_default_graph()
		if not self.session is None:
			self.session.close()

		self.structure = None
		self.trainData = TrainData()
		self.operations = Operations()
		self.plot = Plot()
		self.continue_train = True
		
		self.trainParam = TrainParam()

		self._n_layers = 0
		self._n_inputs = 0
		self._n_outputs = 0

		self.input = None
		self.layers = None
		self.output = None
		self.expected_output = None
		