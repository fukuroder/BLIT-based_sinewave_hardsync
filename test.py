# -*- coding: utf-8 -*-

#------------------------------------------------------------
# test.py
#
# Copyright (c) 2016, fukuroda (https://github.com/fukuroder)
# Released under the MIT license
#------------------------------------------------------------

# import
import math

# initialize...
sample_rate = 44100.0 # sample rate

leak = 0.995          # [0.99, 1.0)
                      # used for leaky integrator

slave = 1.2           # [1.0, 2.0]
                      # (slave_freq = master_freq * slave)

# set frequency...
master_freq = 440.0                                           # master frequency

n = int(sample_rate/2/master_freq)                            # Nyquist limit
dt = 2*math.pi*master_freq/sample_rate                        # delta t
b1 = -2*math.sin(math.pi*slave)/(math.pi*(1+slave)*(1-slave)) # Fourier coefficient for sin(2*PI*1*t)
b2 = -4*math.sin(math.pi*slave)/(math.pi*(2+slave)*(2-slave)) # Fourier coefficient for sin(2*PI*2*t)
b3 = -2*math.sin(math.pi*slave)/math.pi                       # used for BLIT section

# for each sample...
t = -math.pi            # current position
blit_sum = 0.0          # current value for BLIT section
for i in range(1000):
    # output
    print(b1*math.sin(t) + b2*math.sin(2*t) + b3*blit_sum)

    # update position
    t += dt

    # update BLIT (dividing by zero is not considered!)
    blit_sum = leak*blit_sum \
             + math.cos((n+3)*0.5*t)*math.sin((n-3+1)*0.5*t)/math.sin(0.5*t)*dt
