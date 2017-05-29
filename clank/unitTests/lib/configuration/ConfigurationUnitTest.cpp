#include "configuration/Configuration.hpp"
#include "gtest/gtest.h"

using namespace std;

class ConfigurationTestFixture: public testing::Test
{
  public:
    void SetUp()
    {
      configuration = new Clank::Configuration::Configuration();
      
      Clank::Configuration::Configuration* subConfiguration1 = new Clank::Configuration::Configuration();
      subConfiguration1->SetTag("SubTag1");
      subConfiguration1->AddConfigurationData("KeyOne", "ValueOne");
      subConfiguration1->AddConfigurationData("KeyTwo", "ValueTwo");

      
      Clank::Configuration::Configuration* subConfiguration2 = new Clank::Configuration::Configuration();
      subConfiguration2->SetTag("SubTag2");
      subConfiguration2->AddConfigurationData("OneKey", "OneValue");
      subConfiguration2->AddConfigurationData("TwoKey", "TwoValue");

      configuration->AddChildConfiguration(subConfiguration1);
      configuration->AddChildConfiguration(subConfiguration2);
    }

    Clank::Configuration::Configuration* configuration;
};

TEST_F(ConfigurationTestFixture, VerifyFirstConfig)
{
  Clank::Configuration::Configuration config; 
  bool found = configuration->FindConfigurationByTag("SubTag1", config);

  ASSERT_TRUE(found);

  ASSERT_EQ(config.GetTag(), "SubTag1");

  string firstValue, secondValue, thirdValue;

  ASSERT_TRUE(config.GetConfigurationData("KeyOne", firstValue));
  ASSERT_TRUE(firstValue == "ValueOne");

  ASSERT_TRUE(config.GetConfigurationData("KeyTwo", secondValue));
  ASSERT_TRUE(secondValue == "ValueTwo");

  ASSERT_FALSE(config.GetConfigurationData("NonExistantKey", firstValue));
}

TEST_F(ConfigurationTestFixture, VerifySecondConfig)
{
  Clank::Configuration::Configuration config; 
  bool found = configuration->FindConfigurationByTag("SubTag2", config);

  ASSERT_TRUE(found);

  ASSERT_EQ(config.GetTag(), "SubTag2");

  string firstValue, secondValue, thirdValue;

  ASSERT_TRUE(config.GetConfigurationData("OneKey", firstValue));
  ASSERT_TRUE(firstValue == "OneValue");

  ASSERT_TRUE(config.GetConfigurationData("TwoKey", secondValue));
  ASSERT_TRUE(secondValue == "TwoValue");

  ASSERT_FALSE(config.GetConfigurationData("NonExistantKey", firstValue));
}

TEST_F(ConfigurationTestFixture, VerifyNullBehavior)
{
  Clank::Configuration::Configuration config;
  bool found = configuration->FindConfigurationByTag("NonExistantElement", config);

  ASSERT_FALSE(found);
}

TEST_F(ConfigurationTestFixture, AddElement)
{
  Clank::Configuration::Configuration* subConfiguration3 = new Clank::Configuration::Configuration();
  subConfiguration3->SetTag("SubTag3");
  subConfiguration3->AddConfigurationData("A", "a");
  subConfiguration3->AddConfigurationData("B", "b");

  configuration->AddChildConfiguration(subConfiguration3);

  Clank::Configuration::Configuration config;
  bool found = configuration->FindConfigurationByTag("SubTag3", config);

  ASSERT_TRUE(found);

  ASSERT_EQ(config.GetTag(), "SubTag3");

  string firstValue, secondValue, thirdValue;

  ASSERT_TRUE(config.GetConfigurationData("A", firstValue));
  ASSERT_TRUE(firstValue == "a");

  ASSERT_TRUE(config.GetConfigurationData("B", secondValue));
  ASSERT_TRUE(secondValue == "b");

  ASSERT_FALSE(config.GetConfigurationData("NonExistantKey", firstValue));
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
