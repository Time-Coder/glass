import numpy as np 
from matplotlib import pyplot as plt
import random
import datetime
import os
import cv2

import sys
sys.path.append('../')
from tfnn import *

def mkdir(path):
	if not os.path.exists(path):
		os.makedirs(path)

def ls(path):
	return os.listdir(path)

class HandWritingPanel:
	def __init__(self, width = 400, height = 400):
		self.text = None
		self.x = []
		self.y = []
		self.mouse_left_pressed = False
		self.width = width
		self.height = height
		self.width_for_net = 28
		self.height_for_net = 28

	def init_panel(self):
		self.fig = plt.figure(figsize=(self.width/100, self.height/100))
		self.fig.canvas.mpl_connect('button_press_event', self.mouse_left_press)
		self.fig.canvas.mpl_connect('button_release_event', self.mouse_left_release)
		self.fig.canvas.mpl_connect('motion_notify_event', self.mouse_move)
		ax = plt.axes([0, 0, 1, 1])
		ax.set_xlim(0, 1)
		ax.set_ylim(0, 1)
		self.curve, = ax.plot(self.x, self.y, linewidth=10, color='k')
		plt.axis('off')
		
	def collect(self):
		self.init_panel()
		self.fig.canvas.mpl_connect('key_press_event', self.enter_press_collect)
		self.update_collect(True)
		plt.show()

	def train(self):
		P, T = self.get_train_data('handwriting_images')

		trainParam = TrainParam()
		trainParam.map_input = False
		trainParam.map_output = False
		trainParam.loss_func = 'sparse_cross_entropy'
		trainParam.activation = 'ReLU'
		trainParam.optimizer = 'adam'
		trainParam.learning_rate = 0.1
		trainParam.learning_rate_func = 'exp_decay'
		trainParam.learning_rate_decay = 0.999
		trainParam.epochs = 200
		trainParam.plot_flag = True

		self.net = BPNN([self.width_for_net * self.height_for_net, 500, 10], trainParam)
		self.net.train(P, T)
		self.net.save('core')

		print(round(len(np.where(np.argmax(self.net(P), axis=0) == np.argmax(T,axis=0))[0])/P.shape[1]*100, 2))

	def test(self):
		self.init_panel()
		self.fig.canvas.mpl_connect('key_press_event', self.enter_press_test)
		self.net = BPNN()
		self.net.load('core')
		plt.show()

	def preprocess(self, filename):
		image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
		image = np.array((255 - image)/255)
		it_rows, it_cols = np.where(image >= 0.5)
		image = image[it_rows.min():it_rows.max()+1, it_cols.min():it_cols.max()]
		image = cv2.resize(image, (self.width_for_net, self.height_for_net))
		image[image < 0.5] = 0
		image[image >= 0.5] = 1
		return image

	def get_train_data(self, folder_name):
		P = []
		T = []
		rows = self.width_for_net * self.height_for_net
		for i in range(10):
			files = ls(folder_name + '/' + str(i))
			for file in files:
				filename = folder_name + '/' + str(i) + '/' + file
				image = self.preprocess(filename)
				p = np.reshape(image, (rows,))
				t = np.zeros((10,))
				t[i] = 1
				P.append(p)
				T.append(t)

		P = np.array(P).T
		T = np.array(T).T
		return P, T

	def mouse_left_press(self, event):
		self.mouse_left_pressed = True

	def mouse_left_release(self, event):
		self.mouse_left_pressed = False

	def mouse_move(self, event):
		if self.mouse_left_pressed:
			self.x.append(event.x/self.width)
			self.y.append(event.y/self.height)
			# print(self.x[-1], ',', self.y[-1])
		else:
			self.x.append(None)
			self.y.append(None)

		self.curve.set_data(self.x, self.y)
		plt.draw()

	def enter_press_collect(self, event):
		if event.key == 'enter':
			self.update_collect()

	def update_collect(self, first = False):
		if not self.text is None:
			self.text.remove()

		if not first:
			self.save()

		self.x = []
		self.y = []
		self.curve.set_data([], [])
		self.current_number = random.randint(0, 9)
		self.text = plt.text(0.5, 1, "Please write " + str(self.current_number) + ", then press enter.", horizontalalignment='center', verticalalignment='top', fontsize=15)
		plt.draw()

	def save(self):
		mkdir('handwriting_images')
		mkdir('handwriting_images/' + str(self.current_number))
		filename = 'handwriting_images/' + str(self.current_number) + '/' + datetime.datetime.strftime(datetime.datetime.now(), '%Y%m%d%H%M%S') + '.jpg'
		plt.savefig(filename)

	def enter_press_test(self, event):
		if event.key == 'enter':
			self.update_test()

	def update_test(self):
		if not self.text is None:
			self.text.remove()

		filename = 'handwriting_images/temp.jpg'
		plt.savefig(filename)
		self.current_number = self.identify(filename)
		self.x = []
		self.y = []
		self.curve.set_data([], [])
		self.text = plt.text(0.5, 1, "You just write " + str(self.current_number), horizontalalignment='center', verticalalignment='top', fontsize=15)
		plt.draw()

	def identify(self, filename):
		image = self.preprocess(filename)
		p = np.reshape(image, (self.width_for_net * self.height_for_net, ))
		t_real = self.net(p)
		return np.argmax(t_real, axis = 0)