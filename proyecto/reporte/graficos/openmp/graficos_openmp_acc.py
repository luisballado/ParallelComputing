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
    
    plt.title("OPENMP ACELERACIÓN")
    plt.xlabel("Número de procesadores")
    plt.ylabel("Aceleración")
    plt.grid()
    plt.legend()
    plt.savefig('openmp_acc.png')
    plt.show()
    
xs = [32,64,128,256,512]
ys = [0.554,1.145,1.581,3.410,7.298]

xc1 = [1,2,4,8,16,32]
yc1 = [ys[0]/0.554,ys[0]/0.347,ys[0]/0.216,ys[0]/0.144,ys[0]/0.108,ys[0]/0.100]

xc2 = [1,2,4,8,16,32]
yc2 = [ys[1]/1.170,ys[1]/0.621,ys[1]/0.357,ys[1]/0.212,ys[1]/0.144,ys[1]/0.123]

xc4 = [1,2,4,8,16,32]
yc4 = [ys[2]/2.244,ys[2]/1.652,ys[2]/0.631,ys[2]/0.353,ys[2]/0.227,ys[2]/0.175]

xc8 = [1,2,4,8,16,32]
yc8 = [ys[3]/3.383,ys[3]/2.851,ys[3]/1.971,ys[3]/0.627,ys[3]/0.377,ys[3]/0.290]

xc16 = [1,2,4,8,16,32]
yc16 = [ys[4]/7.500,ys[4]/4.231,ys[4]/2.200,ys[4]/1.962,ys[4]/0.673,ys[4]/0.510]

# create data
graficar(ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)

