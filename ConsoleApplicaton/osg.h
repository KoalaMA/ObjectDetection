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
#include <drawpoints.h>
#include <datamanagement.h>
//#include <rebuildtextimage.h>


using namespace std;

struct PIXEL_INFO
{
  PIXEL_INFO();
  ~PIXEL_INFO();
public:
  int pixel_count;
  float spatial_x;
  float spatial_y;
  float spatial_z;
  int R;
  int G;
  int B;

};


class OSG
{
public:
  OSG();
  ~OSG();

public:
  void run();

  void GetAttrInfo(osg::ref_ptr<osgViewer::Viewer> viewer);

  osg::ref_ptr<osg::Geode> DrawSpatialPoints();

  osg::ref_ptr<osg::Geode> DrawSpatialTriangles();

  void ReadModelNames(CMyDBAdd &database, vector<string> & _model_name);

  osg::ref_ptr<osg::Geode> CreateGemotry(CMyDBAdd &database, vector<string> & _model_name);

public:
  int element_count;
//  osg::ref_ptr<osg::Geode> geode;
};

