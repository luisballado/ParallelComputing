"""
SE COMPARA LA IMAGEN DEL MISMO TAMANIO
"""

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
        
    plt.title("EFICIENCIA 32 ROBOTS")
    plt.xlabel("Número de procesadores")
    plt.ylabel("Eficiencia")
    plt.grid()
    plt.legend()
    plt.savefig('eficiencia_32.png')
    plt.show()

xs = [32,64,128,256,512]
ys = [0.554,1.024,1.969,3.683,7.498]

#openmp 32 ROBOTS
xc1 = [1,2,4,8,16,32]
yc1 = [ys[0]/(0.554*1),ys[0]/(0.347*2),ys[0]/(0.216*4),ys[0]/(0.144*8),ys[0]/(0.108*16),ys[0]/(0.100*32)]

#pthreads 32 ROBOTS
xc162 = [1,2,4,8,16,32]
yc162 = [ys[0]/(0.554*1),ys[0]/(0.359*2),ys[0]/(0.284*4),ys[0]/(0.322*8),ys[0]/(0.422*16),ys[0]/(0.699*32)]

# create data
graficar(ys,xc1,yc1,xc162,yc162)
