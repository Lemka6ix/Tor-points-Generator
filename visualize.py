import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('points.txt')

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

ax.scatter(data[:,0], data[:,1], data[:,2], 
           c='red', s=5, alpha=0.7)

x_range = data[:,0].max() - data[:,0].min()
y_range = data[:,1].max() - data[:,1].min()
z_range = data[:,2].max() - data[:,2].min()

max_range = max(x_range, y_range, z_range)

x_center = (data[:,0].max() + data[:,0].min()) / 2
y_center = (data[:,1].max() + data[:,1].min()) / 2
z_center = (data[:,2].max() + data[:,2].min()) / 2

ax.set_xlim(x_center - max_range/2, x_center + max_range/2)
ax.set_ylim(y_center - max_range/2, y_center + max_range/2)
ax.set_zlim(z_center - max_range/2, z_center + max_range/2)

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('3D Points from points.txt')

plt.show()