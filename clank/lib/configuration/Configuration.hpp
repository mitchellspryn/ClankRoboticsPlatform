#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <vector>
#include <map>

namespace Clank
{
  namespace Configuration
  {
    class Configuration
    {
      public:
        Configuration();
        ~Configuration();

        bool FindConfigurationByTag(std::string tag, Clank::Configuration::Configuration& configuration);
        void SetParentConfiguration(Clank::Configuration::Configuration* parentConfiguration);
        void AddChildConfiguration(Clank::Configuration::Configuration* childConfiguration);
        void AddConfigurationData(std::string key, std::string value);
        bool GetConfigurationData(std::string key, std::string &value);
        void SetTag(std::string tag);
        std::string GetTag();

      private:
        void CleanUp();
        std::vector<Clank::Configuration::Configuration*> ChildConfigurations;
        Clank::Configuration::Configuration* ParentConfiguration;
        std::map<std::string, std::string> ConfigurationValues;
        std::string Tag;
        bool isDeleted; 
    };
  }
}

#endif
