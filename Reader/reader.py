

from coords import coords_list
from matplotlib import pyplot as plt
import numpy as np
from scipy.interpolate import interp1d # Different interface to the same function

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

	#return toBinary(char1) + (toBinary(char2))


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


for i in range(len(xi)-1):

	if xi[i+1] == xi[i]:
		norm = np.array([1,0])

	elif yi[i+1] == yi[i]:
		norm = np.array([0,1])

	else:
		deriv = np.array([xi[i+1]-xi[i], yi[i+1]-yi[i]])

		r_m = np.zeros((2,2))
		r_m[0,0] = np.cos(np.pi/2)
		r_m[1,1] = np.cos(np.pi/2)
		r_m[1,0] = np.sin(np.pi/2)
		r_m[0,1] = -np.sin(np.pi/2)

	#	norm = np.dot(r_m,deriv) # normal

		norm = np.array([-deriv[1], deriv[0]])

		norm = norm / np.linalg.norm(norm) # normalize

	# #norm = norm*10
	if disti[i] < 130:
		surf.append(np.array([xi[i], yi[i]]) + norm*disti[i])


	#ax.arrow(xi[i], yi[i], 10*norm[0], 10*norm[1])

box_x = 290
box_y = 290
offset = 100

box_coords = np.array([[0, offset], [box_x, offset], [box_x, offset+box_y], [0, offset+box_y], [0,offset]])+np.array([50,75])

surf = np.array(surf)

# plot path 
fig, ax = plt.subplots(figsize=(8,8))
ax.plot(xi, yi)
ax.plot(xs, ys, 'ko')
ax.scatter(surf[:,0], surf[:,1], s=0.1, c='b')
ax.plot(box_coords[:,0], box_coords[:,1])

plt.xlim(-500, 1250)
plt.ylim(-500, 1250)

#ax.plot(range(len(disti)), disti)

plt.show()

