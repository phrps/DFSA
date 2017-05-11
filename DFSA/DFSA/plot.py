import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import os

def add_points(ax,tags,alls,color,marker,label):
	x=np.array(tags)
	y=np.array(alls)
	ax.scatter(x,y,s=10,c=color,marker=marker,linestyle='-',label=label)
	plt.plot(x,y,c=color,linestyle='-')

def set_window(num,name):
	fig = plt.figure(num)
	fig.canvas.set_window_title(name)
	ax1 = fig.add_subplot(111)
	ax1.set_xlabel('Numero de tags')
	blue_patch = mpatches.Patch(color='blue', label='Eom-Lee')
	red_patch = mpatches.Patch(color='red', label='Lower Bound')
	green_patch = mpatches.Patch(color='green', label='Adaptive Q')
	yellow_patch = mpatches.Patch(color='yellow', label='Fast Q')
	black_patch = mpatches.Patch(color='black', label='Chen')
	orange_patch = mpatches.Patch(color='cyan', label='Vahedi')
	plt.legend(loc='upper left',handles=[red_patch,blue_patch,green_patch,yellow_patch,black_patch,orange_patch])
	plt.grid()
	return ax1,fig

def plot_num_slots(lower,eomLee,fastQ=[],adapQ=[],chen=[],vahedi=[]):
	name = 'Numero de slots'
	ax,fig=set_window(1,name)
	ax.set_ylabel('Numero de slots')
	eixo = [lower[0][0],lower[0][-1],0,3500]
	plt.axis(eixo)
	add_points(ax,lower[0],lower[1],'r','o','Lower Bound')
	add_points(ax,eomLee[0],eomLee[1],'b','s','Eom-Lee')
	add_points(ax,fastQ[0],fastQ[1],'y','o','Fast Q')
	add_points(ax,adapQ[0],adapQ[1],'g','o','Adaptive Q')
	add_points(ax,chen[0],chen[1],'k','o','Chen')
	add_points(ax,vahedi[0],vahedi[1],'c','o','Vahedi')
	fig.savefig('images/' + name)


def plot_num_collisions(lower,eomLee,fastQ=[],adapQ=[],chen=[],vahedi=[]):
	name = 'Numero de colisoes'
	ax,fig=set_window(2,name)
	ax.set_ylabel('Numero de colisoes')
	eixo = [lower[0][0],lower[0][-1],0,1800]
	plt.axis(eixo)
	add_points(ax,lower[0],lower[3],'r','o','Lower Bound')
	add_points(ax,eomLee[0],eomLee[3],'b','s','Eom-Lee')
	add_points(ax,fastQ[0],fastQ[3],'y','o','Fast Q')
	add_points(ax,adapQ[0],adapQ[3],'g','o','Adaptive Q')
	add_points(ax,chen[0],chen[3],'k','o','Chen')
	add_points(ax,vahedi[0],vahedi[3],'c','o','Vahedi')
	fig.savefig('images/' + name)

def plot_num_empty(lower,eomLee,fastQ=[],adapQ=[],chen=[],vahedi=[]):
	name = 'Numero de slots vazios'
	ax,fig=set_window(3,name)
	ax.set_ylabel('Numero de slots vazios')
	eixo = [lower[0][0],lower[0][-1],0,2500]
	plt.axis(eixo)
	add_points(ax,lower[0],lower[2],'r','o','Lower Bound')
	add_points(ax,eomLee[0],eomLee[2],'b','s','Eom-Lee')
	add_points(ax,fastQ[0],fastQ[2],'y','o','Fast Q')
	add_points(ax,adapQ[0],adapQ[2],'g','o','Adaptive Q')
	add_points(ax,chen[0],chen[2],'k','o','Chen')
	add_points(ax,vahedi[0],vahedi[2],'c','o','Vahedi')

	fig.savefig('images/' + name)

def plot_time(lower,eomLee,fastQ=[],adapQ=[],chen=[],vahedi=[],log=False):
	name = 'Tempo para identificacao'
	ax,fig=set_window(4,name)
	ax.set_ylabel('Tempo de identificacao (ms)')
	if log:
		name = 'Tempo para identificacao (escala logaritmica)'
		ax.set_yscale('log')
	eixo = [lower[0][0],lower[0][-1],0,4000]
	plt.axis(eixo)
	add_points(ax,lower[0],lower[4],'r','o','Lower Bound')
	add_points(ax,eomLee[0],eomLee[4],'b','s','Eom-Lee')
	add_points(ax,fastQ[0],fastQ[4],'y','o','Fast Q')
	add_points(ax,adapQ[0],adapQ[4],'g','o','Adaptive Q')
	add_points(ax,chen[0],chen[4],'k','o','Chen')
	add_points(ax,vahedi[0],vahedi[4],'c','o','Vahedi')
	fig.savefig('images/' + name)

def plot_countIter(chen=[],vahedi=[],log=False):
	name = 'Numero de iteracoes'
	ax,fig=set_window(5,name)
	ax.set_ylabel('Numero de iteracoes')
	if log:
		name = 'Numero de iteracoes'
		ax.set_yscale('log')
	menor=min(min(chen[5]),min(vahedi[5]))
	maior=max(max(chen[5]),max(vahedi[5]))
	eixo = [chen[0][0],chen[0][-1],menor,maior]
	plt.axis(eixo)
	add_points(ax,chen[0],chen[5],'k','o','Chen')
	add_points(ax,vahedi[0],vahedi[5],'c','o','Vahedi')
	fig.savefig('images/' + name)

# Read from file name and return a tuple with a set of each one
def get_points(name,hasCount=False):
	arq = open(name,'r')
	points=[]
	points = arq.read().split()
	tags=[]
	alls=[]
	emptys=[]
	collisions=[]
	times=[]
	countIter=[]
	i=0
	while i < len(points):
		tags.append(int(points[i]))
		alls.append(int(points[i+1]))
		emptys.append(int(points[i+2]))
		collisions.append(int(points[i+3]))
		times.append(float(points[i+4]))
		if hasCount:
			countIter.append(int(points[i+5]))
			i=i+6
		else:
			i=i+5
	# arq.close()
	return (tags,alls,emptys,collisions,times,countIter)

os.system('rm -rf *.png')
os.system('mkdir -p images')
lowerBound = get_points('lower.out')
eomLee = get_points('lee.out')
fastQ = get_points('fast_q.out')
adapQ = get_points('adap_q.out')
chen = get_points('chen.out',True)
vahedi = get_points('vahedi.out',True)
plot_num_slots(lowerBound,eomLee,fastQ,adapQ,chen,vahedi)
plot_num_collisions(lowerBound,eomLee,fastQ,adapQ,chen,vahedi)
plot_num_empty(lowerBound,eomLee,fastQ,adapQ,chen,vahedi)
plot_time(lowerBound,eomLee,fastQ,adapQ,chen,vahedi)
plot_time(lowerBound,eomLee,fastQ,adapQ,chen,vahedi,True)
plot_countIter(chen,vahedi)
# plt.show()