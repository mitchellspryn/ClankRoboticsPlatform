#include "configuration/IndentedFileConfigurationReader.hpp"
#include "gtest/gtest.h"

using namespace std;

class IndentedFileConfigurationReaderTestFixture: public testing::Test
{
  public:
    Clank::Configuration::IndentedFileConfigurationReader fileReader;
};

TEST_F(IndentedFileConfigurationReaderTestFixture, ReadValidFile)
{
  Clank::Configuration::Configuration *config = fileReader.ReadFromFile("IndentedFileConfigurationReaderUnitTestInput_MultiLineGood.txt");

  Clank::Configuration::Configuration firstConfig, secondConfig, thirdConfig; 

  ASSERT_TRUE(config->FindConfigurationByTag("TagOne", firstConfig));
  ASSERT_TRUE(config->FindConfigurationByTag("TagTwo", secondConfig));
  ASSERT_TRUE(config->FindConfigurationByTag("TagThree", thirdConfig));

  string value;
  ASSERT_TRUE(firstConfig.GetConfigurationData("FirstKey", value));
  ASSERT_TRUE(value == "FirstValue");
  ASSERT_TRUE(firstConfig.GetConfigurationData("SecondKey", value));
  ASSERT_TRUE(value == "SecondValue:WithColons");
  ASSERT_FALSE(firstConfig.GetConfigurationData("ThirdKey", value));

  ASSERT_TRUE(secondConfig.GetConfigurationData("KeyOne", value));
  ASSERT_TRUE(value == "ValueOne");
  ASSERT_TRUE(secondConfig.GetConfigurationData("KeyTwo", value));
  ASSERT_TRUE(value == "ValueTwo");
  ASSERT_FALSE(secondConfig.GetConfigurationData("KeyThree", value));

  ASSERT_TRUE(thirdConfig.GetConfigurationData("1", value));
  ASSERT_TRUE(value == "A");
  ASSERT_TRUE(thirdConfig.GetConfigurationData("2", value));
  ASSERT_TRUE(value == "B");
  ASSERT_FALSE(thirdConfig.GetConfigurationData("3", value));
}

TEST_F(IndentedFileConfigurationReaderTestFixture, ReadHighlyIndentedFile)
{
  Clank::Configuration::Configuration *config = fileReader.ReadFromFile("IndentedFileConfigurationReaderUnitTestInput_HighlyIndented.txt");

  Clank::Configuration::Configuration firstConfig, secondConfig, thirdConfig, fourthConfig, fifthConfig;

  ASSERT_TRUE(config->FindConfigurationByTag("First", firstConfig));
  ASSERT_TRUE(config->FindConfigurationByTag("Second", secondConfig));
  ASSERT_TRUE(config->FindConfigurationByTag("Third", thirdConfig));
  ASSERT_TRUE(config->FindConfigurationByTag("Fourth", fourthConfig));
  ASSERT_TRUE(config->FindConfigurationByTag("Fifth", fifthConfig));

  string value;

  ASSERT_TRUE(fifthConfig.GetConfigurationData("Key", value));
  ASSERT_TRUE(value == "Value");
}

TEST_F(IndentedFileConfigurationReaderTestFixture, ReadMultipleTopLevels)
{
  Clank::Configuration::Configuration *config = fileReader.ReadFromFile("IndentedFileConfigurationReaderUnitTestInput_MultipleTopLevelTags.txt");

  Clank::Configuration::Configuration firstConfig, secondConfig, firstSubConfig, secondSubConfig;
  ASSERT_TRUE(config->FindConfigurationByTag("FirstTag", firstConfig));
  ASSERT_TRUE(config->FindConfigurationByTag("SecondTag", secondConfig));
  ASSERT_TRUE(firstConfig.FindConfigurationByTag("TagOne", firstSubConfig));
  ASSERT_TRUE(secondConfig.FindConfigurationByTag("TagTwo", secondSubConfig));

  string value;
  ASSERT_TRUE(firstSubConfig.GetConfigurationData("FirstKey", value));
  ASSERT_TRUE(value == "FirstValue");
  ASSERT_TRUE(secondSubConfig.GetConfigurationData("SecondKey", value));
  ASSERT_TRUE(value == "SecondValue");
}

TEST_F(IndentedFileConfigurationReaderTestFixture, BreakOnBadTag)
{
  EXPECT_ANY_THROW(fileReader.ReadFromFile("IndentedFileConfigurationReaderUnitTestInput_BadTagFormatting.txt"));
}

TEST_F(IndentedFileConfigurationReaderTestFixture, BreakOnBadProperty)
{
  EXPECT_ANY_THROW(fileReader.ReadFromFile("IndentedFileConfigurationReaderUnitTestInput_BadValueFormatting.txt"));
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
