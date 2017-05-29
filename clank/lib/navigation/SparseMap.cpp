#include "SparseMap.hpp"

#include <iostream>

using namespace std;
using namespace Clank::Types::Navigation;
using namespace Clank::Navigation;

SparseMap::SparseMap(CSpacePoint corner1, CSpacePoint corner2)
{
  this->Corner1 = corner1;
  this->Corner2 = corner2;
  this->Obstacles = vector<IObstacle*>();
  this->LastComputedPath = vector<CSpacePoint>();
}

SparseMap::SparseMap(string filepath)
{
  this->Obstacles = vector<IObstacle*>();
  this->LastComputedPath = vector<CSpacePoint>();
  this->Deserialize(filepath);
}

SparseMap::~SparseMap()
{
  if (this->Obstacles.size() > 0)
  {
    for (unsigned int i = 0; i < this->Obstacles.size(); i++)
      delete this->Obstacles[i];
    this->Obstacles.clear();
  }
}

void SparseMap::AddObstacle(IObstacle *obstacle)
{
  this->Obstacles.push_back(obstacle);
}

vector<IObstacle*> SparseMap::GetObstacles()
{
  return this->Obstacles;
}

void SparseMap::RemoveObstacle(IObstacle* obstacleToRemove)
{
  for (unsigned int i = 0; i < this->Obstacles.size(); i++)
  {
    if (obstacleToRemove == this->Obstacles[i])
    {
      delete this->Obstacles[i];
      this->Obstacles.erase(this->Obstacles.begin() + i);
    }
  }
}

CSpacePoint SparseMap::GetMinimumBounds()
{
  vector<MapPrecisionType> minimums;
  for (unsigned int i = 0; i < this->Corner1.GetDimensionality(); i++)
  {
    MapPrecisionType c1Val = this->Corner1[i];
    MapPrecisionType c2Val = this->Corner2[i];

    if (c1Val < c2Val)
      minimums.push_back(c1Val);
    else
      minimums.push_back(c2Val);
  }
  return minimums;
}

CSpacePoint SparseMap::GetMaximumBounds()
{
  vector<MapPrecisionType> maximums;
  for (unsigned int i = 0; i < this->Corner1.GetDimensionality(); i++)
  {
    MapPrecisionType c1Val = this->Corner1[i];
    MapPrecisionType c2Val = this->Corner2[i];

    if (c1Val > c2Val)
      maximums.push_back(c1Val);
    else
      maximums.push_back(c2Val);
  }
  return maximums;
}

vector<CSpacePoint> SparseMap::GetPath()
{
  return this->LastComputedPath; 
}

void SparseMap::SetPath(vector<CSpacePoint> path)
{
  this->LastComputedPath = path;
}

void SparseMap::Serialize(string filepath)
{
  ofstream outputStream;
  outputStream.open(filepath);

  /*Write the corners*/
  this->SerializePoint(outputStream, this->Corner1);
  this->SerializePoint(outputStream, this->Corner2);
  outputStream << endl;

  /*Write the obstacles*/
  for (unsigned int i = 0; i < this->Obstacles.size(); i++)
    this->SerializeObstacle(outputStream, this->Obstacles[i]);

  outputStream << "-" << endl;

  for (unsigned int i = 0; i < this->LastComputedPath.size(); i++)
    this->SerializePoint(outputStream, this->LastComputedPath[i]);

  outputStream.close();
}

void SparseMap::Deserialize(string filepath)
{
  ifstream inputStream(filepath);
  string line;
  /*Read the corners*/

  this->Corner1 = this->DeserializePoint(inputStream);
  this->Corner2 = this->DeserializePoint(inputStream);
  getline(inputStream, line);

  bool readingWaypoints = false;
  while(!inputStream.eof())
  {
    char nextChar = inputStream.peek(); 

    if (nextChar == '-')
    {
      readingWaypoints = true;
      getline(inputStream, line);
    }
    else if (isprint(nextChar))
    {
      if (!readingWaypoints)
      {
        this->Obstacles.push_back(this->DeserializeObstacle(inputStream));
      }
      else
      {
        this->LastComputedPath.push_back(this->DeserializePoint(inputStream));
      }
    }
  }

  inputStream.close();
}

