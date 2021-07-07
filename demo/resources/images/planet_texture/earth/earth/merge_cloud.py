import cv2
import numpy as np

def merge_cloud(cloud_map, cloud_transparent):
	dest_image = np.zeros((cloud_map.shape[0], cloud_map.shape[1], 4))
	dest_image[:,:,0:3] = cloud_map
	dest_image[:,:,3] = 255-cloud_transparent
	return dest_image

cloud_map = cv2.imread("earth_cloudmap.jpg", cv2.IMREAD_UNCHANGED)
cloud_transparent = cv2.imread("earth_cloudmaptrans.jpg", cv2.IMREAD_UNCHANGED)
merged_cloud_map = merge_cloud(cloud_map, cloud_transparent)
cv2.imwrite("earth_cloudmap.png", merged_cloud_map)