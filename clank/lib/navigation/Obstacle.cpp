#include "Obstacle.hpp"

#include <iostream>

using namespace std;
using namespace Clank::Types::Navigation;
using namespace Clank::Navigation;

Obstacle::Obstacle(vector<CSpacePoint> corners, int severity)
{
  this->Points = corners;
  this->Severity = severity;
}

Obstacle::~Obstacle()
{
  if (this->Points.size() > 0)
  {
    this->Points.clear();
  }
}

vector<CSpacePoint> Obstacle::GetCorners()
{
  return this->Points;
}

void Obstacle::SetCorners(vector<CSpacePoint> corners)
{
  this->Points = corners;
}

int Obstacle::GetSeverity()
{
  return this->Severity;
}

void Obstacle::SetSeverity(int severity)
{
  this->Severity = severity;
}

bool Obstacle::operator == (IObstacle &other)
{
  if (other.GetSeverity() != this->Severity)
    return false;

  vector<CSpacePoint> otherPoints = other.GetCorners();
  vector<CSpacePoint> thisPoints = this->Points;

  if (otherPoints.size() != thisPoints.size())
    return false;

  for (unsigned int i = 0; i < thisPoints.size(); i++)
  {
    if (otherPoints[i] != thisPoints[i])
    {
      return false;
    }
  }

  return true;

}

bool Obstacle::operator != (IObstacle &other)
{
  return !(*this == other);
}

