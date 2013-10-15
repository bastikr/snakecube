def is_unique(l):
    for i in range(len(l)//2):
        if l[i]<l[-i-1]:
            return False
        elif l[i]>l[-i-1]:
            return True
    return True

def partitions(N, n_min, n_max):
    partition = []
    def f(s):
        for i in range(n_min, n_max+1):
            if s+i==N:
                partition.append(i)
                if is_unique(partition):
                    yield partition
                partition.pop()
                partition.pop()
                return
            elif s+i>N:
                partition.pop()
                return
            partition.append(i)
            for result in f(s+i):
                yield result
        if partition:
            partition.pop()
    for result in f(0):
        yield result

import subprocess

Sol = {}

import multiprocessing
pool = multiprocessing.Pool(processes=3)

def solve(partition):
    s = ",".join(str(i+1) for i in partition)
    P = subprocess.Popen(["./snakecube", s], stdout=subprocess.PIPE)
    P.wait()
    result = P.stdout.read()
    count = result.count("\n")
    if count in Sol:
        Sol[count] += 1
    else:
        Sol[count] = 1
    #print("Solutions: "+str(count))

c = 0
for p in partitions(26,1,2):
    c+=1
    solve(p)
    #print("Test: "+s)
    #if c==5000:
    #    break

pool.close()
pool.join()

i = 0
while Sol:
    c = Sol.pop(i,None)
    if c is not None:
        print "%s: %s" % (i,c)
    i += 1

