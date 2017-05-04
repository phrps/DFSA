import numpy as np
import matplotlib.pyplot as plt
import os

def add_points(ax,tags,alls,color,marker,label):
	x=np.array(tags)
	y=np.array(alls)
	ax.scatter(x,y,s=10,c=color,marker=marker,linestyle='-',label=label)
	plt.legend(loc='upper left')
	plt.plot(x,y,c=color,linestyle='-')

def set_window(num,name):
	fig = plt.figure(num)
	fig.canvas.set_window_title(name)
	ax1 = fig.add_subplot(111)
	plt.grid()
	return ax1,fig

def plot_num_slots(lower,eomLee,Q=[]):
	name = 'Numero de slots'
	ax,fig=set_window(1,name)
	eixo = [lower[0][0],lower[0][-1],0,3500]
	plt.axis(eixo)
	add_points(ax,lower[0],lower[1],'r','o','Lower Bound')
	add_points(ax,eomLee[0],eomLee[1],'b','s','Eom-Lee')
	add_points(ax,Q[0],Q[1],'g','o','Q algorithm')
	fig.savefig('images/' + name)


def plot_num_collisions(lower,eomLee,Q=[]):
	name = 'Numero de colisoes'
	ax,fig=set_window(2,name)
	eixo = [lower[0][0],lower[0][-1],0,1800]
	plt.axis(eixo)
	add_points(ax,lower[0],lower[3],'r','o','Lower Bound')
	add_points(ax,eomLee[0],eomLee[3],'b','s','Eom-Lee')
	add_points(ax,Q[0],Q[3],'g','o','Q algorithm')
	fig.savefig('images/' + name)

def plot_num_empty(lower,eomLee,Q=[]):
	name = 'Numero de slots vazios'
	ax,fig=set_window(3,name)
	eixo = [lower[0][0],lower[0][-1],0,1100]
	plt.axis(eixo)
	add_points(ax,lower[0],lower[2],'r','o','Lower Bound')
	add_points(ax,eomLee[0],eomLee[2],'b','s','Eom-Lee')
	add_points(ax,Q[0],Q[2],'g','o','Q algorithm')
	fig.savefig('images/' + name)

def plot_time(lower,eomLee,Q=[]):
	name = 'Tempo para identificacao'
	ax,fig=set_window(4,name)
	eixo = [lower[0][0],lower[0][-1],0,10]
	plt.axis(eixo)
	add_points(ax,lower[0],lower[4],'r','o','Lower Bound')
	add_points(ax,eomLee[0],eomLee[4],'b','s','Eom-Lee')
	add_points(ax,Q[0],Q[4],'g','o','Q algorithm')
	fig.savefig('images/' + name)

# Read from file name and return a tuple with a set of each one
def get_points(name):
	arq = open(name,'r')
	points=[]
	points = arq.read().split()
	tags=[]
	alls=[]
	emptys=[]
	collisions=[]
	times=[]
	i=0
	while i < len(points):
		tags.append(int(points[i]))
		alls.append(int(points[i+1]))
		emptys.append(int(points[i+2]))
		collisions.append(int(points[i+3]))
		times.append(float(points[i+4]))
		i=i+5
	# arq.close()
	return (tags,alls,emptys,collisions,times)

os.system('rm -rf *.png')
os.system('mkdir -p images')
lowerBound = get_points('lower.out')
eomLee = get_points('lee.out')
Q = get_points('q.out')
plot_num_slots(lowerBound,eomLee,Q)
plot_num_collisions(lowerBound,eomLee,Q)
plot_num_empty(lowerBound,eomLee,Q)
plot_time(lowerBound,eomLee,Q)
plt.show()