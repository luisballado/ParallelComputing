import time
import matplotlib.pyplot as plt
import numpy as np

def graficar(ys,xc1, yc1, xc16, yc16):

    pxc1 = np.array(xc1)
    pyc1 = np.array(yc1)

    pxc16 = np.array(xc16)
    pyc16 = np.array(yc16)
    
    plt.plot(pxc1, pyc1 ,lw=2, marker="o",linestyle='--',color="#ff3333",label='OPENMP')

    plt.plot(pxc16, pyc16, lw=2, marker="o",linestyle='--',color="#8333ff",label='PTHREADS')
        
    plt.title("ACELERACIÓN 512 ROBOTS")
    plt.xlabel("Número de procesadores")
    plt.ylabel("Aceleración")
    plt.grid()
    plt.legend()
    plt.savefig('aceleracion_512.png')
    plt.show()

xs = [32,64,128,256,512]
ys = [0.554,1.145,1.581,3.410,7.298]
#ys = [0.554,1.024,1.969,3.683,7.498]

#OPENMP 32 ROBOTS
#xc1 = [1,2,4,8,16,32]
#yc1 = [ys[0]/0.554,ys[0]/0.347,ys[0]/0.216,ys[0]/0.144,ys[0]/0.108,ys[0]/0.100]
#yc1 = [ys[0]/(0.554),ys[0]/(0.347),ys[0]/(0.216),ys[0]/(0.144),ys[0]/(0.108),ys[0]/(0.100)]

xc1 = [1,2,4,8,16,32]
yc1 = [ys[4]/7.500,ys[4]/4.231,ys[4]/2.200,ys[4]/1.962,ys[4]/0.673,ys[4]/0.510]

#PTH 32 ROBOTS
#xc162 = [1,2,4,8,16,32]
#yc162 = [ys[0]/0.554,ys[0]/0.359,ys[0]/0.284,ys[0]/0.322,ys[0]/0.422,ys[0]/0.699]
xc162 = [1,2,4,8,16,32]
yc162 = [ys[4]/7.119,ys[4]/3.527,ys[4]/1.582,ys[4]/1.960,ys[4]/0.902,ys[4]/1.211]

# create data
graficar(ys,xc1,yc1,xc162,yc162)
