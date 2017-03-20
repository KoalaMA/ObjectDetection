#ifndef TEXTURETOVERTEX_H
#define TEXTURETOVERTEX_H

#endif // TEXTURETOVERTEX_H

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/TriangleFunctor>
#include <osg/TriangleIndexFunctor>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgUtil/Optimizer>

#include <osg/Notify>

#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

#include <datamanagement.h>
#include <drawsides.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace std;

class IndexToTextureCoord
{
public:
    float getRowTexCoord(int col_index, int S);//column为像元列号，S为总列数，返回纹理s坐标
    float getColuTexCoord(int row_index,int T); //row为像元行号，T为总行数，返回纹理y坐标
};

class MatrixInverse   //计算给定矩阵的逆矩阵
{
public:
    bool GetMatrixInverse(float src[3][3],int n,float des[3][3]);   //得到给定矩阵src的逆矩阵保存到des中
    float getA(float arcs[3][3], int n);  //按第一行展开计算|A|
    void getAStart(float arcs[3][3],int n,float ans[3][3]);   //计算每一行每一列的每个元素所对应的余子式，组成A*

};

//遍历三角面片获取顶点索引
class TriangleIndexVisitor
{
public:
    int count;
    int temp_id;
    bool find_map_matrix;
    int pixel_count;

    string matrixes_table_name;
    string lines_table_name;

//    cv::Mat GrayImage;
    //vector<int> aryindex;//三角面片顶点索引数组
    osg::Array* arytexcoord;
    osg::Array* aryvercoord;
    osg::Image* texImg_;
    IndexToTextureCoord textureCoord;
    cv::Mat GrayImage;
    std::vector<osg::Vec3> point_set;

    MYSQL_FIELD *field;
    MYSQL_RES* result;
    MYSQL_ROW row;

    MatrixInverse* matrixInverse;

    float raw_matrix[3][3],inversed_matrix[3][3];

    CMyDBAdd mysql_add;

    SidesManage* sideManage;

public:
    TriangleIndexVisitor();

    ~TriangleIndexVisitor();

    void CreateTable();

    void operator()(const int index1, const int index2, const int index3);

    void MapToSpatialCoordinate();

    void MapMatrix(const int index1, const int index2, const int index3);

    void CreateBinaryImage();

    string ConvertToString(float Num);

    void ClearAllTheRecords();

};



class FindGeometryVisitor:public osg::NodeVisitor
{
public:
    void apply(osg::Group& group);
    void getImage(osg::Geode* geode);
    string GetTableName(osg::Geode* geode);
    string CutTheTail(string file_path);

    void detectsides();

public:
    osg::TriangleIndexFunctor<TriangleIndexVisitor> triIndex;

};


