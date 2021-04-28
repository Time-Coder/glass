import cv2
import numpy as np

class Padding:
	def __init__(self, bump_map):
		self.padding_bump = np.zeros((bump_map.shape[0]+2, bump_map.shape[1]+2))
		self.padding_bump[1:-1, 1:-1] = bump_map
		self.padding_bump[0,1:-1] = 2*bump_map[0,:]-bump_map[1,:]
		self.padding_bump[-1,1:-1] = 2*bump_map[-1,:]-bump_map[-2,:]
		self.padding_bump[:,0] = 2*self.padding_bump[:,1]-self.padding_bump[:,2]
		self.padding_bump[:,-1] = 2*self.padding_bump[:,-1]-self.padding_bump[:,-2]

	def __getitem__(self, key):
		return self.padding_bump[key[0]+1, key[1]+1]

def generate_normal_map(bump_map):
	normal_map = np.ones((bump_map.shape[0], bump_map.shape[1], 3))*255
	padding_bump = Padding(bump_map)
	for i in range(normal_map.shape[0]):
		for j in range(normal_map.shape[1]):
			normal_map[i,j,-1] = ((padding_bump[i-1,j-1]+2*padding_bump[i,j-1]+padding_bump[i+1,j-1]) - \
			                     (padding_bump[i-1,j+1]+2*padding_bump[i,j+1]+padding_bump[i+1,j+1])) / 8
			normal_map[i,j,-2] = ((padding_bump[i+1,j-1]+2*padding_bump[i+1,j]+padding_bump[i+1,j+1]) - \
			                     (padding_bump[i-1,j-1]+2*padding_bump[i-1,j]+padding_bump[i-1,j+1])) / 8

	return normal_map

bump_map = cv2.imread("earth_bump_1K.jpg", cv2.IMREAD_UNCHANGED)
normal_map = generate_normal_map(bump_map)
cv2.imwrite("earth_normal_1K.jpg", normal_map)
print(normal_map)