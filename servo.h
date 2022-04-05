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

void setServo_d(PWM *ServoStruct, uint8_t move);
void setServo_f(PWM *ServoStruct, float move);
PWM enableServo(int8_t pin, int8_t starting_percent, int8_t min_off, int8_t max_off, uint8_t ignore_small, bool inverted);

#endif