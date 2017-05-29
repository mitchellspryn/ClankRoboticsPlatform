#include "navigation/SparseMap.hpp"
#include "navigation/Obstacle.hpp"
#include "navigation/IObstacle.hpp"
#include "types/navigation/CSpacePoint.hpp"
#include "types/navigation/MapPrecision.hpp"

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

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    cout << "Usage: ./Visualize2DMap <MapFileName> <PythonPath>" << endl;
    cout << "\t<MapFileName> : The filename for the visualization file to write" << endl;
    cout << "\t<PythonPath> : Full path to the python map executable" << endl;
    return 0;
  }

  string mapFileName(argv[1]);
  string pythonPath(argv[2]);

  cout << "Creating map..." << endl;
  SparseMap *map = InitializeMap();

  cout << "Writing map file and visualizing..." << endl;
  map->Visualize(mapFileName, pythonPath);

  cout << "Done!" << endl;
}

