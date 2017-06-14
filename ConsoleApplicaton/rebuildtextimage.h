#ifndef REBUILDTEXTIMAGE_H
#define REBUILDTEXTIMAGE_H

#endif // REBUILDTEXTIMAGE_H
#include <osg/PolygonMode>
#include <opencv2/opencv.hpp>

using namespace std;

struct PIXEL_INFO
{
  PIXEL_INFO();
  ~PIXEL_INFO();
public:
  int pixel_count;
  float texture_x;
  float texture_y;
  float spatial_x;
  float spatial_y;
  float spatial_z;
  int pixel_value;
};

struct PIXEL_NODE
{
public:
  PIXEL_NODE();
  ~PIXEL_NODE();
public:
  int row_index,col_index,value;//value
  osg::Vec2d t_coordinate;//2 dimensions coordinate
  osg::Vec3f s_coordinate;//3 dimensions coordiante
  struct PIXEL_NODE *left_NEi,*top_Nei,*rigth_Nei,*down_Nei;
};

struct GRAPH
{
public:
  GRAPH();
  ~GRAPH();
public:
  PIXEL_NODE *row_head,*col_head;
  int rows_count,cols_count,pixels_count;
};

class RebuildTextImage
{

private:
  std::vector<PIXEL_INFO*> Graph_info;
  string file_path;

public:
  RebuildTextImage();
  ~RebuildTextImage();

  void Excute(cv::Mat GrayImage,string binary_file_path);
  void Init(cv::Mat GrayImage,GRAPH *Graph);
  void GetSpatialCoordiante(cv::Mat Image);
  void StartConstructing(GRAPH *Graph);
  void ReadBinaryFile(string file_path);



};

