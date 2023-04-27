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
    
    plt.title("MPI-OPENMP")
    plt.xlabel("Número de hilos")
    plt.ylabel("Tiempo de ejecución (minutos)")
    plt.grid()
    plt.legend()
    plt.savefig('mpi_openmp.png')
    plt.show()
    
xs = [1,2,4,8,16,32]
ys = [0.088,0.39448,0.78500,1.54360,3.06180,6.35080]

#32 [32,64,128,256,512]
xc1 = [1,2,4,8,16,32]
yc1 = [0.394,0.196,0.134,0.076,0.032,0.013]

#64
xc2 = [1,2,4,8,16,32]
yc2 = [0.789,0.436,0.288,0.130,0.075,0.025]

#128
xc4 = [1,2,4,8,16,32]
yc4 = [1.573,0.835,0.769,0.442,0.186,0.049]

#256
xc8 = [1,2,4,8,16,32]
yc8 = [3.039,2.387,2.004,0.896,0.447,0.079]

#512
xc16 = [1,2,4,8,16,32]
yc16 = [6.074,4.995,4.202,3.401,1.609,0.243]
    
# create data
#graficar(xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)
graficar(xs, ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)

