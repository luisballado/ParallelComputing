import time
import matplotlib.pyplot as plt
import numpy as np

def graficar(xs, ys, xc1, yc1, xc2, yc2, xc4, yc4, xc8, yc8, xc16, yc16):
    
    xs = np.array(xs)
    ys = np.array(ys)

    xc1 = np.array(xc1)
    yc1 = np.array(yc1)

    xc2 = np.array(xc2)
    yc2 = np.array(yc2)

    xc4 = np.array(xc4)
    yc4 = np.array(yc4)

    xc8 = np.array(xc8)
    yc8 = np.array(yc8)

    xc16 = np.array(xc16)
    yc16 = np.array(yc16)
    
    #plt.plot(x1, y1,lw=2, marker="o",linestyle='--')
    #plt.plot(xs, ys, lw=2, marker="o",linestyle='--',color="#3336ff",label='secuencial')
    plt.plot(xc1, yc1 ,lw=2, marker="o",linestyle='--',color="#ff3333",label='32 ROBOTS')
    plt.plot(xc2, yc2, lw=2, marker="o",linestyle='--',color="#ffe933",label='64 ROBOTS')
    plt.plot(xc4, yc4, lw=2, marker="o",linestyle='--',color="#36ff33",label='128 ROBOTS')
    plt.plot(xc8, yc8, lw=2, marker="o",linestyle='--',color="#33fff3",label='256 ROBOTS')
    plt.plot(xc16, yc16, lw=2, marker="o",linestyle='--',color="#8333ff",label='512 ROBOTS')
    
    plt.xticks(xc16)
    plt.yticks(yc16)
    
    plt.title("OPENMP")
    plt.xlabel("Número de hilos")
    plt.ylabel("Tiempo de ejecución (minutos)")
    plt.grid()
    plt.legend()
    plt.savefig('openmp.png')
    plt.show()
    
xs = [1,2,4,8,16,32]
ys = [0.088,0.554,1.145,1.581,3.410,7.298]

#32 [32,64,128,256,512]
xc1 = [1,2,4,8,16,32]
yc1 = [0.554,0.347,0.216,0.144,0.108,0.100]

#64
xc2 = [1,2,4,8,16,32]
yc2 = [1.170,0.621,0.357,0.212,0.144,0.123]

#128
xc4 = [1,2,4,8,16,32]
yc4 = [2.244,1.652,0.631,0.353,0.227,0.175]

#256
xc8 = [1,2,4,8,16,32]
yc8 = [3.383,2.851,1.971,0.627,0.377,0.290]

#512
xc16 = [1,2,4,8,16,32]
yc16 = [7.500,4.231,2.200,1.962,0.673,0.510]
    
# create data
#graficar(xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)
graficar(xs, ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)

