

from coords import coords_list
from matplotlib import pyplot as plt
import numpy as np
from scipy.interpolate import interp1d # Different interface to the same function

def error_(verts, points):

	"""
	verts : four vertices of mapped box
	points : points mapped by sensor

	"""

	x_u = np.max(verts[:,0])
	x_l = np.min(verts[:,0])
	y_u = np.max(verts[:,1])
	y_l = np.min(verts[:,1])

	sum_error = 0

	for point in points:

		# use closest edge
		if (point[0] < x_u and point[0] > x_l and point[1] < y_u and point[1] > y_l):

			err = np.min([abs(x_u-point[0]), abs(x_l-point[0]), abs(y_u-point[1]), abs(y_l-point[1])])

		# use closest vertex
		else:

			err = np.min([np.linalg.norm(point-vert) for vert in verts])

		sum_error += err**2

	return sum_error / len(points)



def to_signed_int(bin_string):

	bin_string = bin_string[::-1]

	integer = 0

	for i in range(15):

		if bin_string[i] == '1':
			integer += 2**(i)

	if bin_string[15] == '1':

		return integer -32768
	else:
		return integer



def read_short_int(char1, char2):

	return format(char2, '0{}b'.format(8)) + format(char1, '0{}b'.format(8))


# define box dimensions
box_x = 580
box_y = 790
offset = 100

box_coords = np.array([[0, offset], [box_x, offset], [box_x, offset+box_y], [0, offset+box_y], [0,offset]])+np.array([50,75])

box_centroid = np.mean(box_coords[:4], axis=0)

print(box_centroid)

coords_list = [i for i in coords_list if (((i[0]) and (i[2]) !=0))]

#print(coords_list)

coords = np.array([[to_signed_int( read_short_int(i[0], i[1]) ), to_signed_int(read_short_int(i[2], i[3]))] for i in coords_list])

#coords = coords + np.array([50, -75]) # linear transform from kinematics loc to proximity sensor

dists = [i[4] for i in coords_list]


xs = [i[0] for i in coords]
ys = [i[1] for i in coords]


# generate interpolation
i = np.arange(len(coords))

# 5x the original number of points
interp_i = np.linspace(0, i.max(), 5 * i.max())

xi = interp1d(i, xs, kind='cubic')(interp_i)
yi = interp1d(i, ys, kind='cubic')(interp_i)
disti = interp1d(i, dists, kind='cubic')(interp_i)

path = np.array(list(zip(xi,yi)))

# calculate norm vectors
surf = []
fig, ax = plt.subplots(figsize=(8,8))

for i in range(len(xs)-1):

	if xs[i+1] == xs[i]:
		norm = np.array([1,0])

	elif ys[i+1] == ys[i]:
		norm = np.array([0,1])

	else:
		deriv = np.array([xs[i+1]-xs[i], ys[i+1]-ys[i]])

		norm = np.array([-deriv[1], deriv[0]]) # normal from derivative

		# flip normal
		if np.linalg.norm(np.array([xs[i], ys[i]]) + norm*dists[i] - box_centroid, axis=0) > np.linalg.norm(np.array([xs[i], ys[i]]) - box_centroid, axis=0):
			norm = -norm

		norm = norm / np.linalg.norm(norm) # normalize

	norm = -norm

	#if dists[i] < 130:
	surf.append(np.array([xs[i], ys[i]]) + norm*dists[i])

	# plot normal
	#ax.arrow(xs[i], ys[i],  50*norm[0], 50*norm[1], capstyle='projecting')


# for i in range(len(xi)-1):

# 	if xi[i+1] == xi[i]:
# 		norm = np.array([1,0])

# 	elif yi[i+1] == yi[i]:
# 		norm = np.array([0,1])

# 	else:
# 		deriv = np.array([xi[i+1]-xi[i], yi[i+1]-yi[i]])

# 		r_m = np.zeros((2,2))
# 		r_m[0,0] = np.cos(np.pi/2)
# 		r_m[1,1] = np.cos(np.pi/2)
# 		r_m[1,0] = np.sin(np.pi/2)
# 		r_m[0,1] = -np.sin(np.pi/2)

# 		norm = np.array([-deriv[1], deriv[0]])

# 		norm = norm / np.linalg.norm(norm) # normalize

# 	if disti[i] < 130:
# 		surf.append(np.array([xi[i], yi[i]]) + norm*disti[i])



print('Error: ', error_(box_coords[:4], surf))

surf = np.array(surf)

# plot path 
#fig, ax = plt.subplots(figsize=(8,8))
#ax.plot(xi, yi) # interpolated path
ax.plot(xs, ys, 'ko') # recorded coordinates
ax.scatter(surf[:,0], surf[:,1], s=1, c='b') # perceived object mapping

ax.plot(box_coords[:,0], box_coords[:,1], c='r')

plt.xlim(-250, 1000)
plt.ylim(-100, 1250)

plt.show()

