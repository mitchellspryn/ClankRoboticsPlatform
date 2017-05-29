#ifndef IPATHPLANNER_HPP
#define IPATHPLANNER_HPP

#include "navigation/IMap.hpp"

namespace Clank
{
  namespace Navigation
  {
    class IPathPlanner
    {
      public:
        virtual IPathPlanner() = 0;
        virtual ~IPathPlanner {;};

        /*Returns true if there exists a valid path from the current position to the goal,
         * false otherwise*/
        virtual bool ProcessPath(Clank::Navigation::IMap &map, Clank::Types::Navigation::CSpacePoint startingPoint, Clank::Types::Navigation::CSpacePoint endingPoint, int maxSeverity);
    }
  }
}

#endif
