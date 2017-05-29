#include "CSpacePoint.hpp"

using namespace std;
using namespace Clank::Types::Navigation;

CSpacePoint::CSpacePoint(vector<MapPrecisionType> dataPoints)
{
  this->Value = dataPoints;
}

CSpacePoint::CSpacePoint()
{
  this->Value = vector<MapPrecisionType>();
}

CSpacePoint::~CSpacePoint()
{
  this->Value.clear();
}

MapPrecisionType& CSpacePoint::operator [] (int i)
{
  #if DEBUGMODE
    if (i >= this->Value.size())
    {
      string errorMessage = "CSpacePoint exception. Attempted to set index ";
      errorMessage += to_string(i);
      errorMessage += " in a point with dimensionality ";
      errorMessage += to_string(this->Value->size() - 1);
      throw runtime_error(errorMessage);
    }
  #endif
  return this->Value[i];
}

unsigned int CSpacePoint::GetDimensionality()
{
  return this->Value.size();
}

bool CSpacePoint::operator == (CSpacePoint other)
{
  if (other.GetDimensionality() != this->GetDimensionality())
    return false;

  for (unsigned int i = 0; i < this->Value.size(); i++)
  {
    if (this->Value[i] != other[i])
      return false;
  }
  return true;
}

bool CSpacePoint::operator != (CSpacePoint other)
{
  return !(*this == other);
}
