#include <iostream>
#include <math.h>       /* pow, round */
#include <unistd.h> /* sleep */

#include "ServoController.hpp"

ServoController::ServoController(int i2c_slave_address)
    : m_i2c_slave_address(i2c_slave_address)
    , m_i2c(I2C(m_i2c_slave_address))
    , m_pwm_servo_min(102)
    , m_pwm_servo_max(525)
{
    changePWMFrequency(50);
    m_pwm_pulses_per_degree = (m_pwm_servo_max - m_pwm_servo_min) / 180;
}

ServoController::~ServoController()
{

}

char ServoController::readRegister(int register_addr)
{
    // read register
    unsigned char buffer[2] = {0};
    buffer[0] = register_addr; // register number
    m_i2c.sendData(buffer, 1);
    m_i2c.readData(buffer, 1);
    return buffer[0];
}

/**
* Write whole byte to the register
**/
bool ServoController::writeRegister(int register_addr, char value)
{
    // verify that register changed
    unsigned char buffer[2] = {0};
    buffer[0] = register_addr; // register number
    buffer[1] = value; // register number
    m_i2c.sendData(buffer, 2);

    char result = readRegister(register_addr);

    return result == value;
}

/**
* Modify one bit of register, pos should be 0-7 (position of bit)
**/
bool ServoController::modifyBitInRegister(int register_addr, int pos, bool value)
{
    // verify that register changed
    char reg = readRegister(register_addr);
    // int binary_multiplier = pow(2, pos);
    printf("Register %x to change: %x\n", register_addr, reg);
    if(value){
        // set bit at pos
        reg |= 1 << pos;
    }
    else{
        // unset bit at pos
        reg &= ~(1 << pos);
    }

    printf("Register %x after change: %x\n", register_addr, reg);
    
    // return writeRegister(register_addr, reg);
    writeRegister(register_addr, reg);

    // Changing one bit in register can result in automatic changes to other
    // Therefore returning comparison of one bit only
    char result_reg = readRegister(register_addr);
    bool success = ((result_reg >> pos) & 1) == value;
    if(!success){
        printf("Failed to change bit %d of register %x\n", pos,  register_addr);
    }
    return success;
}


void ServoController::changePWMFrequency(int frequency /*Hz*/)
{
    setSleepMode(true);
    // Formula in documentation page 25
    int reg_val = round(m_PCA9685_osc_clock/4096/frequency) -1;
    writeRegister(PRE_SCALE_REG, (char)reg_val);

    setSleepMode(false);
}

void ServoController::setSleepMode(bool on)
{
    modifyBitInRegister(MODE1_REG, SLEEP_BIT_POS, on);
    sleep(1);
}

void ServoController::setServo(int servoId, int angle)
{
    if(angle < 0 || angle > 180 || servoId < 0 || servoId > 15){
        // invalid parameters
        printf("ERROR invalid servo parameters\n");
        return;
    }

    int max_angle = 180;
    int off_time = round(m_pwm_servo_min + (m_pwm_servo_max - m_pwm_servo_min) * angle / max_angle ); 


// TODO delete if
    if(servoId == 1 || servoId == 0 || servoId == 2){
    printf("### Servo: %d Angle: %d pulses: %d\n", servoId, angle,  off_time);
    }
    

    // ON start time of sevro will always be 0
    // Setting up only OFF time
    char lsb = off_time; // it will cut off most significant bytes 
    char msb = off_time >> 8;
    writeRegister(FIRST_SERVO_OFF_L + 4*servoId, lsb);
    writeRegister(FIRST_SERVO_OFF_H + 4*servoId, msb);

}
