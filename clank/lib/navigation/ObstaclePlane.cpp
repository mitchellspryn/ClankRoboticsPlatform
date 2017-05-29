#include "ObstaclePlane.hpp"

using namespace std;
using namespace Clank::Types::Navigation;

ObstaclePlane::ObstaclePlane(vector<CSpacePoint> corners)
{
  this->Corners = corners;
  /*TODO: Cache normal vector and in-plane vector*/
}

ObstaclePlane::ObstaclePlane()
{
  this->Corners = vector<CSpacePoint>;
}

ObstaclePlane::~ObstaclePlane()
{
  if (this->Corners.size() > 0)
  {
    this->Corners.clear();
  }
}

vector<CSpacePoint> ObstaclePlane::GetCorners()
{
  return this->Corners;
}

void ObstaclePlane::SetCorners(vector<CSpacePoint> corners)
{
  this->Corners = corners;
}

bool ObstaclePlane::DoesRayIntersect(CSpacePoint endPoint1, CSpacePoint endPoint2)
{
  /*TODO: Write algorithm. Steps:
   * 1) Project ray onto plane
   * 2) Create in-plane line segment
   * 3) Solve matrix equations to determine if the line segment intersects corner lines
   */
}
