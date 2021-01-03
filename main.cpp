#include <iostream>
#include "RemoteControl.hpp"

int main()
{

    std::cout << "Hello spider" << std::endl;

    // while(true)
    // {
    //     auto servoController = ServoController(0x40);
    //     servoController.setServo(15, 90);
    //     sleep(1);
    // }

    auto rc = RemoteControl();
    rc.controlRobot();

    std::cout << "Bye spider" << std::endl;

    return 0;
}
