#pragma once

#include <osg/ComputeBoundsVisitor>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/X11/GraphicsWindowX11>
#include <osgGA/GUIEventAdapter>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgFX/Scribe>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgUtil/Optimizer>
#include <osg/StateSet>
#include <osg/ShapeDrawable>
#include <vector>

#include <iostream>

using namespace std;

class OSG
{
public:
  OSG();
  ~OSG();

public:
  void run();
  void GetAttrInfo(osg::ref_ptr<osgViewer::Viewer> viewer);

public:
  int element_count;

};


