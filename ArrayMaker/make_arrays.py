#%%
import numpy as _np
import matplotlib.pyplot as plt

ARRAYSIZE = 256

def list2cpparray(l):
    out = '{ '
    for i,e in enumerate(l):
        if i==len(l)-1:
            out +=  str(e)   
        else:
            out += str(e)+', '
    out += '}'
    return out

def save(arraysize, wavelist, namelist, filename):

    with open(filename,mode='w') as f:
        f.write('#pragma once\n')
        f.write('#define ARRAYSIZE '+str(arraysize)+'\n')

        for i, wave in enumerate(wavelist):
            f.write('static float '+namelist[i]+'[ARRAYSIZE]=')
            f.write(list2cpparray(wave))
            f.write(';\n')

if __name__ == '__main__':

    wavelist = []
    namelist = []
    x = 2*_np.arange(ARRAYSIZE)/float(ARRAYSIZE)-1

    wavelist.append(_np.zeros(ARRAYSIZE))
    namelist.append('saw')
    wavelist[0] = _np.roll(x,int(ARRAYSIZE/2))
    plt.plot(x,wavelist[0])
    # plt.show()
    
    wavelist.append(_np.zeros(ARRAYSIZE))
    namelist.append('square')
    wavelist[1] = _np.ones_like(x)*(x<0.5-0.5)
    plt.plot(x,wavelist[1])
    
    wavelist.append(_np.zeros(ARRAYSIZE))
    namelist.append('sine')
    wavelist[2] = -_np.sin(_np.pi*x)
    plt.plot(x,wavelist[2])

    wavelist.append(_np.zeros(ARRAYSIZE))
    namelist.append('tri')
    # wavelist[3] = -_np.sin(_np.pi*x)
    wavelist[3] = _np.roll(2*x,int(ARRAYSIZE/2))*(x<=-0.5) + (_np.roll(2*x,int(ARRAYSIZE/2)))*(x>=0.5) - 2*x*(abs(x)<0.5)
    plt.plot(x,wavelist[3])

    a = _np.loadtxt("D#1-velocity.csv")
    a[:,0]=a[:,0]/1000

    freq = 39.89
    dur = 1/freq
    xinterp = _np.linspace(0,dur,ARRAYSIZE)
    wavelist.append(_np.zeros(ARRAYSIZE))
    namelist.append('piano')    
    wavelist[4] = _np.interp(xinterp, a[:,0], a[:,1])
    wavelist[4] = wavelist[4]/max(wavelist[4])
    plt.plot(x,wavelist[4])
    plt.show()
    
    save(ARRAYSIZE,wavelist,namelist,'SampleArrays.h')


# %%
