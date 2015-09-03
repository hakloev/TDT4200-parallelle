from matplotlib import pyplot as plt
from matplotlib import style
import numpy as np

style.use('ggplot')

x,y,z,w = np.loadtxt('result.txt',
					unpack=True,
					delimiter = ',')


t = [z[c] for c in range(40, 60)]
u = [w[c] for c in range(40, 60)]

print(t, u)

plt.plot(t, u)

plt.show()
