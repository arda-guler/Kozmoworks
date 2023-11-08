## To be used to visualize results of luna_pointmass
## or luna_sphericalharmonics scenarios

import csv
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
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

# Initialize lists to store x, y, and z coordinates
x_coords = []
y_coords = []
z_coords = []

# Read the CSV file
csv_file = '9000.csv'  # Replace with your CSV file's path
with open(csv_file, mode='r') as file:
    reader = csv.reader(file)
    next(reader)  # Skip header if present

    for row in reader:
        if len(row) >= 3:
            x_coords.append(float(row[0]))
            y_coords.append(float(row[1]))
            z_coords.append(float(row[2]))

# Create a 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the trajectory
ax.plot(x_coords, y_coords, z_coords, label='Trajectory 1', lw=1)

# put Earth
phi = np.linspace(0,2*np.pi, 256).reshape(256, 1) # the angle of the projection in the xy-plane
theta = np.linspace(0, np.pi, 256).reshape(-1, 256) # the angle from the polar axis, ie the polar angle
equatorial_radius = 1738100
polar_radius =      1736000

xe = equatorial_radius*np.sin(theta)*np.cos(phi)
ye = equatorial_radius*np.sin(theta)*np.sin(phi)
ze = polar_radius*np.cos(theta)

ax.plot_surface(xe, ye, ze, color='gray', alpha=0.4)

# Set labels and title
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Trajectory Plot')

ax.set_box_aspect([1, 1, 1])
set_axes_equal(ax)

# Show the plot
plt.show()

