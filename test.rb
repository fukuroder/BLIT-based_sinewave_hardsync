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
n = (sample_rate/2/master_freq).truncate                        # Nyquist limit (round down)
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
