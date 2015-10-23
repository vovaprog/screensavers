FOLDER = "./fractals"
WIDTH=1680
HEIGHT=1050
ITERATIONS=50000000
FRACTAL_FLAME_PROGRAM="./build/fractal_flame"

#=======================================================================================

from os import listdir
from os.path import isfile, join
from multiprocessing import Pool as ThreadPool,cpu_count
import subprocess
import os
import time

get_millis = lambda: int(round(time.time() * 1000))

NUMBER_OF_WORKERS =  cpu_count()

def render(files,worker_number):    
    for i in range(worker_number,len(files),NUMBER_OF_WORKERS):
        print str.format("worker: {0}   file: {1}",str(worker_number),files[i])
        os.system(str.format("{0} --mode=render --input-file={1} --width={2} --height={3} --iterations={4}",FRACTAL_FLAME_PROGRAM,files[i],WIDTH,HEIGHT,ITERATIONS))


millis=get_millis()


files = [ join(FOLDER,f) for f in listdir(FOLDER) if isfile(join(FOLDER,f)) and f.endswith(".xml") ]
files.sort()


pool = ThreadPool(NUMBER_OF_WORKERS) 

for i in range(NUMBER_OF_WORKERS):
    pool.apply_async(render,args=(files,i))

pool.close()
pool.join()


print "total time millis: "+str(get_millis()-millis)
