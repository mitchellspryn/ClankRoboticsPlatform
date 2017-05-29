#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <stdlib.h>

#include "IMap.hpp"
#include "IObstacle.hpp"
#include "Obstacle.hpp"


namespace Clank
{
  namespace Navigation
  {
    class SparseMap : public IMap
    {
      public:
        SparseMap(Clank::Types::Navigation::CSpacePoint corner1, Clank::Types::Navigation::CSpacePoint corner2);
        SparseMap(std::string filepath);
        ~SparseMap();

        void AddObstacle(Clank::Navigation::IObstacle *obstacle);
        std::vector<Clank::Navigation::IObstacle*> GetObstacles();
        void RemoveObstacle(Clank::Navigation::IObstacle *obstacleToRemove);

        Clank::Types::Navigation::CSpacePoint GetMinimumBounds();
        Clank::Types::Navigation::CSpacePoint GetMaximumBounds();

        std::vector<Clank::Types::Navigation::CSpacePoint> GetPath();
        void SetPath(std::vector<Clank::Types::Navigation::CSpacePoint> path);

        void Serialize(std::string filepath);
        void Deserialize(std::string filepath);

        void Visualize(std::string filepath, std::string pythonpath);

      private:
        Clank::Types::Navigation::CSpacePoint Corner1;
        Clank::Types::Navigation::CSpacePoint Corner2;

        std::vector<Clank::Navigation::IObstacle*> Obstacles;

        std::vector<Clank::Types::Navigation::CSpacePoint> LastComputedPath;

        void SerializePoint(std::ofstream& stream, Clank::Types::Navigation::CSpacePoint point);
        Clank::Types::Navigation::CSpacePoint DeserializePoint(std::ifstream& stream);

        void SerializeObstacle(std::ofstream& stream, IObstacle* obstacle);
        IObstacle* DeserializeObstacle(std::ifstream& stream);
    };
  }

}


#endif
