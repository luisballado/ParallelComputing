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
    
    plt.title("MPI")
    plt.xlabel("Número de hilos")
    plt.ylabel("Tiempo de ejecución (minutos)")
    plt.grid()
    plt.legend()
    plt.savefig('mpi.png')
    plt.show()
    
xs = [1,2,4,8,16,32]
ys = [0.088,0.39448,0.7850,1.54360,3.0618,6.35080]

#32 [32,64,128,256,512]
xc1 = [1,2,4,8,16,32]
yc1 = [0.39,0.19,0.097027,0.048539,0.024299,0.012179]

#64
xc2 = [1,2,4,8,16,32]
yc2 = [0.76,0.39,0.19388,0.09708,0.048742,0.02429]

#128
xc4 = [1,2,4,8,16,32]
yc4 = [1.55,0.77,0.388,0.19244,0.0969,0.0484]

#256
xc8 = [1,2,4,8,16,32]
yc8 = [3.01,1.56,0.775,0.38784,0.19399,0.0970]

#512
xc16 = [1,2,4,8,16,32]
yc16 = [5.95,2.99,1.556,0.778365,0.3888,0.194028]
    
# create data
#graficar(xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)
graficar(xs, ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)

