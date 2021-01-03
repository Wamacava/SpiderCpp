#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#include "Robot.hpp"

// TODO refactor with using this TCP socket wrapper here 

class RemoteControl
{
    int m_listeningSocket = -1;
    int m_clientSocket = -1;
    sockaddr_in m_hint; // TODO it shouldn't be a member I think
    sockaddr_in m_client;

    Robot m_robot;
    bool m_operate = true;
    std::string ERROR = "ERROR";

public:
    RemoteControl()
        : m_robot(Robot())
    {
    }

    ~RemoteControl()
    {
        //Close the socket
        close(m_clientSocket);
    }


    // this will be executed in a thread in main and stopped with stop()
    void controlRobot()
    {
        if (init() < 0)
        {
            std::cerr << "Can't create socket" << std::endl;
            throw 1;
        }

        m_operate == true;
        while (m_operate)
        {
            bool success = waitForConnection();
            while (success)
            {
                std::string command = readFromSocket();
                if (command == ERROR)
                    break;
                executeCommand(command);
            }
        }
    }

    void stop()
    {
        m_operate = false;
    }

private:
    void executeCommand(std::string command)
    {
        if (command == "f")
        {
            std::cout << "Moving forward!!!" << std::endl;
            m_robot.forward();
        }
        else if (command == "b")
        {
            std::cout << "Moving backward!!!" << std::endl;
            m_robot.backward();
        }
        else if (command == "l")
        {
            std::cout << "Moving left!!!" << std::endl;
            m_robot.backward();
        }
        else if (command == "r")
        {
            std::cout << "Moving right!!!" << std::endl;
            m_robot.backward();
        }
        else
        {
            std::cout << "Unknown command" << std::endl;
        }
    }

    int init()
    {
        // Create a socket

        m_listeningSocket = socket(AF_INET, SOCK_STREAM, 0); // arguments: address family(IPv4), type- socket stream, protocol 0 and will be determined by networking stack
        if (m_listeningSocket == -1)
        {
            std::cerr << "Can't create a socket!";
            return -1;
        }

        // Bind the socket to the IP/port
        // sockaddr_in hint;
        m_hint.sin_family = AF_INET;
        m_hint.sin_port = htons(54000);                  // htons: host to network short - flips bits depending if little endian or big endian (how sysytem stores number bigger than 255)
        inet_pton(AF_INET, "0.0.0.0", &m_hint.sin_addr); // internet command, inverts number to array of integers 127.0.0.1 converts to array

        if (bind(m_listeningSocket, (sockaddr *)&m_hint, sizeof(m_hint)) == -1) // bind hint to the port
        {
            std::cerr << "Can't bind to IP/port" << std::endl;
            return -2;
        }
        // Mark the socket for listening in
        if (listen(m_listeningSocket, SOMAXCONN) == -1)
        {
            std::cerr << "Can't listen!" << std::endl;
            return -3;
        }
        return 0;
    }

    bool waitForConnection()
    {
        

        // Accept a call
        // sockaddr_in client;
        socklen_t clientSize = sizeof(m_client);
        char host[NI_MAXHOST];
        char svc[NI_MAXSERV];

        m_clientSocket = accept(m_listeningSocket,
                                    (sockaddr *)&m_client,
                                    &clientSize); // acccept an incoming connection on listening socket
        if (m_clientSocket == -1)
        {
            std::cerr << "Problem with client connecting!" << std::endl;
            return false;
        }

        // Close the listening socket
        close(m_listeningSocket);

        memset(host, 0, NI_MAXHOST); //clean things up
        memset(svc, 0, NI_MAXSERV);

        int result = getnameinfo((sockaddr *)&m_client,
                                 sizeof(m_client),
                                 host,
                                 NI_MAXHOST,
                                 svc,
                                 NI_MAXSERV,
                                 0); // try to get client computer name, if fails, returns 0

        if (result)
        {
            std::cout << host << " connected on " << svc << std::endl;
        }
        else // if we don't get name of the host, we do things manualy
        {
            inet_ntop(AF_INET, &m_client.sin_addr, host, NI_MAXHOST); // oposite of inet_pton
            std::cout << host << " connected on " << ntohs(m_client.sin_port) << std::endl;
        }
        return true;
    }

    std::string readFromSocket()
    {
        char buf[4096];

        // Clear the buffer
        memset(buf, 0, 4096);

        // Wait for a message
        int bytesRecv = recv(m_clientSocket, buf, 4096, 0); // the socket is no longer the listening socket, it's a client socket
        if (bytesRecv == -1)
        {
            std::cerr << "There was a connection issue!" << std::endl;
            return "ERROR";
        }
        if (bytesRecv == 0)
        {
            std::cout << "The client disconnected" << std::endl;
            return ERROR;
        }
        // Display message
        std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
        return std::string(buf, 0, bytesRecv);
    }

    void writeToSocket()
    {
        char buf[4096];

        // Clear the buffer
        memset(buf, 0, 4096);
        buf[0] = 'a';
        int len = 2;
        // Send message
        send(m_clientSocket, buf, 2, 0); // +1 because we have to have 0 in the end
    }
};