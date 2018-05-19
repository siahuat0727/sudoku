import matplotlib.pyplot as plt

def read_list(file_name):
  f = open(file_name, "r")
  return [int(x[:-1]) for x in f]

x1 = read_list("out3")
x2 = read_list("out2")

x_m = 100
y_m = 12000

plt.subplot(211)
plt.plot(x1)
plt.xlim(0, x_m)
plt.ylim(0, y_m)

plt.subplot(212)
plt.plot(x2)
plt.xlim(0, x_m)
plt.ylim(0, y_m)
plt.show()
