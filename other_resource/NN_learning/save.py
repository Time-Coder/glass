import pickle
import os
import scipy.io
import numpy as np

def save(filename, var):
	file = open(filename, 'wb')
	pickle.dump(var, file)
	file.close()

def load(filename):
	if filename[-4:] == '.mat':
		pos1 = filename.rfind('/')
		pos2 = filename.rfind('\\')
		pos = max(pos1, pos2)
		var_name = ''
		if pos == -1:
			var_name = filename[:-4]
		else:
			var_name = filename[pos+1 : -4]
		mat = scipy.io.loadmat(filename)[var_name]
		shape = mat.shape
		return np.reshape(mat.flatten(), shape)
	else:
		file = open(filename, 'rb')
		var = pickle.load(file)
		file.close()
		return var

def mkdir(path):
	if not os.path.exists(path):
		os.makedirs(path)