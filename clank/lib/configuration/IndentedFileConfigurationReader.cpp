#include "IndentedFileConfigurationReader.hpp"
using namespace std;
using namespace Clank::Configuration;

IndentedFileConfigurationReader::IndentedFileConfigurationReader()
{ 
}

IndentedFileConfigurationReader::~IndentedFileConfigurationReader()
{
}


Configuration* IndentedFileConfigurationReader::ReadConfigurationHelper(istream &stream, int level, int lineNumber, string tag, string &nextLine, Configuration* parentConfiguration)
{
  Configuration *configuration = new Configuration();
  configuration->SetTag(tag);
  configuration->SetParentConfiguration(parentConfiguration);
  getline(stream, nextLine);
  lineNumber++;
  int nextLevel = this->GetLevel(nextLine);
  while(nextLevel != level)
  {
    if (nextLine.length() == 0)
    {
      break;
    }

    string str1;
    string str2;
    if (nextLevel != level + 1)
    {
      if (nextLevel > level)
        this->ThrowFormattedException(nextLine, lineNumber, "Improper formatting.");
      return configuration;
    }

    if (this->FormatTag(nextLine, str1, nextLevel))
    {
      Configuration *childConfig = this->ReadConfigurationHelper(stream, level + 1, lineNumber, str1, nextLine, configuration);
      configuration->AddChildConfiguration(childConfig);
    }

    else if (this->FormatKeyValuePair(nextLine, str1, str2, nextLevel))
    {
      configuration->AddConfigurationData(str1, str2);
      getline(stream, nextLine);
      lineNumber++;
    }

    else
    {
      this->ThrowFormattedException(nextLine, lineNumber, "Unsupported line");
    }

    nextLevel = this->GetLevel(nextLine);
  }

  return configuration;
}

Configuration* IndentedFileConfigurationReader::ReadFromFile(string filePath)
{
  ifstream inputStream;
  inputStream.open(filePath);

  if (!inputStream.is_open())
  {
    string exceptionMessage = "Exception: Cannot open configuration file: ";
    exceptionMessage += filePath;
    exceptionMessage += ".";
    throw runtime_error(exceptionMessage);
  }

  string nextLine;
  getline(inputStream, nextLine);
  string tag;
  if (!this->FormatTag(nextLine, tag, 0))
  {
    this->ThrowFormattedException(nextLine, 1, "File did not start with a tag.");
    return NULL;
  }

  Configuration* configuration = this->ReadConfigurationHelper(inputStream, 0, 1, tag, nextLine, NULL);

  inputStream.close();
  return configuration;
}

int IndentedFileConfigurationReader::GetLevel(string input)
{
  int level = 0;
  for (unsigned int i = 0; i < input.length(); i++)
  {
    if (input[i] == ' ')
    {
      level++;
    }
    else
    {
      break;
    }
  }
  return level / 2;
}

bool IndentedFileConfigurationReader::FormatTag(string input, string &tag, int level)
{
  for (int i = 0; i < 2 * level; i++)
    if (input[i] != ' ')
      return false;

  size_t colonIndex = input.find_first_of(":");
  tag = input.substr(2*level, colonIndex - (2*level)); //TODO: off by one?

  for (unsigned int i = colonIndex + 1; i < input.length(); i++)
  {
    if (!isspace(input[i]))
      return false;
  }

  return true;
}

bool IndentedFileConfigurationReader::FormatKeyValuePair(string input, string &key, string &value, int level)
{
  for (int i = 0; i < 2*level; i++)
    if (input[i] != ' ')
      return false;

  size_t colonIndex = input.find_first_of(":");
  size_t endIndex = input.find_first_of(";");

  key = input.substr(level*2, colonIndex - (2*level));
  value = input.substr(colonIndex+1, endIndex - colonIndex - 1);


  for (unsigned int i = endIndex + 1; i < input.length(); i++)
  {
    if (!isspace(input[i]))
      return false;
  }

  return true;
}

void IndentedFileConfigurationReader::ThrowFormattedException(string line, int lineNumber, string additionalExceptionInfo)
{
  string exceptionMessage = "Parse Exception on line " + to_string(lineNumber);
  exceptionMessage += ": ";
  exceptionMessage += additionalExceptionInfo;
  exceptionMessage += "\n";
  exceptionMessage += "Line contents: ";
  exceptionMessage += line;
  throw runtime_error(exceptionMessage);
}

