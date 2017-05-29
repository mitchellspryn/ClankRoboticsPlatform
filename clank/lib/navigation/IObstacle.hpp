#ifndef IOBSTACLE_HPP
#define IOBSTACLE_HPP

#include <vector>

#include "types/navigation/CSpacePoint.hpp"

namespace Clank
{
  namespace Navigation
  {
    class IObstacle
    {
      public:
        virtual ~IObstacle() {;};

        /*Gets all of the planes associated with this obstacle*/
        virtual std::vector<std::vector<Clank::Types::Navigation::CSpacePoint> > GetPlanes() = 0;

        /*Merges a plane into this obstacle if it is close enough to an existing plane*/
        virtual bool MergePlane(std::vector<Clank::Types::Navigation::CSpacePoint> plane) = 0;

        /*Forcefully adds a plane to the obstacle. Used when creating a new obstacle*/
        virtual void AddPlane(std::vector<Clank::Types::Navigation::CSpacePoint> plane) = 0;

        /*Determines if a point is within an obstacle, with respect to some origin point*/
        virtual bool IsInside(Clank::Types::Navigation::CSpacePoint testPoint, Clank::Types::Navigation::CSpacePoint origin);


        /*0 -> no penalty for hitting the obstacle. 100 -> catastrophic failure if the obstacle is hit*/
        virtual int GetSeverity() = 0;
        virtual void SetSeverity(int severity) = 0;

        virtual bool operator == (IObstacle &other) = 0;
        virtual bool operator != (IObstacle &other) = 0;
    };
  }
}

#endif
