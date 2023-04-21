import time
import matplotlib.pyplot as plt
import numpy as np

def graficar(ys,xc1, yc1,
             xc2, yc2,
             xc4, yc4,
             xc8, yc8,
             xc16, yc16):
    
    pxc1 = np.array(xc1)
    pyc1 = np.array(yc1)

    pxc2 = np.array(xc2)
    pyc2 = np.array(yc2)

    pxc4 = np.array(xc4)
    pyc4 = np.array(yc4)

    pxc8 = np.array(xc8)
    pyc8 = np.array(yc8)

    pxc16 = np.array(xc16)
    pyc16 = np.array(yc16)
    
    plt.plot(pxc1, pyc1 ,lw=2, marker="o",linestyle='--',color="#ff3333",label='32 ROBOTS')
    plt.plot(pxc2, pyc2, lw=2, marker="o",linestyle='--',color="#ffe933",label='64 ROBOTS')
    plt.plot(pxc4, pyc4, lw=2, marker="o",linestyle='--',color="#36ff33",label='128 ROBOTS')
    plt.plot(pxc8, pyc8, lw=2, marker="o",linestyle='--',color="#33fff3",label='256 ROBOTS')
    plt.plot(pxc16, pyc16, lw=2, marker="o",linestyle='--',color="#8333ff",label='512 ROBOTS')
        
    plt.title("PTHREADS EFICIENCIA")
    plt.xlabel("Número de procesadores")
    plt.ylabel("Eficiencia")
    plt.grid()
    plt.legend()
    plt.savefig('pthreads_eff.png')
    plt.show()

xs = [32,64,128,256,512]
ys = [0.554,1.024,1.969,3.683,7.498]

xc1 = [1,2,4,8,16,32]
yc1 = [ys[0]/(0.554*1),ys[0]/(0.359*2),ys[0]/(0.284*4),ys[0]/(0.322*8),ys[0]/(0.422*16),ys[0]/(0.699*32)]

xc2 = [1,2,4,8,16,32]
yc2 = [ys[1]/(1.183*1),ys[1]/(0.599*2),ys[1]/(0.397*4),ys[1]/(0.360*8),ys[1]/(0.466*16),ys[1]/(0.790*32)]

xc4 = [1,2,4,8,16,32]
yc4 = [ys[2]/(1.530*1),ys[2]/(1.796*2),ys[2]/(0.622*4),ys[2]/(0.471*8),ys[2]/(0.521*16),ys[2]/(0.739*32)]

xc8 = [1,2,4,8,16,32]
yc8 = [ys[3]/(3.415*1),ys[3]/(1.549*2),ys[3]/(1.406*4),ys[3]/(0.704*8),ys[3]/(0.657*16),ys[3]/(0.816*32)]

xc162 = [1,2,4,8,16,32]
yc162 = [ys[4]/(7.119*1),ys[4]/(3.527*2),ys[4]/(1.582*4),ys[4]/(1.960*8),ys[4]/(0.902*16),ys[4]/(1.211*32)]

# create data
graficar(ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc162,yc162)
