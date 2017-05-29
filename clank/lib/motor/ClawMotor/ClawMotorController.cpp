#include "ClawMotorController.hpp"

using namespace std;
using namespace Clank::Configuration;
using namespace Clank::Motor;

ClawMotorController::ClawMotorController(Clank::Configuration::Configuration *configuration)
{
  string serialPortName, baudRateStr, timeoutMillisStr, addressStr;
  if (!configuration->GetConfigurationData("SerialPortName", serialPortName)
      ||
      !configuration->GetConfigurationData("BaudRate", baudRateStr)
      ||
      !configuration->GetConfigurationData("TimeoutMillis", timeoutMillisStr)
      ||
      !configuration->GetConfigurationData("Address", addressStr)
     )
    {
      string errorMessage = "ClawMotorController error: invalid configuration.\n";
      errorMessage += "\tValid configuration: <SerialPortName> <BaudRate>";
      throw runtime_error(errorMessage);
    }

  unsigned int baudRate = (unsigned int)stoi(baudRateStr); 

  long timeoutMillisLong = stol(timeoutMillisStr);
  if (timeoutMillisLong <= 0)
  {
    string errorMessage = "ClawMotorController error: serial port timeout <= 0.";
    throw runtime_error(errorMessage);
  }
  if ((unsigned long)timeoutMillisLong > UINT_MAX)
  {
    string errorMessage = "ClawMotorController error: serial port timeout too long.";
    throw runtime_error(errorMessage);
  }

  unsigned int timeoutMillis = (unsigned int) timeoutMillisLong;

  unsigned int address = (unsigned int) stoi(addressStr);

  this->Initialize(serialPortName, baudRate, timeoutMillis, address);
  
}

ClawMotorController::ClawMotorController(string serialPortName, unsigned int baudRate, unsigned int timeoutMillis, unsigned int address)
{
  this->Initialize(serialPortName, baudRate, timeoutMillis, address); 
}

void ClawMotorController::Initialize(string serialPortName, unsigned int baudRate, unsigned int timeoutMillis, unsigned int address)
{
  speed_t baudRate_t;
  
  if (baudRate == 2400)
  {
    baudRate_t = B2400;
  }
  else if(baudRate == 9600)
  {
    baudRate_t = B9600;
  }
  else if(baudRate == 9600)
  {
    baudRate_t = B19200;
  }
  else if(baudRate == 38400)
  {
    baudRate_t = B38400;
  }
  else if(baudRate == 57600)
  {
    baudRate_t = B57600;
  }
  else if(baudRate == 115200)
  {
    baudRate_t = B115200;
  }
  else if(baudRate == 230400)
  {
    baudRate_t = B230400;
  }
  else if(baudRate == 460800)
  {
    baudRate_t = B460800;
  }
  else
  {
    string errorMessage = "ClawMotorController error: unrecognized baud rate: " + to_string(baudRate) + "\n";
    errorMessage += "\tValid values: <2400> <9600> <19200> <38400> <57600> <115200> <230400> <460800>\n";
    throw runtime_error(errorMessage);
  }


  this->SerialPortFd = open(serialPortName.c_str(), O_RDWR | O_NOCTTY);
  if (this->SerialPortFd < 0)
  {
    string errorMessage = "ClawMotorController exception: Unable to open serial port " + serialPortName;
    throw runtime_error(errorMessage);
  }

  /*Configure the port*/
  struct termios termOptions;

  fcntl(this->SerialPortFd, F_SETFL, 0); //Remove any special modifiers from this port
  
  tcgetattr(this->SerialPortFd, &termOptions); //Get old options

  termOptions.c_cflag |= (CLOCAL | CREAD); //Local connection (no modem control lines)
                                           //Enable read characters
  termOptions.c_cflag &= ~PARENB;          //No parity
  termOptions.c_cflag &= ~CSTOPB;          //Single stop bit
  termOptions.c_cflag &= ~CSIZE;           //Reset character size field
  termOptions.c_cflag |= CS8;              //8 bit characters
  termOptions.c_cflag &= ~CRTSCTS;         //No hardware flow control

  cfsetispeed(&termOptions, baudRate_t);     //Set baud rates
  cfsetospeed(&termOptions, baudRate_t);

  termOptions.c_lflag &= ~ICANON;          //Turn off canonical mode
  cfmakeraw(&termOptions);                 //Remove special character processing

  tcsetattr(this->SerialPortFd, TCSANOW, &termOptions);

  function<double(void)> GetMotor1VelocityFxn = std::bind(&Clank::Motor::ClawMotorController::GetMotor1Velocity, this);
  function<void(double)> SetMotor1VelocityFxn = std::bind(&Clank::Motor::ClawMotorController::SetMotor1Velocity, this, std::placeholders::_1);
  function<double(void)> GetMotor1RampSpeedFxn = std::bind(&Clank::Motor::ClawMotorController::GetMotor1RampSpeed, this);
  function<void(double)> SetMotor1RampSpeedFxn = std::bind(&Clank::Motor::ClawMotorController::SetMotor1RampSpeed, this, std::placeholders::_1);

  function<double(void)> GetMotor2VelocityFxn = std::bind(&Clank::Motor::ClawMotorController::GetMotor2Velocity, this);
  function<void(double)> SetMotor2VelocityFxn = std::bind(&Clank::Motor::ClawMotorController::SetMotor2Velocity, this, std::placeholders::_1);
  function<double(void)> GetMotor2RampSpeedFxn = std::bind(&Clank::Motor::ClawMotorController::GetMotor2RampSpeed, this);
  function<void(double)> SetMotor2RampSpeedFxn = std::bind(&Clank::Motor::ClawMotorController::SetMotor2RampSpeed, this, std::placeholders::_1);

  this->Motor1 = new ClawMotor
  (
    GetMotor1VelocityFxn,
    SetMotor1VelocityFxn,
    GetMotor1RampSpeedFxn,
    SetMotor1RampSpeedFxn
  );

  this->Motor2 = new ClawMotor
  (
    GetMotor2VelocityFxn,
    SetMotor2VelocityFxn,
    GetMotor2RampSpeedFxn,
    SetMotor2RampSpeedFxn
  );

  /*Set timeout*/
  this->Timeout.tv_sec = timeoutMillis / 1000;
  this->Timeout.tv_usec = (timeoutMillis % 1000) * 1000;

  FD_ZERO(&this->FileDescriptorSet);
  FD_SET(this->SerialPortFd, &this->FileDescriptorSet);

  if (address < 0x80 || address > 0x87)
  {
    string errorMessage = "ClawMotorController exception: Address out of range.\n";
    errorMessage += "Expected value on range [0x80, 0x87], got " + to_string(address);
    throw runtime_error(errorMessage);
  }
  this->Address = address;

  pthread_mutex_init(&mutex, NULL);
}

