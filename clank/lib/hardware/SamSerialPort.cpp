#include "SamSerialPort.hpp"

using namespace std;
using namespace Clank::Hardware;

SamSerialPort* SamSerialPort::SamSerialPortSingletonInstance = NULL;

void* SamSerialPortReceiveFxn(void* arg)
{
  SamSerialPort* port = (SamSerialPort*)arg;
  char recv_buf[BUF_SIZE];
  int serialPortFd = port->GetSerialPortFd();

  for (int i = 0; i < BUF_SIZE; i++)
    recv_buf[i] = 0;

  while(port->IsListening())
  {
    int bytesRead;
    if ( (bytesRead = read(serialPortFd, recv_buf, BUF_SIZE-1)) > 0) //blocks
    {
      recv_buf[bytesRead] = '\0';
      port->ParseMessage(std::string(recv_buf));
    }
  }
  return NULL;
}

bool SamSerialPort::IsListening()
{
  pthread_mutex_lock(&serialPortThreadMutex);
  bool returnValue = this->isListening;
  pthread_mutex_unlock(&serialPortThreadMutex);
  return returnValue;
}

void SamSerialPort::ParseMessage(std::string message)
{
  this->SamPeripheralManager->RouteMessage(message); 
}

SamSerialPort* SamSerialPort::Instance()
{
  if (SamSerialPortSingletonInstance == NULL)
  {
    SamSerialPortSingletonInstance = new SamSerialPort();
  }
  return SamSerialPortSingletonInstance;
}

int SamSerialPort::GetSerialPortFd()
{
  return this->SerialPortFd;
}

SamSerialPort::SamSerialPort()
{
  this->SamPeripheralManager = SamPeripheralManager::Instance();

  /*Copied from http://www.udoo.org/tutorial/interaction-between-linux-and-arduino-on-udoo/ */
  system("stty -F /dev/ttymxc3 cs8 115200 icanon");
  
  this->SerialPortFd = open("/dev/ttymxc3", O_RDWR | O_NOCTTY);
  if (this->SerialPortFd < 0)
    throw runtime_error("SamSerialPort exception: Unable to open /dev/ttymxc3.");

  /*Verify that the daemon is listening*/
  bool isListening = false;
  for (int i = 0; i < 30; i++)
  {
    char response[256];
    write(this->SerialPortFd, "Ping\n", 5);
    usleep(10000);
    read(this->SerialPortFd, response, 256);
    if (!strncmp(response, "Pong", 4))
    {
      isListening = true;
      break;
    }
    usleep(10000);
  }

  if (!isListening)
  {
    throw runtime_error("SamSerialPort exception: the SAM hardware daemon could not be reached.");
  }

  /*Attempt to reset the device*/
  char response2[256];
  write(this->SerialPortFd, "Reset\n", 6);
  usleep(10000);
  read(this->SerialPortFd, response2, 256);
  if (strncmp(response2, "Reset", 5))
  {
    throw runtime_error("SamSerialPort exception: unable to reset hardware daemon"); 
  }

  this->isListening = true;

  pthread_mutex_init(&serialPortThreadMutex, NULL);
  pthread_create(&serialPortThread, NULL, SamSerialPortReceiveFxn, this);
}

SamSerialPort::~SamSerialPort()
{
  pthread_mutex_lock(&serialPortThreadMutex);
  pthread_cancel(serialPortThread);
  this->isListening = false;
  pthread_mutex_unlock(&serialPortThreadMutex);
  pthread_join(serialPortThread, NULL);
  pthread_mutex_destroy(&serialPortThreadMutex);

  /*Attempt to reset the device*/
  write(this->SerialPortFd, "Reset\n", 6);
  usleep(10000);

  /*Close the serial port*/
  close(this->SerialPortFd);
}

void SamSerialPort::SendMessage(string message)
{
  if (this->SerialPortFd < 0)
    throw runtime_error("SamSerialPort exception: Attempting to write to an invalid file descriptor for /dev/ttymxc3");

  unsigned int result = write(this->SerialPortFd, message.c_str(), message.length());

  if (result != message.length())
    throw runtime_error("SamSerialPort exception: unable to write complete message to /dev/ttymxc3");
}

