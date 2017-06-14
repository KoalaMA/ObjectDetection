#include <osg/Geometry>
#include <drawpoints.h>

osg::Drawable *DrawPoints::DrawPoint(float s_point_x, float s_point_y, float s_point_z, int R, int G, int B)
{
  osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

  //vertices
  osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
  vertices->push_back(osg::Vec3(s_point_x,s_point_y,s_point_z));

  //normals
//  osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
//  normals->push_back(osg::Vec3(0.,1.,0.));

  //color
  osg::ref_ptr<osg::Vec3Array> colors = new osg::Vec3Array;
  colors->push_back(osg::Vec3(R,G,B));

  geom->setVertexArray(vertices.get());
//  geom->setNormalArray(normals.get());
//  geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
  geom->setColorArray(colors.get());
  geom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::POINTS,0,1));
  return geom.release();

}
