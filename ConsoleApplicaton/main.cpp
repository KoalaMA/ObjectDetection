#include <osg.h>

int main()
{
  OSG* osgObject;
  osgObject->run();
  return 0 ;
}

//#include "texturetovertex.h"
//int main()
//{
//    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
//    osg::ref_ptr<osg::Group> root = new osg::Group();
//    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("/home/shilei/Data/Tile_+000_+000/Tile_+000_+000_L16_0.osgb");

//    FindGeometryVisitor fgev;
//    //node->accept(fgev);

//    root->addChild(node.get());
//    root->accept(fgev);
//    osgUtil::Optimizer optimizer;
//    optimizer.optimize(root.get());
//    viewer->setSceneData(root.get());

//    //添加状态事件
//    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
//    //窗口大小变化事件
//    viewer->addEventHandler(new osgViewer::WindowSizeHandler);
//    //添加一些常用状态设置
//    viewer->addEventHandler(new osgViewer::StatsHandler);

//    viewer->realize();
//    viewer->run();

//    return 0;
//}

