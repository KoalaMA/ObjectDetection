#ifndef GETTEX_H
#define GETTEX_H

#endif // GETTEX_H

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/TriangleFunctor>
#include <osg/TriangleIndexFunctor>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgUtil/Optimizer>

#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

using namespace std;

class GetTexture:public osg::NodeVisitor
{
public:
    void getImage(osg::Geode& geode);

public://成员变量，贴图
    osg::Image* teximg;
};

struct TrianglePrinter_
{
    osg::Vec3 point_;
    std::vector<osg::Vec3> v1_;
    std::vector<osg::Vec3> v2_;
    std::vector<osg::Vec3> v3_;

    int count;
    int tricount;
    TrianglePrinter_()
    {
        point_ = osg::Vec3(0.0f,0.0f,0.0f);   //初始化默认起始点
        count = 0;
    }

    //通过交点寻找对应的三角面片，同时给成员v1_,v2_,v3_赋值
    /*
            遍历所有的三角面片，通过计算三角面片的三个顶点与交点的连线的夹角之和，
            来判断交点在哪个三角面片内(或者找到交点的一个由三角面片组成的小邻域)，
            将所得三角面片的三个顶点赋值到v1_,v2_,v3_中；
            */
    void operator()(const osg::Vec3 &v1, const osg::Vec3 &v2, const osg::Vec3 &v3, bool);
};



//纹理坐标的获取和颜色值的获取
class FindGeometryVisitorm:public osg::NodeVisitor
{
public:
    //FIndGeometryVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN);

    int index1;   //第一个顶点索引
    int index2;   //第二个顶点索引
    int index3;   //第三个顶点索引
    osg::Image* teximage;
    void loadpoint(osg::Vec3 &point);
    void outvertex(std::vector<osg::Vec3> &v1, std::vector<osg::Vec3> &v2, std::vector<osg::Vec3> &v3);    //返回顶点
    void loadindices(int indices1, int indices2, int indices3);
    void loadteximage(osg::Image* image);

    void apply(osg::Group &group);

protected:
    osg::TriangleFunctor<TrianglePrinter_> triPrinter;
};

//事件处理类
class EventHandler : public osgGA::GUIEventHandler
{
public:
    EventHandler(); //osg::Image*image
    void addTriangles(osg::Group *root, std::vector<osg::Vec3> &v1, std::vector<osg::Vec3> &v2, std::vector<osg::Vec3> &v3);
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
protected:
        osg::Vec3 point_;			//交点
        std::vector<osg::Vec3> v1_;	//三角面片的第一个顶点
        std::vector<osg::Vec3> v2_;	//三角面片的第二个顶点
        std::vector<osg::Vec3> v3_;	//三角面片的第三个顶点
        FindGeometryVisitorm fgv;	//结点访问器(用于访问三角面片)
        osg::TriangleFunctor<TrianglePrinter_> triPrinter;
        GetTexture teximage;        //纹理图片
public:
        int indices1; //三角面片第一个顶点索引
        int indices2; //三角面片第二个顶点索引
        int indices3; // 三角面片第三个顶点索引
        osg::Image* texImage;

};























