#include <iostream>
// #include "I2C.hpp"
#include "ServoController.hpp"
#include "Robot.hpp"

#include <unistd.h>				//Needed for I2C port
// #include <fcntl.h>				//Needed for I2C port
// #include <sys/ioctl.h>			//Needed for I2C port
// #include <linux/i2c-dev.h>		//Needed for I2C port



int main(){

    std::cout << "Hello spider" << std::endl;

    // auto i2c = I2C(0x40); // create controller connection
    // auto servoController = ServoController(0x40);
    auto robot = Robot();
    
// while(true){
    // servoController.setServo(15, 0);
    // sleep(1);
    // servoController.setServo(15, 45);
    // sleep(1);
    // servoController.setServo(15, 90);
    // sleep(1);
    // servoController.setServo(15, 135);
    // sleep(1);
    // servoController.setServo(15, 90);


    printf("\n\n\n!!!@@@### BEFORE FIRST FORWARD\n");

    robot.forward();
    sleep(2);

    printf("\n\n\n!!!@@@### AFTER FIRST FORWARD\n");

    // robot.forward();
    // sleep(2);

    // printf("\n\n\n!!!@@@### AFTER SECOND FORWARD\n");

    // robot.backward();
    // sleep(2);

    // printf("\n\n\n!!!@@@### AFTER BACKWARD\n");
// }

    return 0;
}

