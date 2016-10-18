import sys
import itertools
from matplotlib import pyplot as plt

def read_clusters(fname):
	clusters = []
	with open("data_new") as file:
		data = file.read().splitlines()
	for line in data:
		clusters.append(map(float, line.split("\t")))
	return clusters

if __name__ == "__main__":
	clusters = read_clusters("data_new")
	fig = plt.figure()
	ax = fig.add_subplot(111)

	ax.scatter([i[0] for i in clusters], [i[1] for i in clusters])
	# palette = itertools.cycle(sns.color_palette("Set2", len(clusters)))
 #        for (x, y) in filter(lambda x:x, clusters):
	# 	ax.scatter(x, y, s = 20, color=next(palette))
	plt.show()
