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

    plt.plot(xc1, yc1 ,lw=2, marker="o",linestyle='--',color="#ff3333",label='32 Robots')
    plt.plot(xc2, yc2, lw=2, marker="o",linestyle='--',color="#ffe933",label='64 Robots')
    plt.plot(xc4, yc4, lw=2, marker="o",linestyle='--',color="#36ff33",label='128 Robots')
    plt.plot(xc8, yc8, lw=2, marker="o",linestyle='--',color="#33fff3",label='256 Robots')
    plt.plot(xc16, yc16, lw=2, marker="o",linestyle='--',color="#8333ff",label='512 Robots')
    

    plt.title("PTHREADS ACELERACIÓN")
    plt.xlabel("Número de procesadores")
    plt.ylabel("Aceleración")
    plt.grid()
    plt.legend()
    plt.savefig('pth_acc.png')
    plt.show()
    
xs = [32,64,128,256,512]
ys = [0.554,1.145,1.580,3.410,7.290]

xc1 = [1,2,4,8,16,32]
yc1 = [ys[0]/0.554,ys[0]/0.359,ys[0]/0.284,ys[0]/0.322,ys[0]/0.422,ys[0]/0.699]

xc2 = [1,2,4,8,16,32]
yc2 = [ys[1]/1.118,ys[1]/0.599,ys[1]/0.238,ys[1]/0.360,ys[1]/0.466,ys[1]/0.790]

xc4 = [1,2,4,8,16,32]
yc4 = [ys[2]/1.530,ys[2]/1.796,ys[2]/0.622,ys[2]/0.471,ys[2]/0.521,ys[2]/0.739]

xc8 = [1,2,4,8,16,32]
yc8 = [ys[3]/3.415,ys[3]/1.549,ys[3]/1.406,ys[3]/0.704,ys[3]/0.657,ys[3]/0.816]

xc16 = [1,2,4,8,16,32]
yc16 = [ys[4]/7.119,ys[4]/3.527,ys[4]/1.582,ys[4]/1.960,ys[4]/0.902,ys[4]/1.211]

# create data
graficar(ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)


