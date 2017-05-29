#include "SamPeripheral.hpp"

using namespace std;
using namespace Clank::Hardware;

SamPeripheral::SamPeripheral(Clank::Hardware::SamPeripheralName name)
{
  this->Init(name); 
}

SamPeripheral::SamPeripheral(std::string name)
{
  SamPeripheralName enumeratedName = SamPeripheralExtentions::StringToPeripheralName(name);
  this->Init(enumeratedName);
}

SamPeripheral::~SamPeripheral()
{
  this->SamPeripheralManager->Return(this->Name);
}

void SamPeripheral::Init(Clank::Hardware::SamPeripheralName name)
{
  this->SamSerialPort = SamSerialPort::Instance();
  this->SamPeripheralManager = SamPeripheralManager::Instance();
  this->SamPeripheralManager->InitializeSerialPort();
  this->SamPeripheralManager->Get(name, this); //Throws if error
  this->Name = name;
}

void SamPeripheral::SetState()
{
  this->SamSerialPort->SendMessage(
      this->StateToMessage()
  );
}

Clank::Hardware::SamPeripheralName SamPeripheral::GetName()
{
  return this->Name;
}
