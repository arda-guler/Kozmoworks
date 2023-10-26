import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np

def set_axes_equal(ax):
    x_limits = ax.get_xlim3d()
    y_limits = ax.get_ylim3d()
    z_limits = ax.get_zlim3d()

    x_range = abs(x_limits[1] - x_limits[0])
    x_middle = np.mean(x_limits)
    y_range = abs(y_limits[1] - y_limits[0])
    y_middle = np.mean(y_limits)
    z_range = abs(z_limits[1] - z_limits[0])
    z_middle = np.mean(z_limits)

    plot_radius = 0.5*max([x_range, y_range, z_range])

    ax.set_xlim3d([x_middle - plot_radius, x_middle + plot_radius])
    ax.set_ylim3d([y_middle - plot_radius, y_middle + plot_radius])
    ax.set_zlim3d([z_middle - plot_radius, z_middle + plot_radius])

csv_file = 'output.csv'
data = pd.read_csv(csv_file, header=None, names=['x', 'y', 'z'])

x = data['x']
y = data['y']
z = data['z']

csv_file = 'output2.csv'
data2 = pd.read_csv(csv_file, header=None, names=['x', 'y', 'z'])

x2 = data2['x']
y2 = data2['y']
z2 = data2['z']

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d', computed_zorder=False)

# put Earth
phi = np.linspace(0,2*np.pi, 256).reshape(256, 1) # the angle of the projection in the xy-plane
theta = np.linspace(0, np.pi, 256).reshape(-1, 256) # the angle from the polar axis, ie the polar angle
equatorial_radius = 6378137
polar_radius = 6356752.3

xe = equatorial_radius*np.sin(theta)*np.cos(phi)
ye = equatorial_radius*np.sin(theta)*np.sin(phi)
ze = polar_radius*np.cos(theta)

ax.plot_surface(xe, ye, ze, color='b', zorder=0, alpha=0.4)

# put trajectories
ax.plot(x, y, z, label='Trajectory 1', lw=2, zorder=1)
ax.plot(x2, y2, z2, label='Trajectory 2', lw=2, zorder=1)

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Trajectory Plot')

ax.set_box_aspect([1, 1, 1])
set_axes_equal(ax)

plt.legend()
plt.show()
