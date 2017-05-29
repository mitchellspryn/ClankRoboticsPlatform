#ifndef CSPACEPOINT_HPP
#define CSPACEPOINT_HPP

#include <vector>
#include <stdexcept>
#include <string>

#include "MapPrecision.hpp"


namespace Clank
{
  namespace Types
  {
    namespace Navigation
    {
      class CSpacePoint
      {
        public:
          CSpacePoint(std::vector<Clank::Types::Navigation::MapPrecisionType> dataPoints);
          CSpacePoint();
          ~CSpacePoint();

          unsigned int GetDimensionality();

          bool operator == (CSpacePoint other);
          bool operator != (CSpacePoint other);

          Clank::Types::Navigation::MapPrecisionType& operator [] (int i);

        private:
          std::vector<Clank::Types::Navigation::MapPrecisionType> Value;
      };
    }
  }
}

#endif
