import cv2
import numpy as np
import math

def interp2(image, i, j):
	i_lower = math.floor(i)
	i_upper = math.ceil(i)
	j_lower = math.floor(j)
	j_upper = math.ceil(j)
	i_rear = i - i_lower
	j_rear = j - j_lower

	return (1-j_rear)*((1-i_rear)*image[i_lower,j_lower] + i_rear*image[i_upper,j_lower]) + \
	       j_rear*((1-i_rear)*image[i_lower,j_upper] + i_rear*image[i_upper,j_upper])

def merge_map(color_map, cloud_map, cloud_map_transparent):
	dest_map = np.zeros_like(color_map)
	for i in range(color_map.shape[0]):
		i_cloud_map = i/(color_map.shape[0]-1)*(cloud_map.shape[0]-1)
		i_cloud_map_transparent = i/(color_map.shape[0]-1)*(cloud_map_transparent.shape[0]-1)
		for j in range(color_map.shape[1]):
			j_cloud_map = j/(color_map.shape[1]-1)*(cloud_map.shape[1]-1)
			j_cloud_map_transparent = j/(color_map.shape[1]-1)*(cloud_map_transparent.shape[1]-1)
			transparent = interp2(cloud_map_transparent, i_cloud_map_transparent, j_cloud_map_transparent)/255
			cloud_map_value = interp2(cloud_map, i_cloud_map, j_cloud_map)
			dest_map[i,j,0] = transparent*color_map[i,j,0] + (1-transparent)*cloud_map_value[0]
			dest_map[i,j,1] = transparent*color_map[i,j,1] + (1-transparent)*cloud_map_value[1]
			dest_map[i,j,2] = transparent*color_map[i,j,2] + (1-transparent)*cloud_map_value[2]

	return dest_map

file_names = ["earth_map_1K.jpg", "earth_map_2K.jpg", "earth_map_4K.jpg", "earth_map_10K.jpg"]
cloud_map = cv2.imread("earth_cloudmap.jpg", cv2.IMREAD_UNCHANGED)
cloud_map_transparent = cv2.imread("earth_cloudmaptrans.jpg", cv2.IMREAD_UNCHANGED)
for file_name in file_names:
	print(file_name)
	color_map = cv2.imread(file_name, cv2.IMREAD_UNCHANGED)
	dest_map = merge_map(color_map, cloud_map, cloud_map_transparent)
	cv2.imwrite(file_name.replace("_map_", "_colormap_"), dest_map)