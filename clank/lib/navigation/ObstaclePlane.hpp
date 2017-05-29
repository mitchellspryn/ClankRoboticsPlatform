#ifndef OBSTACLEPLANE_HPP
#define OBSTACLEPLANE_HPP

#include <vector>

#include "IObstaclePlane.hpp"
#include "types/navigation/CSpacePoint.hpp"

namespace Clank
{
  namespace Navigation
  {
    class ObstaclePlane : public IObstaclePlane
    {
      public:
        ObstaclePlane();
        ObstaclePlane(std::vector<Clank::Types::Navigation::CSpacePoint> corners);

        ~ObstaclePlane();

        std::vector<Clank::Types::Navigation::CspacePoint> GetCorners();

        void SetCorners(std::vector<Clank::Types::Navigation::CSpacePoint> corners);

        bool DoesRayIntersect(Clank::Types::Navigation::CSpacePoint endPoint1, Clank::Types::Navigation::CSpacePoint endPoint2);

      private:
        std::vector<Clank::Types::Navigation::CSpacePoint> Corners;
    }
  }
}


#endif
