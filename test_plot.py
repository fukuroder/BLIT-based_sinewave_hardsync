#------------------------------------------------------------
# test_plot.py
#
# Copyright (c) 2016, fukuroda (https://github.com/fukuroder)
# Released under the MIT license
#------------------------------------------------------------

# import
import math
import matplotlib.pyplot as plt

# initialize...
sample_rate = 44100.0 # sample rate

leak = 0.995          # [0.99, 1.0)
                      # used for leaky integrator

slave = 1.2           # [1.0, 2.0]
                      # (slave_freq = master_freq * slave)

# set frequency...
master_freq = 440.0                                               # master frequency

n = int(sample_rate/2/master_freq)                                # Nyquist limit
dt = 2*math.pi*master_freq/sample_rate                            # delta t
b1 = math.sin(math.pi*slave)/math.pi*(-1/(1+slave)-1/(1-slave)+2) # Fourier coefficient for sin(2*PI*1*t)
b2 = math.sin(math.pi*slave)/math.pi*(-1/(2+slave)-1/(2-slave)+1) # Fourier coefficient for sin(2*PI*2*t)
b3 = -math.sin(math.pi*slave)/math.pi                             # used for BLIT section

# for each sample...
t = -math.pi            # current position
blit_sum = 0.0          # current value for BLIT section
v1 = []
v2 = []
v3 = []
for _ in range(400):
    # output
    v1.append(b1*math.sin(t) + b2*math.sin(2*t))
    v2.append(b3*blit_sum)
    v3.append(b1*math.sin(t) + b2*math.sin(2*t) + b3*blit_sum)

    # update position
    t += dt

    # update BLIT (dividing by zero is not considered!)
    blit_sum = leak*blit_sum + (math.sin((n+0.5)*t)/math.sin(0.5*t)-1.0)*dt

# Additive synthesis section(k=1...2)
p1 = plt.subplot(311)
plt.plot(v1)
plt.ylim([-1.5,1.5])
plt.grid()
plt.setp(p1.get_xticklabels(), visible=False)
plt.setp(p1.get_yticklabels(), visible=False)
plt.title('Additive synthesis(k=1...2)', fontsize=10)

# BLIT synthesis section(k=3...N)
p2 = plt.subplot(312)
plt.plot(v2)
plt.ylim([-1.5,1.5])
plt.grid()
plt.setp(p2.get_xticklabels(), visible=False)
plt.setp(p2.get_yticklabels(), visible=False)
plt.title('BLIT_saw', fontsize=10)

# Additive + BLIT
p3 = plt.subplot(313)
plt.plot(v3)
plt.ylim([-1.5,1.5])
plt.grid()
plt.setp(p3.get_xticklabels(), visible=False)
plt.setp(p3.get_yticklabels(), visible=False)
plt.title('Additive + BLIT_saw', fontsize=10)

plt.show()
