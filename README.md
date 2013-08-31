### BLIT-based sinewave hardsync(vst3/VC++2012)

![Screenshot](https://raw.github.com/fukuroder/BLIT-based_sinewave_hardsync/master/screenshot.png)

- - -

### Algorithm
This wave is constructed with additive synthesis and BLIT synthesis.

![Figure1](https://raw.github.com/fukuroder/BLIT-based_sinewave_hardsync/master/figure1.png)

```ruby
# test.rb

# initialize...
sample_rate = 44100.0   # sample rate
leak = 0.995            # [0.99, 1.0)
                        # used for leaky integrator
slave = 1.2             # [1.0, 2.0]
                        # (slave_freq = master_freq * slave)
t = 0.5                 # current position
blit_sum = 0.0          # current value for BLIT section

# set frequency...
master_freq = 440.0                                             # master frequency
n = (sample_rate/2/master_freq).to_i                            # Nyquist limit (round down)
dt = master_freq/sample_rate                                    # delta t
b1 = -2*Math.sin(Math::PI*slave)/(Math::PI*(1+slave)*(1-slave)) # Fourier coefficient for sin(2*PI*1*t)
b2 = -4*Math.sin(Math::PI*slave)/(Math::PI*(2+slave)*(2-slave)) # Fourier coefficient for sin(2*PI*2*t)
b3 = -4*Math.sin(Math::PI*slave)                                # used for BLIT section

# for each sample...
1000.times do
    # output
    p b1*Math.sin(2*Math::PI*1*t) + b2*Math.sin(2*Math::PI*2*t) + b3*blit_sum
    
    # update position
    t += dt
    
    # update BLIT (dividing by zero is not considered!)
    blit_sum = leak*blit_sum \
             + Math.cos((n+3)*Math::PI*t)*Math.sin((n-3+1)*Math::PI*t)/Math.sin(Math::PI*t)*dt
end
```

- - -

### Implementation by Reaktor
* [BLIT-based sinewave hardsync](https://co.native-instruments.com/index.php?id=userlibrary&type=0&ulbr=1&plview=detail&patchid=13180)
