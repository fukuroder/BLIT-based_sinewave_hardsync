### BLIT-based sinewave hardsync(Xcode/VisualStdio2015)

![Screenshot](https://raw.github.com/fukuroder/BLIT-based_sinewave_hardsync/master/screenshot.png)

- - -

### Algorithm
This waveform is constructed with additive synthesis and BLIT synthesis.

![Figure1](https://raw.github.com/fukuroder/BLIT-based_sinewave_hardsync/master/figure1.png)

```python
# test.py

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

n = int(sample_rate/2/master_freq)                            # Nyquist limit (round down)
dt = master_freq/sample_rate                                  # delta t
b1 = -2*math.sin(math.pi*slave)/(math.pi*(1+slave)*(1-slave)) # Fourier coefficient for sin(2*PI*1*t)
b2 = -4*math.sin(math.pi*slave)/(math.pi*(2+slave)*(2-slave)) # Fourier coefficient for sin(2*PI*2*t)
b3 = -4*math.sin(math.pi*slave)                               # used for BLIT section

# for each sample...
t = 0.5                 # current position
blit_sum = 0.0          # current value for BLIT section
for i in range(1000):
    # output
    print b1*math.sin(2*math.pi*1*t) + b2*math.sin(2*math.pi*2*t) + b3*blit_sum

    # update position
    t += dt

    # update BLIT
    blit_sum = leak*blit_sum \
        + math.cos((n+3)*math.pi*t)*math.sin((n-3+1)*math.pi*t)/math.sin(math.pi*t)*dt
```

- - -

### Implementation by Reaktor
* [BLIT-based sinewave hardsync](http://www.native-instruments.com/jp/community/reaktor-user-library/entry/show/7639/)