void SparseMap::Visualize(string filepath, string pythonpath)
{
  /*Check that we are in 2 or 3-d*/
  unsigned int dimensionality = this->Corner1.GetDimensionality();
  if (!(dimensionality == 2 || dimensionality == 3))
  {
    string errorMessage = "SparseMap error: Attempting to visualize a map of dimensionality ";
    errorMessage += to_string(dimensionality);
    errorMessage += ". Supported dimensionalities are 2, 3";
    errorMessage += "\n"; //TODO: formally replace with endl;
    throw runtime_error(errorMessage);
  }

  ofstream outputStream;
  outputStream.open(filepath);

  /*Write out the boundaries of the map in black lines*/
  unsigned int firstEndPoint;
  unsigned int secondEndPoint;
  unsigned int terminationPoint = (unsigned int)pow(2, dimensionality);
  for (firstEndPoint = 0; firstEndPoint < terminationPoint; firstEndPoint++)
  {
    for (secondEndPoint = firstEndPoint+1; secondEndPoint < terminationPoint; secondEndPoint++)
    {
      /*If the two counters don't differ by only one bit, don't draw the line*/
      unsigned int diffBitCount = 0;
      for (unsigned int i = 0; i < dimensionality; i++)
      {
        if ((firstEndPoint & (0x0001 << i)) != (secondEndPoint & 0x0001 << i))
          diffBitCount++;
      }

      if (diffBitCount == 1)
      {
        outputStream << "#000000" << endl;
        for (unsigned int i = 0; i < dimensionality; i++)
        {
          (firstEndPoint & (0x0001 << i) ?
            outputStream << to_string(this->Corner1[i])
            :
            outputStream << to_string(this->Corner2[i])
          );

          (i == (dimensionality - 1) ?
            outputStream << endl
            :
            outputStream << ","
          );
        }
        for (unsigned int i = 0; i < dimensionality; i++)
        {
          (secondEndPoint & (0x0001 << i) ?
            outputStream << to_string(this->Corner1[i])
            :
            outputStream << to_string(this->Corner2[i])
          );

          (i == (dimensionality - 1) ?
            outputStream << endl
            :
            outputStream << ","
          );
        }
        outputStream << "-" << endl;
      }
    }
  }

  /*Draw all of the obstacles as red polygons*/
  for (unsigned int obstacleIndex = 0; obstacleIndex < this->Obstacles.size(); obstacleIndex++)
  {
    vector<CSpacePoint> currentCorners = this->Obstacles[obstacleIndex]->GetCorners();
    outputStream << "#0000FF" << endl;
    for (unsigned int cornerIndex = 0; cornerIndex < currentCorners.size(); cornerIndex++)
    {
      CSpacePoint point = currentCorners[cornerIndex];
      this->SerializePoint(outputStream, point);
    }
    outputStream << "-" << endl;
  }

  /*Draw all of the waypoints as green lines*/
  /*TODO: Triangles?*/
  if (this->LastComputedPath.size() == 1)
  {
    outputStream << "#00FF00" << endl;
    this->SerializePoint(outputStream, this->LastComputedPath[0]);
  }

  for (unsigned int waypointIndex = 0; waypointIndex < this->LastComputedPath.size()-1; waypointIndex++)
  {
    outputStream << "#00FF00" << endl;
    this->SerializePoint(outputStream, this->LastComputedPath[waypointIndex]);
    this->SerializePoint(outputStream, this->LastComputedPath[waypointIndex+1]);
    outputStream << "-" << endl;
  }

  outputStream.close();

  /*If a python path is provided, attempt to call the plotter*/
  if (pythonpath.length() > 0)
  {
    string cmd = "python ";
    cmd += pythonpath;
    cmd += " ";
    cmd += filepath;
    system(cmd.c_str());
  }
}

void SparseMap::SerializePoint(ofstream& stream, CSpacePoint point)
{
  /*Write the point in the following format:
   * dim1,dim2,dim3...,dimN
   */
  int dimensionality = point.GetDimensionality();
  for (int i = 0; i < dimensionality; i++)
  {
    stream << point[i];
    if (i != dimensionality - 1) //TODO: does this break deserialization?
      stream << ",";
  }
  stream << endl;
  return;
}

CSpacePoint SparseMap::DeserializePoint(ifstream& stream)
{
  /*Read point written in the form 
   * dim1,dim2,dim3...,dimN
   */
  vector<MapPrecisionType> pointValues;
  string line;
  getline(stream, line);
  istringstream stringStream(line);
  string token;
  MapPrecisionType tokenValue;
  while(getline(stringStream, token, ',')) //TODO: This feels ugly.
  {
    istringstream(token) >> tokenValue;
    pointValues.push_back(tokenValue);
  }

  CSpacePoint point(pointValues);
  return point;
}

void SparseMap::SerializeObstacle(ofstream& stream, IObstacle *obstacle)
{
  /*Serializes the obstacle like 
   * <Number of corners>
   * Corner1
   * Corner2
   * ...
   * CornerN
   * Severity
   * <blank line>
   */
  vector<CSpacePoint> points = obstacle->GetCorners();
  stream << points.size() << endl;
  
  for (unsigned int i = 0; i < points.size(); i++)
  {
    this->SerializePoint(stream, points[i]);
  }
  stream << obstacle->GetSeverity();
  stream << endl;
  return;
}

IObstacle* SparseMap::DeserializeObstacle(ifstream& stream)
{
  /*Reads in an obstacle serialized like
   * <Number of corners>
   * Corner1
   * Corner2
   * ...
   * CornerN
   * Severity
   * <Blank line>
   */
  string line;
  getline(stream, line);
  int numberOfCorners = stoi(line);

  vector<CSpacePoint> corners;

  for (int i = 0; i < numberOfCorners; i++)
  {
    corners.push_back(this->DeserializePoint(stream));  
  }

  getline(stream, line);
  int severity = stoi(line);

  IObstacle* newObstacle = (IObstacle*)new Obstacle(corners, severity);
  return newObstacle;
}
