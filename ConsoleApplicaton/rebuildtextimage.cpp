#include <rebuildtextimage.h>
#include <fstream>

RebuildTextImage::RebuildTextImage()
{
//  cout<<"i am back"<<endl;

}

RebuildTextImage::~RebuildTextImage()
{

//  delete Graph;

}

void RebuildTextImage::Excute(cv::Mat GrayImage, string binary_file_path)
{
//  file_path = binary_file_path;
  ReadBinaryFile(binary_file_path);
  GRAPH *Graph = (GRAPH*)malloc(sizeof(GRAPH));
//  RebuildTextImage();
  Init(GrayImage,Graph);
  StartConstructing(Graph);
}

void RebuildTextImage::Init(cv::Mat GrayImage, GRAPH *Graph)\
{
  Graph->rows_count = GrayImage.rows;
  Graph->cols_count = GrayImage.cols;
  int pixels_count = 0;
  for(int row_index = 0;row_index < GrayImage.rows;row_index ++)
    {
      for(int col_index = 0;col_index < GrayImage.cols;col_index ++)
        {
          if(GrayImage.at<cv::Vec3b>(row_index,col_index)[0] != 0)
            {
              pixels_count ++;
            }

        }

    }
  Graph->pixels_count = pixels_count;
  Graph->row_head = (PIXEL_NODE*)malloc(GrayImage.rows*sizeof(PIXEL_NODE));
  Graph->col_head = (PIXEL_NODE*)malloc(GrayImage.cols*sizeof(pixels_count));
  if(!Graph->row_head || !Graph->col_head)
    {
      return;
    }
}

void RebuildTextImage::GetSpatialCoordiante(cv::Mat Image)
{

}

void RebuildTextImage::StartConstructing(GRAPH *Graph)
{

}

void RebuildTextImage::ReadBinaryFile(string file_path)
{
  ifstream infile(file_path.c_str(),ios::binary);
  if(infile)
    {
      std::vector<PIXEL_INFO*> Graph_info;
      infile.read((char*)&Graph_info,sizeof(Graph_info));
      infile.close();
    }

}
