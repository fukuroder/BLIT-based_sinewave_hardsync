### BLIT-based sinewave hardsync(vst3/VC++2019)

![Screenshot](https://raw.github.com/fukuroder/BLIT-based_sinewave_hardsync/master/screenshot.png)

- - -

### Algorithm
This waveform is constructed with additive synthesis and BLIT synthesis.

![Figure1](https://raw.github.com/fukuroder/BLIT-based_sinewave_hardsync/master/figure.gif)

```python
import math

# initialize...
sample_rate = 44100.0 # sample rate

leak = 0.995          # [0.99, 1.0) (used for leaky integrator)

slave = 1.2           # [1.0, 2.0] (slave_freq = master_freq * slave)

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
for _ in range(1000):
    # output
    print(b1*math.sin(t) + b2*math.sin(2*t) + b3*blit_sum)

    # update position
    t += dt

    # update BLIT (dividing by zero is not considered!)
    blit_sum = leak*blit_sum + (math.sin((n+0.5)*t)/math.sin(0.5*t)-1.0)*dt
```
