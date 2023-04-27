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
    
    plt.title("MPI EFICIENCIA")
    plt.xlabel("Número de procesadores")
    plt.ylabel("Eficiencia")
    plt.grid()
    plt.legend()
    plt.savefig('mpi_eff.png')
    plt.show()
    
xs = [32,64,128,256,512]
ys = [0.088,0.39448,0.7850,1.54360,3.0618,6.35080]

xc1 = [1,2,4,8,16,32]
yc1 = [1,1.01,1.02,1.02,1.01,1.01]

xc2 = [1,2,4,8,16,32]
yc2 = [1.03,1.01,1.01,1.01,1.01,1.01]

xc4 = [1,2,4,8,16,32]
yc4 = [0.998,1.0,0.99,1,0.99,1]

xc8 = [1,2,4,8,16,32]
yc8 = [1.0,0.98,0.99,0.99,0.99,0.99]

xc16 = [1,2,4,8,16,32]
yc16 = [1,1.06,1.02,1.02,1.02,1.02]

# create data
graficar(ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)

