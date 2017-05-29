#include "navigation/SparseMap.hpp"
#include "navigation/Obstacle.hpp"
#include "navigation/IObstacle.hpp"
#include "types/navigation/CSpacePoint.hpp"
#include "types/navigation/MapPrecision.hpp"
#include "gtest/gtest.h"

using namespace std;
using namespace Clank::Types::Navigation;
using namespace Clank::Navigation;

SparseMap* InitializeMap()
{
  /*Create points*/
  /*Create the map*/
  SparseMap* sparseMap = new SparseMap(
    CSpacePoint(
      vector<MapPrecisionType>({0, 0})),
    CSpacePoint(
      vector<MapPrecisionType>({10, 10}))
  );

  /*Add two obstacles*/
  sparseMap->AddObstacle(
    new Obstacle(
      vector<CSpacePoint>({
        CSpacePoint(
          vector<MapPrecisionType>({1, 1})),
        CSpacePoint(
          vector<MapPrecisionType>({1, 2})),
        CSpacePoint(
          vector<MapPrecisionType>({2, 1})),
        CSpacePoint(
          vector<MapPrecisionType>({2, 2}))
        }),
        100
      )
    );

  sparseMap->AddObstacle(
    new Obstacle(
      vector<CSpacePoint>({
        CSpacePoint(
          vector<MapPrecisionType>({5, 5})),
        CSpacePoint(
          vector<MapPrecisionType>({5, 6})),
        CSpacePoint(
          vector<MapPrecisionType>({6, 5})),
        CSpacePoint(
          vector<MapPrecisionType>({6, 6}))
        }),
        100
      )
    );

  /*Add some waypoints*/
  sparseMap->SetPath(
    vector<CSpacePoint>({
      CSpacePoint(
        vector<MapPrecisionType>({2, 3})),
      CSpacePoint(
        vector<MapPrecisionType>({3, 4}))
      })
    );

  return sparseMap;
}

TEST(SparseMapTest, InitAddAndRemoveObstacles)
{
  /*Create the default map*/
  SparseMap* sparseMap = InitializeMap();

  /*Get the obstacles*/
  vector<IObstacle*> obstacles = sparseMap->GetObstacles();

  /*Verify that we have two obstacles*/
  ASSERT_EQ(obstacles.size(), 2);

  /*Delete the first obstacle*/
  sparseMap->RemoveObstacle(obstacles[0]);

  /*Get the obstacles and verify that we only have one left*/
  obstacles = sparseMap->GetObstacles();

  ASSERT_EQ(obstacles.size(), 1);

  /*Also check that the obstacle remaining is the one we expect*/
  Obstacle* expectedObstacle = 
    new Obstacle(
        vector<CSpacePoint>({
          CSpacePoint(
            vector<MapPrecisionType>({5, 5})),
          CSpacePoint(
            vector<MapPrecisionType>({5, 6})),
          CSpacePoint(
            vector<MapPrecisionType>({6, 5})),
          CSpacePoint(
            vector<MapPrecisionType>({6, 6}))
          }),
          100
        );

  ASSERT_TRUE(*expectedObstacle == *obstacles[0]);  
}

TEST(SparseMapTest, SerializeAndDeserialize)
{
  string testFile = "SerializationTestfile.map";

  /*Create the default map*/
  SparseMap* expectedSparseMap = InitializeMap();

  /*Serialize to test file*/
  expectedSparseMap->Serialize(testFile);

  /*Deserialize to a new object*/
  SparseMap* actualSparseMap = new SparseMap(testFile);

  /*Assert that the corners are the same*/
  ASSERT_TRUE(expectedSparseMap->GetMinimumBounds() == actualSparseMap->GetMinimumBounds());
  ASSERT_TRUE(expectedSparseMap->GetMaximumBounds() == actualSparseMap->GetMaximumBounds());

  /*Assert that the obstacles are the same*/
  vector<IObstacle*> expectedObstacles = expectedSparseMap->GetObstacles();
  vector<IObstacle*> actualObstacles = actualSparseMap->GetObstacles();

  for (unsigned int i = 0; i < expectedObstacles.size(); i++)
  {
    ASSERT_TRUE(*((Obstacle*)expectedObstacles[i]) == *actualObstacles[i]);
  }

  /*Assert that the paths are the same*/
  vector<CSpacePoint> expectedPath = expectedSparseMap->GetPath();
  vector<CSpacePoint> actualPath = actualSparseMap->GetPath();

  ASSERT_EQ(expectedPath.size(), actualPath.size());

  for (unsigned int i = 0; i < expectedPath.size(); i++)
  {
    ASSERT_TRUE(expectedPath[i] == actualPath[i]);
  }
}
