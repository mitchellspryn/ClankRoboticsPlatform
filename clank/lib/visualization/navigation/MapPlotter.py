import mpl_toolkits.mplot3d as a3
from matplotlib.collections import PolyCollection
from matplotlib.colors import colorConverter
from scipy.spatial import ConvexHull
from matplotlib.collections import LineCollection
import matplotlib.pyplot as plt
import numpy as np
import pylab as pl
import sys
import copy

class PlotItem:
  def __init__(self):
    self.color = '#000000'
    self.verticies = []

def ReadFile(fileName):
  itemsToPlot = []
  with open(fileName, 'r') as f:
    line = f.readline().strip()
    while(line != ""):
      #Read in the color information
      currentItem = PlotItem()
      currentItem.color = line

      #Read in the verticies.
      line = f.readline().strip()
      verts = []
      while(line != "-" and line != ""):
        currentPoint = []
        tokens = line.split(",")
        if (len(tokens) != 2 and len(tokens) != 3):
          print("Error: incorrect dimensionality on line {0}".format(line))
          raise ValueError
        for i in xrange(0, len(tokens), 1):
          try:
            currentPoint.append(float(tokens[i]))
          except:
            print("Error: expected integer in position {0} one line {1}.".format(i, line))
            raise ValueError
        if (len(currentPoint) == 2):
          currentPoint.append(0.0)
        if (len(currentPoint) != 3):
          print("Error: Incorrect number of dimensions read from line {0} (read {1} dimensions)".format(line, len(currentPoint)))
          raise AssertionError
        verts.append(currentPoint)
        line = f.readline().strip()

      if (len(verts) == 0):
        print("Error: item found with no verticies.")
        raise ValueError
      currentItem.verticies = verts
      itemsToPlot.append(currentItem)
      if (line != ""):
        line = f.readline().strip()

  return itemsToPlot

def MakeTriangleCollection(verts, color):
  verts_final = []
  if (len(verts[0]) == 2):
    for i in xrange(0, len(verts), 1):
      verts_next = verts[i]
      verts_next.append(0)
      verts_final.append(verts_next)
  else:
    verts_final = verts
      
  triCollection = a3.art3d.Poly3DCollection([verts_final])
  triCollection.set_color(color)
  return triCollection

def AddHighlightedPoint(item, axis):
  print("Error: Single point plotting not implemented")
  raise NotImplementedError
  
def AddLine(item, axis):
  x = [item.verticies[0][0], item.verticies[1][0]]
  y = [item.verticies[0][1], item.verticies[1][1]]
  z = [item.verticies[0][2], item.verticies[1][2]]
  axis.plot(xs = x, ys = y, zs = z, color = item.color)

def AddTriangle(item, axis):
  triCollection = MakeTriangleCollection(item.verticies, item.color)
  axis.add_collection3d(triCollection)

def AddPolygon(item, axis):
  #If the item is in 2-d, remove the third dimension 
  points = []
  is2d = True
  for i in xrange(0, len(item.verticies), 1):
    if (item.verticies[i][2] != 0):
        is2d = False 
        break
  if (is2d):
    points = []
    for i in xrange(0, len(item.verticies), 1):
        currentPoint = []
        currentPoint.append(item.verticies[i][0])
        currentPoint.append(item.verticies[i][1])
        currentPoint.append(.0001)
        points.append(copy.deepcopy(currentPoint))
        currentPoint[2] = 0
        points.append(copy.deepcopy(currentPoint))
  else:
    points = item.verticies

  print(points)
    
  #Get the convex hull
  hull = ConvexHull(points)

  #For each simplex (triangular face), add a face patch
  for simplex in hull.simplices:
    triangles = []
    for i in simplex:
      triangles.append(points[i])
    triCollection = MakeTriangleCollection(triangles, item.color)
    axis.add_collection3d(triCollection)

def GenerateAndShowPlot(items):
  fig = plt.figure()
  ax3d = a3.Axes3D(fig)

  maxX = -9999
  maxY = -9999
  maxZ = -9999
  minX = 9999
  minY = 9999
  minZ = 9999

  for plotItem in items:
    #Update bounds of plot
    maxX = max(maxX, max(v[0] + 0.1*abs(v[0]) for v in plotItem.verticies))
    maxY = max(maxY, max(v[1] + 0.1*abs(v[1]) for v in plotItem.verticies))
    maxZ = max(maxZ, max(v[2] + 0.1*abs(v[2]) for v in plotItem.verticies))
    
    minX = min(minX, min(v[0] - 0.1*abs(v[0]) for v in plotItem.verticies))
    minY = min(minY, min(v[1] - 0.1*abs(v[1]) for v in plotItem.verticies))
    minZ = min(minZ, min(v[2] - 0.1*abs(v[2]) for v in plotItem.verticies))


    #Ensure that we have something to plot
    if (len(plotItem.verticies) == 0):
      print("Error: cannot plot plotItem with no verticies.")
      raise ValueError
    #If there is one vertex, then we are plotting a hilighted point
    elif (len(plotItem.verticies) == 1):
      AddHighlightedPoint(plotItem, ax3d)
    #If there are two verticies, then we are plotting a line 
    elif (len(plotItem.verticies) == 2):
      print("Adding line...")
      AddLine(plotItem, ax3d)
    #If there are three verticies, then we are plotting a triangle
    elif (len(plotItem.verticies) == 3):
      print("Adding triangle...")
      AddTriangle(plotItem, ax3d)
    #If there are more than three verticies, then we are plotting an arbitrary polygon.
    #Get the convex hull and plot that
    else:
      print("Adding polygon...")
      AddPolygon(plotItem, ax3d)

  #Show the plot
  ax3d.set_xlim3d(minX, maxX)
  ax3d.set_ylim3d(minY, maxY)
  ax3d.set_zlim3d(minZ, maxZ)
  pl.show()

def main():
  if (len(sys.argv) != 2):
    print("Usage: python MapPlotter.py <InputFile>")
    print("\t<InputFile>: The file containing the data to plot")
    return

  print("Reading data from {0}...".format(sys.argv[1]))
  items = ReadFile(sys.argv[1])

  print("Done! Plotting...")
  GenerateAndShowPlot(items)

main()
