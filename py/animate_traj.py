import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation
from matplotlib.animation import PillowWriter
import csv
import numpy as np

x_data1 = []
y_data1 = []
z_data1 = []

with open('9000.csv', 'r') as file:
    csv_reader = csv.reader(file)
    for row in csv_reader:
        if len(row) >= 3:
            x_data1.append(float(row[0]))
            y_data1.append(float(row[1]))
            z_data1.append(float(row[2]))

x_data2 = []
y_data2 = []
z_data2 = []

with open('9001.csv', 'r') as file:
    csv_reader = csv.reader(file)
    for row in csv_reader:
        if len(row) >= 3:
            x_data2.append(float(row[0]))
            y_data2.append(float(row[1]))
            z_data2.append(float(row[2]))

def calculate_max_distance(x, y, z):
    distances = [(xi**2 + yi**2 + zi**2)**0.5 for xi, yi, zi in zip(x, y, z)]
    return max(distances)

max_distance1 = calculate_max_distance(x_data1, y_data1, z_data1)
max_distance2 = calculate_max_distance(x_data2, y_data2, z_data2)

axis_limit = 1.2 * max(max_distance1, max_distance2)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.set_xlim(-axis_limit, axis_limit)
ax.set_ylim(-axis_limit, axis_limit)
ax.set_zlim(-axis_limit, axis_limit)

line1, = ax.plot([], [], [], lw=2)
line2, = ax.plot([], [], [], lw=2)

scatter1 = ax.scatter([], [], [], c='red', marker='o', s=100, label='Last Position (Trajectory 1)')
scatter2 = ax.scatter([], [], [], c='blue', marker='o', s=100, label='Last Position (Trajectory 2)')

def init():
    line1.set_data([], [])
    line1.set_3d_properties([])
    line2.set_data([], [])
    line2.set_3d_properties([])
    scatter1.set_offsets([None, None])
    scatter2.set_offsets([None, None])
    return line1, line2, scatter1, scatter2

window_size = 2000
def update(frame):
    start_frame = max(0, frame - window_size)
    
    x1 = x_data1[start_frame:frame]
    y1 = y_data1[start_frame:frame]
    z1 = z_data1[start_frame:frame]
    
    x2 = x_data2[start_frame:frame]
    y2 = y_data2[start_frame:frame]
    z2 = z_data2[start_frame:frame]

    line1.set_data(x1, y1)
    line1.set_3d_properties(z1)
    line2.set_data(x2, y2)
    line2.set_3d_properties(z2)
    
    last_x1 = x_data1[frame - 1] if frame > 0 else None
    last_y1 = y_data1[frame - 1] if frame > 0 else None
    last_z1 = z_data1[frame - 1] if frame > 0 else None
    
    last_x2 = x_data2[frame - 1] if frame > 0 else None
    last_y2 = y_data2[frame - 1] if frame > 0 else None
    last_z2 = z_data2[frame - 1] if frame > 0 else None
    
    scatter1.set_offsets([(last_x1, last_y1, last_z1)])
    scatter2.set_offsets([(last_x2, last_y2, last_z2)])

    return line1, line2, scatter1, scatter2

phi = np.linspace(0,2*np.pi, 256).reshape(256, 1)
theta = np.linspace(0, np.pi, 256).reshape(-1, 256)
equatorial_radius = 6378137
polar_radius = 6356752.3

xe = equatorial_radius*np.sin(theta)*np.cos(phi)
ye = equatorial_radius*np.sin(theta)*np.sin(phi)
ze = polar_radius*np.cos(theta)

ax.plot_surface(xe, ye, ze, color='b', alpha=0.4)

num_frames = max(len(x_data1), len(x_data2))
ani = FuncAnimation(fig, update, init_func=init, frames=num_frames, repeat=False, blit=True, interval=2)

ax.set_xlabel('X (m)')
ax.set_ylabel('Y (m)')
ax.set_zlabel('Z (m)')
plt.title("Trajectory")
## ax.legend()
ax.set_box_aspect([1, 1, 1])

plt.show()

ani.save("traj.mp4")
