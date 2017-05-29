#ifndef LINEOFSIGHTPATHPLANNER_HPP
#define LINEOFSIGHTPATHPLANNER_HPP

#include "navigation/IPathPlanner.hpp"

namespace Clank
{
  namespace Navigation
  {
    class LineOfSightPathPlanner:IPathPlanner
    {
      public:
        LineOfSightPathPlanner();
        ~LineOfSightPathPlanner();

        bool ProcessPath(Clank::Navigation::IMap &map, Clank::Types::Navigation::CSpacePoint startingPoint, Clank::Types::Navigation::CSpacePoint endingPoint, int maxSeverity);
    }
  }
}


#endif
