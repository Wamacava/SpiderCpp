#pragma once
#include "I2C.hpp"

// Class to controll PCA9685 device
// REGISTERS
#define MODE1_REG 0x00
#define FIRST_SERVO_ON_L 0x06
#define FIRST_SERVO_ON_H 0x07
#define FIRST_SERVO_OFF_L 0x08
#define FIRST_SERVO_OFF_H 0x09

#define PRE_SCALE_REG 0xFE


// BITS
#define SLEEP_BIT_POS 4

class ServoController
{
private:
    int m_i2c_slave_address;
    I2C m_i2c;
    int m_PCA9685_osc_clock = 25000000;

    // pwm pulses corresponding to -90 and 90 degrees
    int m_pwm_servo_min;
    int m_pwm_servo_max;
    int m_pwm_pulses_per_degree;

    char readRegister(int register_addr);

    /**
     * Write whole byte to the register
     **/
    bool writeRegister(int register_addr, char value); // verify that register changed


    /**
     * Modify one bit of register, pos should be 0-7 (position of bit)
     **/
    bool modifyBitInRegister(int register_addr, int pos, bool value); // verify that register changed


    

public:
    ServoController(int i2c_slave_address);
    ~ServoController();

    void changePWMFrequency(int frequency);

    void setSleepMode(bool on);
    
    void setServo(int servoId, int angle);
};
