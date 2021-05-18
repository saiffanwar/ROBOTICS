import numpy as np
from matplotlib import pyplot as plt
from sklearn.linear_model import LinearRegression

r_n = np.array([0,360, 360, 360, 720, 720, 720, 1080, 1080, 1080, 1440, 1440, 1440]).reshape(-1,1)
r_a = np.array([0,381.33, 380.37, 381.8, 760.91, 760.91, 762.58, 1143.4, 1144.1, 1143.43, 1523.48, 1522.4, 1522.67]).reshape(-1,1)

reg = LinearRegression(fit_intercept=False).fit(r_a, r_n)

print(reg.coef_)

r_p = reg.predict(np.array(r_n))

fig = plt.figure()
ax = fig.add_subplot(111)

ax.scatter(r_a, r_n)
ax.plot(r_n, r_p, c='r')

plt.show()