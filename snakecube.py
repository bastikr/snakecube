import itertools
import numpy

COUNTER = 0

conf = (3,2,3,2,2,4,2,3,2,3,2,3,2,2,2,2,2,2,2,2,3,3,2,2,2,2,2,3,4,2,2,2,4,2,3,2,2,2,2,2,2,2,2,2,4,2)
def check_conf(conf):
    for i in conf:
        assert 1<i<=4
    assert sum(conf)-len(conf)+1 == 4**3

check_conf(conf)

D = (1,2,3)
D1 = (2,-2,3,-3)
D2 = (1,-1,3,-3)
D3 = (1,-1,2,-2)

def perpendicular_directions(direction):
    d = abs(direction)
    if d==1:
        return D1
    elif d==2:
        return D2
    elif d==3:
        return D3

def show_solution(conf, directions):
    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    p = numpy.array([0,0,0])
    points = []
    points.append(p.copy())
    for l,d in itertools.izip(conf, directions):
        index = abs(d)-1
        a = d/abs(d)
        p[index] += a*(l-1)
        points.append(p.copy())
    points = numpy.array(points)
    ax.plot(points[:,0], points[:,1], points[:,2])
    plt.show()




def is_valid(conf, directions):
    minima = numpy.array([0,0,0])
    maxima = numpy.array([0,0,0])
    p = numpy.array([0,0,0])
    for l,d in itertools.izip(conf, directions):
        index = abs(d)-1
        a = d/abs(d)
        p[index] += a*(l-1)
        if a>0:
            maxima[index] = max(maxima[index], p[index])
        else:
            minima[index] = min(minima[index], p[index])
    if numpy.any((maxima-minima)>3):
        return False
    p = -minima
    cube = numpy.zeros((4,4,4), dtype=bool)
    cube[tuple(p)] = True
    for l,d in itertools.izip(conf, directions):
        index = abs(d)-1
        for i in range(1,l):
            p[index] += d/abs(d)
            if cube[tuple(p)]:
                return False
            cube[tuple(p)] = True
    return True


def construct_step(conf, directions):
    global COUNTER
    if len(directions)<11:
        print("%s/%s" % (len(directions), len(conf)),
            COUNTER/float(4**(len(conf))))
    if len(directions)==10:
        print(directions)
    if not is_valid(conf, directions):
        COUNTER += 4**(len(conf)-len(directions))
        return set()
    if len(conf)==len(directions):
        return set((directions,))
    S = set()
    for d in perpendicular_directions(directions[-1]):
        S |= construct_step(conf, directions+(d,))
    return S

#S = construct_step(conf, (1,2))
#print("finished")
#print(S)
solutions = (
(1,2,-1,2,1,3,-1,-3,-2,3,1,-3,1,2,-3,1,3,-2,-3,-2,3,2,-1,-2,3,2,1,-2,-1,-3,-2,3,1,-3,-1,2,1,-3,-2,-1,2,-1,-2,-3,1,3,),
(1,2,-1,2,1,3,-1,-3,-2,3,1,-3,1,2,-3,1,3,-2,-3,-2,3,2,-1,-2,3,2,1,-2,-1,-3,-2,3,1,-3,-1,-3,1,2,3,-1,-3,-1,-2,-3,1,3,),
(1,2,-1,2,1,3,-1,-3,-2,3,1,-3,-2,-1,3,1,3,1,-3,-2,-1,-3,1,3,1,-3,1,2,3,-1,2,1,-3,-1,3,-2,-3,-2,1,-2,3,2,3,-2,-1,2,),
(1,2,-1,2,1,-3,-1,3,-2,-3,1,3,1,2,3,1,-3,-2,3,-2,-3,2,-1,-2,-3,2,1,-2,-1,3,-2,-3,1,3,-1,2,1,3,-2,-1,2,-1,-2,3,1,-3,),
(1,2,-1,2,1,-3,-1,3,-2,-3,1,3,1,2,3,1,-3,-2,3,-2,-3,2,-1,-2,-3,2,1,-2,-1,3,-2,-3,1,3,-1,3,1,2,-3,-1,3,-1,-2,3,1,-3,),
(1,2,-1,2,1,-3,-1,3,-2,-3,1,3,-2,-1,-3,1,-3,1,3,-2,-1,3,1,-3,1,3,1,2,-3,-1,2,1,3,-1,-3,-2,3,-2,1,-2,-3,2,-3,-2,-1,2,),
(1,2,-1,3,-2,1,-3,2,-1,3,-1,-3,-1,2,1,3,1,-3,1,3,-2,-1,3,-1,2,-3,2,3,-2,-3,1,3,2,-3,1,-2,3,-2,-3,-1,-2,1,3,-1,2,1,),
(1,2,-1,-3,-2,1,3,2,-1,-3,-1,3,-1,2,1,-3,1,3,1,-3,-2,-1,-3,-1,2,3,2,-3,-2,3,1,-3,2,3,1,-2,-3,-2,3,-1,-2,1,-3,-1,2,1,),
)

show_solution(conf, sol3)
