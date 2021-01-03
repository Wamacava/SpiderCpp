#pragma once

class I2C
{
private:
    int m_address;
    int m_file_i2c;

public:
    I2C(int address);
    ~I2C();

    void sendData(const void * buffer, int length);

    void readData(void * buffer, int length);
};
