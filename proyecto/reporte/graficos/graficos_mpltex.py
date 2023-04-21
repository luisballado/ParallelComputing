import numpy as np
import matplotlib.pyplot as plt
import mpltex

@mpltex.acs_decorator
def my_plot(x,y):
    fig, ax = plt.subplots(1)
    linestyles = mpltex.linestyle_generator(hollow_styles=[])

    ax.plot(x,y, label="$ROBOTS$",**next(linestyles))

    ax.set_xlabel('$\# ROBOTS$')
    ax.set_ylabel('$tiempo(segundos)$')
    ax.legend(loc='best', ncol=2)
    fig.tight_layout(pad=0.1)
    fig.savefig('grafica_secuencial')

x = [1,32,64,128,256,512]
y = [5.293,33.254,61.456,118.14,221.005,449.889]

my_plot(x,y)

plt.close('all')
