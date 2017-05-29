#ifndef INDENTEDFILECONFIGURATIONREADER_HPP
#define INDENTEDFILECONFIGURATIONREADER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Configuration.hpp"

namespace Clank
{
  namespace Configuration
  {
    class IndentedFileConfigurationReader
    {
      public:
        IndentedFileConfigurationReader();
        ~IndentedFileConfigurationReader();

        Clank::Configuration::Configuration* ReadFromFile(std::string filePath);

      private:
        Clank::Configuration::Configuration* ReadConfigurationHelper(std::istream &stream, int level, int lineNumber, std::string tag, std::string &nextLine, Clank::Configuration::Configuration* parentConfiguration);
        void ThrowFormattedException(std::string line, int lineNumber, std::string additionalExceptionInfo);
        int GetLevel(std::string input);
        bool FormatTag(std::string input, std::string &tag, int level);
        bool FormatKeyValuePair(std::string input, std::string &key, std::string &value, int level);
    };
  }
}

#endif
