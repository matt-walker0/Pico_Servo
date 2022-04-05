#ifndef servo_included
#define servo_included

typedef struct {
    uint8_t pin;        // gpio pin number
    uint8_t slice;      // ugh
    int16_t pulseMax;   // max pulse
    int16_t pulseMin;
    int16_t range;      // max pulse - min pulse
    int16_t channel;    
    int16_t output;     // Used as output to servo
    int16_t lvl_ignore; // Ignore changes in pulse width less than this.
} PWM;

// Set servo based upon values between 1 (min) and 255 (max)
void setServo_d(PWM *ServoStruct, uint8_t move);

// Set servo based upon values between -1(min) and +1(max)
void setServo_f(PWM *ServoStruct, float move);

/* Arguments: 
    - GPIO pin used for PWM.
    - Starting percentage (tends to be 50).
    - Min value offset. x% where + gives greater range, - gives reduced range.
    - Max value offset. x% where + gives greater range, - gives reduced range.
    - Ignore percentage, x% change that is ignored on a servo level
    - Inverted flips servo motion */
PWM enableServo(int8_t pwm_pin, int8_t starting_percent, int8_t min_percent_offset, int8_t max_percent_offset, uint8_t ignore_percent, bool inverted);

#endif