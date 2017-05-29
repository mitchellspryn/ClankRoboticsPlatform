#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <vector>
#include <typeinfo>

#include "types/navigation/CSpacePoint.hpp"
#include "IObstacle.hpp"

namespace Clank
{
  namespace Navigation
  {
    class Obstacle : public IObstacle
    {
      public:
        Obstacle(std::vector<Clank::Types::Navigation::CSpacePoint> corners, int severity);
        ~Obstacle();

        std::vector<Clank::Types::Navigation::CSpacePoint> GetCorners();
        void SetCorners(std::vector<Clank::Types::Navigation::CSpacePoint> corners);

        int GetSeverity();
        void SetSeverity(int severity);

        bool operator == (IObstacle &other);
        bool operator != (IObstacle &other);

      private:
        std::vector<Clank::Types::Navigation::CSpacePoint> Points;
        int Severity;
    };
  }
}


#endif
