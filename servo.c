#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "servo.h"


/*
~~~ Background Information ~~~
- 9g servos tend to use 1000us - 2000us pulses at 50Hz. (https://raspberrypi.stackexchange.com/questions/106858/what-is-the-proper-calculation-of-duty-cycle-range-for-the-sg90-servo)
- ESCs tend to use same as above.
- Much of below is borrowed from here: https://www.raspberrypi.org/forums/viewtopic.php?t=305513

*/


// Set servo based upon values between 1 (min) and 255 (max)
void setServo_d(PWM *ServoStruct, uint8_t move){
    uint16_t prev_output = ServoStruct->output; 

    // Go from 1-255 -> Servo x's min-max
    uint16_t new_output = ServoStruct->pulseMin + (int16_t) ((((float) move-1)/254.f) * (float)ServoStruct->range);

    if(abs(new_output - prev_output) < ServoStruct->lvl_ignore) {
        return; // don't change servo output
    }
    
    ServoStruct->output = new_output;
    //Set the pwm level. Value is duty percentage (20,000*0.1 = 2,000)
    pwm_set_gpio_level(ServoStruct->pin, ServoStruct->output);
}


// Set servo based upon values between -1(min) and +1(max)
void setServo_f(PWM *ServoStruct, float move){
    uint16_t prev_output = ServoStruct->output; 

    // Go from +-1 -> Servo x's min-max
    uint16_t new_output = ServoStruct->pulseMin + ((move+1)/2.f) * (float) ServoStruct->range;
    
    if(abs(new_output - prev_output) < ServoStruct->lvl_ignore) {
        return; // don't change servo output
    }

    ServoStruct->output = new_output;
    //Set the pwm level. Value is duty percentage (20,000*0.1 = 2,000)
    pwm_set_gpio_level(ServoStruct->pin, ServoStruct->output);
}


// GPIO pin, offsets (%, + gives greater range, - gives reduced range), inverted (false normally)
PWM enableServo(int8_t pwm_pin, int8_t starting_percent, int8_t min_percent, int8_t max_percent, uint8_t ignore_percent, bool inverted) {
    PWM pwm;

    if(inverted == false) {
        pwm.pulseMin = (int16_t) (1000 - (10 * min_percent)); // *10 to convert percentage inc/dec into pulse width.
        pwm.pulseMax = (int16_t) (2000 + (10 * max_percent));
    } else {
        pwm.pulseMin = (int16_t) (2000 + (10 * max_percent));
        pwm.pulseMax = (int16_t) (1000 - (10 * min_percent)); 
    }
    pwm.range = pwm.pulseMax - pwm.pulseMin;
    pwm.output = pwm.pulseMin + ((pwm.range/100) * starting_percent);
    pwm.pin = pwm_pin;
    pwm.lvl_ignore = (pwm.range/100) * ignore_percent; // ignore small, 0.1x% of range

    // Turn the pin into a pwm pin
    gpio_set_function(pwm.pin, GPIO_FUNC_PWM);
    // Need to store the slice and channel
    pwm.slice = pwm_gpio_to_slice_num(pwm_pin);
    pwm.channel = pwm_gpio_to_channel(pwm_pin);
    // Set the clock to equal 1 us per cycle instead of 8ns 125MHz/125(=1us) (lowering frequency)
    pwm_set_clkdiv(pwm.slice,125.0f);  
    // Wrap every 20000 cycles. 50Hz cycle in microsec / standard cycle in microsec (1us) = 20000/1
    pwm_set_wrap(pwm.slice,20000); 
    // Enable and set level     
    pwm_set_enabled(pwm.slice,true);
    pwm_set_gpio_level(pwm.pin, pwm.output);

    return pwm;
}
