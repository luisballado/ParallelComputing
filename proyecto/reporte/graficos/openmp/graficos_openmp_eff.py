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
    
    plt.title("OPENMP EFICIENCIA")
    plt.xlabel("Número de procesadores")
    plt.ylabel("Eficiencia")
    plt.grid()
    plt.legend()
    plt.savefig('openmp_eff.png')
    plt.show()
    
xs = [32,64,128,256,512]
ys = [0.554,1.024,1.969,3.683,7.498]

xc1 = [1,2,4,8,16,32]
yc1 = [ys[0]/(0.554*1),ys[0]/(0.347*2),ys[0]/(0.216*4),ys[0]/(0.144*8),ys[0]/(0.108*16),ys[0]/(0.100*32)]

xc2 = [1,2,4,8,16,32]
yc2 = [ys[1]/(1.170*1),ys[1]/(0.621*2),ys[1]/(0.357*4),ys[1]/(0.212*8),ys[1]/(0.144*16),ys[1]/(0.123*32)]

xc4 = [1,2,4,8,16,32]
yc4 = [ys[2]/(2.244*1),ys[2]/(1.652*2),ys[2]/(0.631*4),ys[2]/(0.353*8),ys[2]/(0.227*16),ys[2]/(0.175*32)]

xc8 = [1,2,4,8,16,32]
yc8 = [ys[3]/(3.383*1),ys[3]/(2.851*2),ys[3]/(1.971*4),ys[3]/(0.627*8),ys[3]/(0.377*16),ys[3]/(0.290*32)]

xc16 = [1,2,4,8,16,32]
yc16 = [ys[4]/(7.500*1),ys[4]/(4.231*2),ys[4]/(2.200*4),ys[4]/(1.962*8),ys[4]/(0.673*16),ys[4]/(0.510*32)]

# create data
graficar(ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)

