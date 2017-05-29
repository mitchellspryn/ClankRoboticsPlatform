#ifndef IMAP_HPP
#define IMAP_HPP

#include <vector>
#include <map>

#include "types/navigation/CSpacePoint.hpp"
#include "IObstacle.hpp"

namespace Clank
{
  namespace Navigation
  {
    class IMap
    {
      public:
        virtual ~IMap() {;};

        /*Adds an obstacle with specified severity (0 = clear, 100 = impassable) to the map*/
        virtual void AddObstacle(Clank::Navigation::IObstacle *obstacle) = 0;

        /*Gets a list of all obstacles in the map*/
        virtual std::vector<Clank::Navigation::IObstacle*> GetObstacles() = 0;

        /*Removes an obstacle from the map whose corners match obstacleToRemove, if it exists*/
        virtual void RemoveObstacle(Clank::Navigation::IObstacle* obstacleToRemove) = 0;

        void SetPath(std::vector<Clank::Types::Navigation::CSpacePoint> path);
        std::vector<Clank::Types::Navigation::CSpacePoint> GetPath();

        void Serialize(std::string filepath);
        void Deseralize(std::string filepath);

        void Visualize(std::string filepath, std::string pythonpath);
    };
  }
}

#endif
