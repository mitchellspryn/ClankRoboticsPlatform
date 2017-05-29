#include "Configuration.hpp"
using namespace std;
using namespace Clank::Configuration;

#include <iostream>

Configuration::Configuration()
{
  this->ChildConfigurations = vector<Configuration*>();
  this->ConfigurationValues = map<string, string>();  
}

Configuration::~Configuration()
{
  for (unsigned int i = 0; i < this->ChildConfigurations.size(); i++)
  {
    this->ChildConfigurations[i]->CleanUp(); 
  }
}

void Configuration::CleanUp()
{
  for (unsigned int i = 0; i < this->ChildConfigurations.size(); i++)
  {
    if (this->ChildConfigurations[i] != NULL)
    {
      delete this->ChildConfigurations[i];
    }
  }
  this->ChildConfigurations = vector<Configuration*>();
  this->ConfigurationValues = map<string, string>();
}

bool Configuration::FindConfigurationByTag(string tag, Configuration &configuration)
{
  if (this->GetTag() == tag)
  {
    configuration = *this;
    return true;
  }

  for (unsigned int i = 0; i < this->ChildConfigurations.size(); i++)
  {
    if (this->ChildConfigurations[i]->FindConfigurationByTag(tag, configuration))
    {
      return true;
    }
  }

  return false;
}

void Configuration::AddChildConfiguration(Configuration* childConfiguration)
{
  this->ChildConfigurations.push_back(childConfiguration);
}

void Configuration::AddConfigurationData(string key, string value)
{
  this->ConfigurationValues[key] = value;
}

bool Configuration::GetConfigurationData(string key, string &value)
{

  if (this->ConfigurationValues.count(key) != 1)
  {
    value = "";
    return false;
  }

  value = this->ConfigurationValues[key];
  return true;
}

void Configuration::SetTag(string tag)
{
  this->Tag = tag;
}

void Configuration::SetParentConfiguration(Configuration* parentConfiguration)
{
  this->ParentConfiguration = parentConfiguration;
}

string Configuration::GetTag()
{
  return this->Tag;
}
