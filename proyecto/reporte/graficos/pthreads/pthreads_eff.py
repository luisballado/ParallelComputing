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
ys = [0.554,1.145,1.580,3.410,7.290]

xc1 = [1,2,4,8,16,32]
yc1 = [1,0.772,0.4876,0.22,0.08,0.02]

xc2 = [1,2,4,8,16,32]
yc2 = [1,0.956,0.95,0.40,0.15,0.05]

xc4 = [1,2,4,8,16,32]
yc4 = [1,0.440,0.6350,0.42,0.19,0.07]

xc8 = [1,2,4,8,16,32]
yc8 = [0.999,0.998,0.6063,0.61,0.32,0.13]

xc16 = [1,2,4,8,16,32]
yc16 = [1,0.99,0.98,0.46,0.51,0.19]

# create data
graficar(ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)
