#ifndef IOBSTACLEPLANE_HPP
#define IOBSTACLEPLANE_HPP

#include <vector>
#include "types/navigation/CSpacePoint.hpp"

namespace Clank
{
  namespace Navigation
  {
    class IObstaclePlane
    {
        virtual ~IObstacle() {;};

        /*Gets the corners associated with the current plane*/
        virtual std::vector<Clank::Types::Navigation::CSpacePoint> GetCorners() = 0;

        /*Sets the corners associated with the current plane*/
        virtual void SetCorners(std::vector<Clank::Types::Navigation::CSpacePoint> corners) = 0;

        /*Returns true if the ray drawn between endPoint1 and endPoint2 intersects this hyperplane*/
        virtual bool DoesRayIntersect(Clank::Types::Navigation::CSpacePoint endPoint1, Clank::Types::Navigation::CSpacePoint endPoint2) = 0;
    }
  }
}
