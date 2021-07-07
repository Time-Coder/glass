import numpy as np

import sys
sys.path.append('../')
from tfnn import *

def generate_circle_data(n1 = 200, n2 = 500, dx = 0, dy = 0):
	inner_theta  = 2 * np.pi * np.random.rand(1, n1)
	inner_r = np.random.randn(1, n1)
	inner_x = inner_r*np.cos(inner_theta) + dx
	inner_y = inner_r*np.sin(inner_theta) + dy

	outter_theta = 2 * np.pi * np.random.rand(1, n2)
	outter_r = 0.5*np.random.randn(1, n2)+4
	outter_x = outter_r*np.cos(outter_theta) + dx
	outter_y = outter_r*np.sin(outter_theta) + dy

	return [[inner_x, inner_y], [outter_x, outter_y]]

def generate_half_moon_data(n = 500, R = 4, width = 2, distance = -1, dx = 0, dy = 0):
	theta1 =   np.pi * np.random.rand(1, n)
	theta2 = - np.pi * np.random.rand(1, n)

	r1 = np.random.randn(1, n)/3*width + R
	r2 = np.random.randn(1, n)/3*width + R

	x1 = r1*np.cos(theta1) - R/2 + dx
	y1 = r1*np.sin(theta1) + distance/2 + dy

	x2 = r2*np.cos(theta2) + R/2 + dx
	y2 = r2*np.sin(theta2) - distance/2 + dy

	return [[x1, y1], [x2, y2]]

class Classifier2D:
	def __init__(self):
		self.P = None
		self.T = None
		self.net = None
		self._n_samples = 0
		self._n_groups = 0
		self.groups = None

	def set_data(self, point_groups):
		self.groups = point_groups
		self._n_samples = 0
		self._n_groups = len(point_groups)
		for i in range(self._n_groups):
			self._n_samples += np.array(point_groups[i][0]).flatten().shape[0]
		self.T = np.zeros((self._n_groups, self._n_samples))

		it_sample = 0
		for i in range(self._n_groups):
			for j in range(np.array(point_groups[i][0]).flatten().shape[0]):
				self.T[i, it_sample] = 1
				it_sample += 1

		x = []
		y = []
		for i in range(self._n_groups):
			x.extend(np.array(point_groups[i][0]).flatten().tolist())
			y.extend(np.array(point_groups[i][1]).flatten().tolist())
		self.P = np.vstack((np.array(x).flatten(), np.array(y).flatten()))

	def train(self):
		trainParam = TrainParam()
		trainParam.activation = 'ReLU'
		trainParam.map_output = False
		trainParam.map_input = True
		trainParam.input_map_range = [0, 1]
		trainParam.loss_func = 'sparse_cross_entropy'
		trainParam.optimizer = 'adam'
		trainParam.learning_rate = 0.1
		trainParam.learning_rate_func = 'exp_decay'
		trainParam.learning_rate_decay = 0.99
		trainParam.epochs = 2000
		trainParam.plot_flag = True
		trainParam.plot_type = 'classify'

		self.net = BPNN([2, 50, self.T.shape[0]], trainParam)

		self.net.train(self.P, self.T)
		self.save()

	def show_points(self):
		x_points = self.P[0, :]
		y_points = self.P[1, :]
		it_rows, it_cols = np.where(self.T == 1)
		for i in range(self._n_groups):
			current_group = it_cols[np.where(it_rows == i)]
			plt.plot(x_points[current_group], y_points[current_group], '.')

		plt.axis('equal')
		plt.tight_layout()
		plt.show()

	def show_edge(self):
		x = np.linspace(self.P[0,:].min(), self.P[0,:].max())
		y = np.linspace(self.P[1,:].min(), self.P[1,:].max())
		x, y = np.meshgrid(x, y)
		P = np.vstack((x.flatten(), y.flatten()))
		T_real = self.net(P)
		z = np.reshape(np.argmax(T_real, axis = 0), x.shape)

		contour_levels = np.array(range(self.T.shape[0] + 1)) - 0.5
		plt.contour(x, y, z, contour_levels, colors='k', linewidths=1)
		plt.contourf(x, y, z, contour_levels, alpha=0.4, cmap=plt.cm.terrain)

		x_points = self.P[0, :]
		y_points = self.P[1, :]
		it_groups = []
		it_rows, it_cols = np.where(self.T == 1)

		for i in range(self._n_groups):
			it_groups.append(it_cols[np.where(it_rows == i)])
			plt.plot(x_points[it_groups[-1]], y_points[it_groups[-1]], '.')

		plt.axis('equal')
		plt.tight_layout()
		plt.show()

	def classify(self, x, y):
		is_single_value = False
		is_list = False
		if not isinstance(x, np.ndarray) and not isinstance(x, list):
			is_single_value = True
		elif isinstance(x, list):
			is_list = True

		P = np.vstack((np.array(x).flatten(), np.array(y).flatten()))
		T_real = self.net(P)
		result = np.argmax(T_real, axis = 0)

		if is_single_value:
			return result[0]

		if is_list:
			return result.tolist()

		return result

	def save(self, name = None):
		if name is None:
			name = 'data/autosave/classifier_' + now()

		mkdir('data')
		last_classifier_file = open('data/last_classifier.txt', 'w')
		last_classifier_file.write(name)
		last_classifier_file.close()

		mkdir(name)
		save(name + '/groups.pickle', self.groups)
		save(name + '/P.pickle', self.P)
		save(name + '/T.pickle', self.T)
		save(name + '/n_samples.pickle', self._n_samples)
		save(name + '/n_groups.pickle', self._n_groups)
		self.net.save(name + '/core')

	def load(self, name = None):
		if name is None:
			last_classifier_file = open('data/last_classifier.txt', 'r')
			name = last_classifier_file.read()
			last_classifier_file.close()

		self.groups = load(name + '/groups.pickle')
		self.P = load(name + '/P.pickle')
		self.T = load(name + '/T.pickle')
		self._n_samples = load(name + '/n_samples.pickle')
		self._n_groups = load(name + '/n_groups.pickle')
		self.net = BPNN(name + '/core')