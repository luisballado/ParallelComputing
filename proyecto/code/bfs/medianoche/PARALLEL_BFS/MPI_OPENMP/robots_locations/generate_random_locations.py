import sys
import random

n = int(sys.argv[1])

#print("Numero de ubicaciones a generar: " + str(n))

#que el i sea la semilla
for i in range(n):
    random.seed(random.random())
    print(str(random.randint(0,n)) + " " + str(random.randint(0,1000000)))
