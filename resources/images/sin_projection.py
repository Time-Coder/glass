import cv2
import numpy as np
import os
import copy

def extname(filename):
	basename = os.path.basename(filename)
	pos_point = basename.rfind(".")
	if pos_point in [-1, len(basename)-1]:
		return ""
	else:
		return basename[pos_point+1:]

def purename(filename):
	basename = os.path.basename(filename)
	pos_point = basename.rfind(".")
	if pos_point == -1:
		return basename
	else:
		return basename[:pos_point]

def ls_all(root):
	files = []
	for root_path, dirnames, filenames in os.walk(root):
		files.extend([os.path.join(root_path, names).replace("\\", "/") for names in filenames])
	return files

def interp2_pixel(image, i, j):
	s = j/(image.shape[1]-1)
	if s < 0 or s > 1:
		s -= np.floor(s)

	j = s*(image.shape[1]-1)

	t = 1-i/(image.shape[0]-1)
	if t < 0 or t > 1:
		t += np.floor(t)

	i = (1-t)*(image.shape[0]-1)

	i_lower = np.floor(i)
	i_upper = np.ceil(i)
	j_lower = np.floor(j)
	j_upper = np.ceil(j)
	i_rear = i - i_lower
	j_rear = j - j_lower

	return (1-j_rear)*((1-i_rear)*image[i_lower,j_lower] + i_rear*image[i_upper,j_lower]) + \
	       j_rear*((1-i_rear)*image[i_lower,j_upper] + i_rear*image[i_upper,j_upper])

def interp2(image, i, j):
	s = j/(image.shape[1]-1)
	s = np.where((s < 0) | (s > 1), s-np.floor(s), s)

	j = s*(image.shape[1]-1)

	t = 1-i/(image.shape[0]-1)
	t = np.where((t < 0) | (t > 1), t-np.floor(t), t)

	i = (1-t)*(image.shape[0]-1)

	i_lower = np.floor(i).astype(int)
	i_upper = np.ceil(i).astype(int)
	j_lower = np.floor(j).astype(int)
	j_upper = np.ceil(j).astype(int)
	_i_rear = i - i_lower
	_j_rear = j - j_lower

	i_rear = np.zeros((*i.shape,image.shape[2]))
	for k in range(0, image.shape[2]):
		i_rear[:,:,k] = _i_rear

	j_rear = np.zeros((*j.shape,image.shape[2]))
	for k in range(0, image.shape[2]):
		j_rear[:,:,k] = _j_rear

	return (1-j_rear)*((1-i_rear)*image[i_lower,j_lower] + i_rear*image[i_upper,j_lower]) + \
	       j_rear*((1-i_rear)*image[i_lower,j_upper] + i_rear*image[i_upper,j_upper])

def sin_projection(src_image):
	dest_image = np.zeros_like(src_image)
	i = np.arange(0, dest_image.shape[0])
	j = np.arange(0, dest_image.shape[1])
	j, i = np.meshgrid(j, i)
	phi = (0.5 - i/(dest_image.shape[0]-1))*np.pi
	theta = 1/np.cos(phi) * (j/(dest_image.shape[1]-1)-0.5)
	i_src = i
	j_src = (theta+0.5)*(dest_image.shape[1]-1)
	return interp2(src_image, i_src, j_src)

def sin_projection_row(src_image):
	dest_image = np.zeros_like(src_image)
	j = np.expand_dims(np.arange(0, dest_image.shape[1]), 0)
	for i in range(dest_image.shape[0]):
		phi = (0.5 - i/(dest_image.shape[0]-1))*np.pi
		phi = phi*np.ones((dest_image.shape[1],))
		theta = 1/np.cos(phi) * (j/(dest_image.shape[1]-1)-0.5)
		i_src = i*np.ones((1,dest_image.shape[1]))
		j_src = (theta+0.5)*(dest_image.shape[1]-1)
		dest_image[i_src.astype(int),j.astype(int)] = interp2(src_image, i_src, j_src)
	return dest_image

files = ls_all(".")
for file in files:
	if extname(file) in ["jpg", "png", "bmp"] and file.find("_sin") == -1:
		print(file)
		dest_name = os.path.dirname(file) + "/" + purename(file) + "_sin." + extname(file)
		try:
			dest_image = sin_projection_row(cv2.imread(file, cv2.IMREAD_UNCHANGED))
			cv2.imwrite(dest_name, dest_image)
		except IndexError as e:
			print(e)

		# try:
		# 	dest_image = sin_projection(cv2.imread(file, cv2.IMREAD_UNCHANGED))
		# 	cv2.imwrite(dest_name, dest_image)
		# except MemoryError as e:
		# 	try:
		# 		dest_image = sin_projection_row(cv2.imread(file, cv2.IMREAD_UNCHANGED))
		# 		cv2.imwrite(dest_name, dest_image)
		# 	except IndexError as e:
		# 		pass
		# except IndexError as e:
		# 	pass