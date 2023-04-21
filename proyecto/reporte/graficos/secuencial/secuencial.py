import time
import matplotlib.pyplot as plt
import numpy as np

def graficar(xs, ys):
    
    xs = np.array(xs)
    ys = np.array(ys)
        
    #plt.plot(x1, y1,lw=2, marker="o",linestyle='--')
    plt.plot(xs, ys, lw=2, marker="o",linestyle='--',color="#3336ff",label='secuencial')
        
    plt.title("SECUENCIAL")
    plt.xlabel("Número de ROBOTS")
    plt.ylabel("Tiempo de ejecución (minutos)")
    plt.grid()
    plt.xticks(xs)
    plt.yticks(ys)
    #plt.legend()
    plt.savefig('secuencial.png')
    plt.show()
    
xs = [32,64,128,256,512]
ys = [0.554,1.145,1.581,3.410,7.298]

# create data
graficar(xs, ys)

