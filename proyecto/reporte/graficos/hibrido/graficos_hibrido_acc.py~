import time
import matplotlib.pyplot as plt
import numpy as np

def graficar(ys, xc1, yc1, xc2, yc2, xc4, yc4, xc8, yc8, xc16, yc16):

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
        
    plt.plot(xc1, yc1 ,lw=2, marker="o",linestyle='--',color="#ff3333",label='32 ROBOTS')
    plt.plot(xc2, yc2, lw=2, marker="o",linestyle='--',color="#ffe933",label='64 ROBOTS')
    plt.plot(xc4, yc4, lw=2, marker="o",linestyle='--',color="#36ff33",label='128 ROBOTS')
    plt.plot(xc8, yc8, lw=2, marker="o",linestyle='--',color="#33fff3",label='256 ROBOTS')
    plt.plot(xc16, yc16, lw=2, marker="o",linestyle='--',color="#8333ff",label='512 ROBOTS')
    
    plt.title("MPI OPENMP ACELERACIÓN")
    plt.xlabel("Número de procesadores")
    plt.ylabel("Aceleración")
    plt.grid()
    plt.legend()
    plt.savefig('mpi_openmp_acc.png')
    plt.show()

xs = [1,2,4,8,16,32]
ys = [0.088,0.39448,0.78500,1.54360,3.06180,6.35080]

xc1 = [1,2,4,8,16,32]
yc1 = [1,2.016,2.935,5.195,12.449,31.020]

xc2 = [1,2,4,8,16,32]
yc2 = [0.995,1.8,2.727,6.060,10.517,31.145]

xc4 = [1,2,4,8,16,32]
yc4 = [0.981,1.848,2.006,3.495,8.284,31.385]

xc8 = [1,2,4,8,16,32]
yc8 = [1.008,1.282,1.528,3.418,6.850,38.720]

xc16 = [1,2,4,8,16,32]
yc16 = [1.045,1.272,1.511,1.867,3.947,26.141]

# create data
graficar(ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)

