import cv2
import numpy as np

def height2normal(height_map):
	normal_map = np.zeros((height_map.shape[0], height_map.shape[1], 3))
	dx = 255*2/height_map.shape[1]
	dy = 255*2/height_map.shape[0]
	for i in range(height_map.shape[0]):
		i_lower = i-1 if i-1 >= 0 else i
		i_upper = i+1 if i+1 < height_map.shape[0] else i
		for j in range(height_map.shape[1]):
			j_lower = j-1 if j-1 >= 0 else j
			j_upper = j+1 if j+1 < height_map.shape[1] else j
			dz = ((height_map[i_lower][j_upper] + 2*height_map[i][j_upper] + height_map[i_upper][j_upper]) - \
			      (height_map[i_lower][j_lower] + 2*height_map[i][j_lower] + height_map[i_upper][j_lower]))/4
			dz_dx = dz/dx

			dz = ((height_map[i_lower][j_lower] + 2*height_map[i_lower][j] + height_map[i_lower][j_upper]) - \
			      (height_map[i_upper][j_lower] + 2*height_map[i_upper][j] + height_map[i_upper][j_upper]))/4
			dz_dy = dz/dy

			normal = np.array([1, -dz_dy, -dz_dx])
			normal = 255*normal/np.sqrt(np.sum(normal*normal))
			normal_map[i, j] = (normal+255)/2

	return normal_map

height_map = cv2.imread("depth2.png", cv2.IMREAD_GRAYSCALE)
normal_map = height2normal(255-height_map)
cv2.imwrite("normal_test.png", normal_map)