ClawMotorController::~ClawMotorController()
{
  if (this->Motor1 != NULL)
  {
    this->Motor1->SetVelocity(0);
  }

  if (this->Motor2 != NULL)
  {
    this->Motor2->SetVelocity(0);
  }

  delete this->Motor1;
  delete this->Motor2;
  pthread_mutex_destroy(&mutex);
}

IMotor* ClawMotorController::GetMotor1()
{
  return this->Motor1;
}

IMotor* ClawMotorController::GetMotor2()
{
  return this->Motor2;
}

double ClawMotorController::GetMotor1Velocity()
{
  return this->Motor1Velocity;
}

void ClawMotorController::SetMotor1Velocity(double velocity)
{
  this->DataBuffer[0] = this->Address;
  this->DataBuffer[1] = (velocity > 0 ? (unsigned char)0 : (unsigned char)1);
  this->DataBuffer[2] = (unsigned char)(std::abs(velocity) * 127.0 / 100.0);
  unsigned int crc = this->ComputeCRC(3);
  this->DataBuffer[3] = (unsigned char)((crc >> 8) & 0x00FF);
  this->DataBuffer[4] = (unsigned char)(crc & 0x00FF);
  this->WriteAndVerify(5);
  this->Motor1Velocity = velocity;
}

double ClawMotorController::GetMotor1RampSpeed()
{
  return this->Motor1RampSpeed;
}

void ClawMotorController::SetMotor1RampSpeed(double speed)
{
  this->Motor1RampSpeed = speed;
  /*TODO: Do we need to implement this?*/
}

double ClawMotorController::GetMotor2Velocity()
{
  return this->Motor2Velocity;
}

void ClawMotorController::SetMotor2Velocity(double velocity)
{
  this->DataBuffer[0] = this->Address;
  this->DataBuffer[1] = (velocity > 0 ? (unsigned char)4 : (unsigned char)5);
  this->DataBuffer[2] = (unsigned char)(std::abs(velocity) * 127.0 / 100.0);
  unsigned int crc = this->ComputeCRC(3);
  this->DataBuffer[3] = (unsigned char)((crc >> 8) & 0x00FF);
  this->DataBuffer[4] = (unsigned char)(crc & 0x00FF);
  this->WriteAndVerify(5);
  this->Motor2Velocity = velocity;
}

double ClawMotorController::GetMotor2RampSpeed()
{
  return this->Motor2RampSpeed;
}

void ClawMotorController::SetMotor2RampSpeed(double speed)
{
  this->Motor2RampSpeed = speed;
  /*TODO: Do we need to implement this?*/
}

bool ClawMotorController::WriteAndVerify(unsigned int numBytes)
{
  unsigned char response = 0x00;
  pthread_mutex_lock(&mutex); 

  write(this->SerialPortFd, this->DataBuffer, numBytes);

  int selectVal = select(this->SerialPortFd + 1, &this->FileDescriptorSet, NULL, NULL, &this->Timeout);

  if (selectVal <= 0)
  {
    string errorMessage = "ClawMotorController exception: motor controller did not respond to the command {";
    for (unsigned int i = 0; i < numBytes-1; i++)
    {
      errorMessage += to_string(this->DataBuffer[i]);
      errorMessage += ",";
    }
    errorMessage += to_string(this->DataBuffer[numBytes-1]);
    errorMessage += "} before the set timeout;\n";
    throw runtime_error(errorMessage);
  }

  read(this->SerialPortFd, &response, 1);
  
  if (response != 0xFF)
  {
    string errorMessage = "ClawMotorController exception: motor controller did not recognize the command {";
    for (unsigned int i = 0; i < numBytes-1; i++)
    {
      errorMessage += to_string(this->DataBuffer[i]);
      errorMessage += ",";
    }
    errorMessage += to_string(this->DataBuffer[numBytes-1]);
    errorMessage += "};\n";
    throw runtime_error(errorMessage);
  }

  pthread_mutex_unlock(&mutex);
  return true;
}

unsigned int ClawMotorController::ComputeCRC(unsigned int numBytes)
{
  /*TODO: Is this initialization correct?*/
  unsigned int crc = 0;

  for (unsigned int byte = 0; byte < numBytes; byte++)
  {
    crc = crc ^ ((unsigned int) this->DataBuffer[byte] << 8);

    for (unsigned char bit = 0; bit < 8; bit++)
    {
      if (crc & 0x8000)
      {
        crc = (crc << 1) ^ 0x1021;
      }
      else
      {
        crc = (crc << 1);
      }
    }
  }

  return crc;
}
