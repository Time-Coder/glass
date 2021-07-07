from mpl_toolkits.mplot3d import Axes3D 
from matplotlib import cm 
from matplotlib.ticker import LinearLocator, FormatStrFormatter 
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Ellipse, Circle

def peaks(n = 50):
	x = np.linspace(-3, 3, n)
	y = np.linspace(-3, 3, n)
	x, y = np.meshgrid(x, y)
	z = 3*np.square(1-x) * np.exp(-x*x - np.square(y+1)) - 10*(x/5 - np.power(x, 3) - np.power(y, 5))*np.exp(- x*x - y*y) - np.exp(-np.square(x+1) - y*y)/3
	# z =  3*(1-x).^2.*exp(-(x.^2) - (y+1).^2) - 10*(x/5 - x.^3 - y.^5).*exp(-x.^2-y.^2) - 1/3*exp(-(x+1).^2 - y.^2) 
	return x, y, z

def surf(x, y, z, color = None):
	fig = plt.figure()
	ax = fig.gca(projection='3d')
	if color is None:
		surfase = ax.plot_surface(x, y, z, linewidth=0, antialiased=True)
	else:
		surfase = ax.plot_surface(x, y, z, linewidth=0, antialiased=True, color=color)

	return surfase