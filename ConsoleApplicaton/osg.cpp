#include <osg.h>
#include <document.h>
//#include <gettex.h>
#include <texturetovertex.h>


OSG::OSG()
{
    element_count = 0;
}

OSG::~OSG()
{

}

void OSG::run()
{
    osg::ref_ptr<osg::Group> group = new osg::Group();
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

    DocumentManage* documentManage = new DocumentManage;
    std::vector<TreeNode*> searchingTree;
    searchingTree = documentManage->OnFileFolderOpen();
    for(int index = 0;index < int(searchingTree.size());index++)
    {
        if(searchingTree[index]->loadable)
        {
            osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(searchingTree[index]->file_path);
            group->addChild(node.get());


            break;
            if (index >= 600)
              {
                break;
              }
        }
    }

    FindGeometryVisitor fgev;
    group->accept(fgev);

    osgUtil::Optimizer optimizer;
    optimizer.optimize(group.get());
    viewer->setSceneData(group.get());

    //添加状态事件
    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
    //gettexture
    //OSG::GetAttrInfo(viewer);
    //窗口大小变化事件
    viewer->addEventHandler(new osgViewer::WindowSizeHandler);
    //添加一些常用状态设置
    viewer->addEventHandler(new osgViewer::StatsHandler);

    //textureoperetion
//    viewer->addEventHandler(new EventHandler());

    viewer->realize();
    viewer->run();
}

//void OSG::GetAttrInfo(osg::ref_ptr<osgViewer::Viewer> viewer)
//{
//    GetTexture* textureManage = new GetTexture;
//  //  viewer->addEventHandler(new EventHandler(textureManage->teximg));
//}
