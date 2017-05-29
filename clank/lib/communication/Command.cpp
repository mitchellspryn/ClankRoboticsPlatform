#include "Command.hpp"

using namespace std;
using namespace Clank::Communication;

Command::Command()
{
  this->errors = vector<const char*>();
  this->Valid = false;
  this->Module = NULL;
  this->Component = NULL;
  this->Property = NULL;
  this->Value = NULL;
  for (int i = 0; i < 256; i++)
  {
    this->workBuf[i] = 0;
  }
  return;
}

Command::~Command()
{
  ;
}

bool Command::setModule(const char* _M)
{
    this->Module = _M;
    return true;
}

const char* Command::getModule()
{
  return this->Module;
}

bool Command::setComponent(const char* _C)
{
  this->Component = _C;
  return true;
}

const char* Command::getComponent()
{
  return this->Component;
}

bool Command::setProperty(const char* _P)
{
  this->Property = _P;
  return true;      
}

const char* Command::getProperty()
{
  return this->Property;
}

bool Command::setValue(const char* _V)
{
  this->Value = _V;
  return true;
}

const char* Command::getVal()
{
  return this->Value;
}

bool Command::isValid()
{
  return (strlen(this->Module) != 0 && strlen(this->Component) != 0 && strlen(this->Property) != 0 && strlen(this->Value) != 0);
}

vector<const char*> Command::getErr()
{
  return this->errors;
}

bool Command::Parse(const char* _Module, const char* _Component, const char* _Property, const char* _Value)
{
  this->Valid = false;
  this->setModule(_Module);
  this->setComponent(_Component);
  this->setProperty(_Property);
  this->setValue(_Value);
  return this->Valid; 
}

bool Command::Parse(const char* str)
{
  this->errors = vector<const char*>();
  this->Module = NULL;
  this->Component = NULL;
  this->Property = NULL;
  this->Value = NULL;

  /*Divide up command based on initial protocol*/
  strcpy(this->workBuf, str);
  
  char* moduleSeparator = strchr(this->workBuf, ':');
  char* componentSeparator = strchr(this->workBuf, '.');
  char* commandSeparator = strstr(this->workBuf, "=(");
  char* finalBracket = strchr(this->workBuf, ')');


  if (moduleSeparator != NULL && componentSeparator != NULL && commandSeparator != NULL && finalBracket != NULL)
  {
    *moduleSeparator = 0;
    moduleSeparator++;
    *componentSeparator = 0;
    componentSeparator++;
    *commandSeparator = 0;
    commandSeparator++;
    *commandSeparator = 0;
    commandSeparator++;
    *finalBracket = 0;
    this->Parse(this->workBuf, moduleSeparator, componentSeparator, commandSeparator); 
    return true;
  }
  return false;
}

void Command::printCmd(FILE *fp)
{
  fprintf(fp, "%s:%s.%s=(%s)\n", this->Module, this->Component, this->Property, this->Value);
  return;
}
