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
        
    plt.title("PTHREADS")
    plt.xlabel("Número de hilos")
    plt.ylabel("Tiempo de ejecución (minutos)")
    plt.xticks(xc16)
    
    plt.grid()
    plt.legend()
    plt.savefig('pthreads.png')
    plt.show()
    
xs = [1,2,4,8,16,32]
ys = [0.088,0.554,1.145,1.581,3.410,7.298]

xc1 = [1,2,4,8,16,32]
yc1 = [0.554,0.359,0.284,0.322,0.422,0.699]

xc2 = [1,2,4,8,16,32]
yc2 = [1.183,0.599,0.397,0.360,0.466,0.790] 

xc4 = [1,2,4,8,16,32]
yc4 = [1.530,1.796,0.622,0.471,0.521,0.739]

xc8 = [1,2,4,8,16,32]
yc8 = [3.415,1.549,1.406,0.704,0.657,0.816]

xc16 = [1,2,4,8,16,32]
yc16 = [7.119,3.527,1.582,1.960,0.902,1.211]
        
# create data
graficar(xs,ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)

