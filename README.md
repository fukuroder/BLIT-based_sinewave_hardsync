### BLIT-based sinewave hardsync(vst3/VC++2012)

![Screenshot](https://raw.github.com/fukuroder/BLIT-based_sinewave_hardsync/master/screenshot.png)

- - -

### Algorithm
This wave is constructed with additive synthesis and BLIT synthesis.

![Figure1](https://raw.github.com/fukuroder/BLIT-based_sinewave_hardsync/master/figure1.png)

```
# initialize...
    sample_rate = 44100     # sample rate
    leak = 0.995            # [0.99, 1.0)
                            # used for leaky integrator
    slave = 1.2             # [1.0, 2.0]
                            # (slave_freq = master_freq * slave)
    master_freq = 440       # master frequency
    t = 0                   # current position
    blit_sum = 0            # current value for BLIT section

# set frequency...
    n = sample_rate/2/master_freq                   # Nyquist limit (round down)
    dt = master_freq/sample_rate                    # delta t
    b1 =  2*sin(PI*slave)/(PI*(1+slave)*(1-slave))  # Fourier coefficient for sin(2*PI*1*t)
    b2 = -4*sin(PI*slave)/(PI*(2+slave)*(2-slave))  # Fourier coefficient for sin(2*PI*2*t)
    b3 = -4*sin(PI*slave)                           # used for BLIT section

# for each sample...
    t += dt
    blit_sum = leak*blit_sum
             + cos((n+3)*PI*(t+0.5))*sin((n-3+1)*PI*(t+0.5))/sin(PI*(t+0.5))*dt
    output = b1*sin(2*PI*1*t) + b2*sin(2*PI*2*t) + b3*blit_sum
```

- - -

### Implementation by Reaktor
* [BLIT-based sinewave hardsync](https://co.native-instruments.com/index.php?id=userlibrary&type=0&ulbr=1&plview=detail&patchid=13180)
