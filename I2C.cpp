#include <iostream>

#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

#include "I2C.hpp"

I2C::I2C(int address)
    : m_address(address)
{
    //----- OPEN THE I2C BUS -----
    char *filename = (char *)"/dev/i2c-1";
    if ((m_file_i2c = open(filename, O_RDWR)) < 0)
    {
        //ERROR HANDLING: you can check errno to see what went wrong
        printf("Failed to open the i2c bus");
        throw -1;
    }

    std::cout << "opened i2c bus!" << std::endl;

    if (ioctl(m_file_i2c, I2C_SLAVE, m_address) < 0)
    {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        //ERROR HANDLING; you can check errno to see what went wrong
        throw -1;
    }

    std::cout << "acquired bus access and/or talk to slave!" << std::endl;
}

I2C::~I2C()
{
    std::cout << "closing file in destructor i2c!" << std::endl;
    close(m_file_i2c);
}
	
void I2C::sendData(const void * buffer, int length){

	if (write(m_file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
    else{
        // std::cout << "Written to the i2c bus.!" << std::endl;
    }
	
}

void I2C::readData(void * buffer, int length){
	// //----- READ BYTES -----
	if (read(m_file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
}
