#include <texturetovertex.h>
using namespace std;

float IndexToTextureCoord::getRowTexCoord(int col_index, int S)
{
    float tx;
    tx = (float)(col_index+1)/S;
    return tx;
}

float IndexToTextureCoord::getColuTexCoord(int y_index,int T)
{
    float ty;
    ty = (float)(y_index+1)/T;
    return ty;
}

void FindGeometryVisitor::apply(osg::Group &group)
{
    for(int node_index = 0;node_index < group.getNumChildren();node_index ++)
      {
//        int s = group.getNumChildren();
        osg::Node* node = group.getChild(node_index);
        osg::Geode*  geode = node->asGeode();
        if (geode)
          {
            string file_name_table = GetTableName(geode);
            for(unsigned int i = 0; i<1;  i++ ) //
//            for(unsigned int i = 0; i<geode->getNumDrawables();  i++ ) //
            {
//                int p = geode->getNumDrawables();

                osg::Geometry* geo = dynamic_cast<osg::Geometry*>(geode->getDrawable(i));
                osg::Array* vercoord = geo->getVertexArray();
                osg::Array* texcoord = geo->getTexCoordArray(i);
                osg::Drawable* drawable = geode->getDrawable(i);


                triIndex.count = 0;
                triIndex.matrixes_table_name = file_name_table + "_matrix";
                triIndex.pixels_table_name = file_name_table + "_pixels";
                triIndex.sides_table_name = file_name_table + "_sides";
                triIndex.triangles_table_name = file_name_table + "_triangles";
//                triIndex.model_table_name = file_name_table + "_model";
                triIndex.arytexcoord = texcoord;
                triIndex.aryvercoord = vercoord;                
                triIndex.model_count = node_index;
                triIndex.CreateTable();
//                triIndex.ClearAllTheRecords();
                drawable->accept(triIndex);
            }

//            getImage(geode);//getting image behind computing convert_matrix

          }
//        break;

      }

}

void FindGeometryVisitor::getImage(osg::Geode *geode)
{
//  osg::Image* texImg = NULL;
//  for(unsigned int i = 0; i<1;  i++ ) //
  int drawable_count = geode->getNumDrawables();
  for(unsigned int i=0;i<geode->getNumDrawables();i++)
  {
      osg::StateSet* state = geode->getDrawable(i)->getStateSet();
      osg::StateSet::TextureAttributeList& texAttribList = state->getTextureAttributeList();
      for(unsigned int j=0;j<texAttribList.size();j++)
      {
          int p = texAttribList.size();
          osg::Texture2D* tex2D = NULL;
          if(tex2D = dynamic_cast<osg::Texture2D*>(state->getTextureAttribute(j,osg::StateAttribute::TEXTURE)))
          {
              if(!tex2D->getImage())
                {
                  cout<<"let me know it"<<endl;
                }
              osg::Image* texImg = tex2D->getImage();

              cout<<"FileName:"<<texImg->getFileName()<<endl;
              triIndex.texImg_ = texImg;
              triIndex.CreateBinaryImage();
              free(texImg);
          }
      }

    }
}

string FindGeometryVisitor::GetTableName(osg::Geode *geode)
{
  osg::StateSet* state = geode->getDrawable(0)->getStateSet();
//  osg::StateSet::TextureAttributeList& texAttribList = state->getTextureAttributeList();
  osg::Texture2D* tex2D = NULL;
  string table_name;
  if(tex2D = dynamic_cast<osg::Texture2D*>(state->getTextureAttribute(00,osg::StateAttribute::TEXTURE)))
   {
      osg::Image* texImg = tex2D->getImage();
      string file_name = texImg->getFileName();
      table_name = CutTheTail(file_name);
    }
  return table_name;
}

string FindGeometryVisitor::CutTheTail(string file_name)//"_0.jpg"
{
  string new_name = file_name.substr(0,file_name.length() - 6);
  string::iterator it;
  for(it =new_name.begin(); it != new_name.end(); ++it)
    {
      if ( *it == '+')
          {
              new_name.erase(it);
          }
    }

  return new_name;
}

TriangleIndexVisitor::TriangleIndexVisitor()
{  
  BaseVector.x() = 0;
  BaseVector.y() = 0;
  BaseVector.z() = 1;
  mysql_add.initDB("localhost" , "root", "shilei744534" , "convert_matrix" );
  temp_id = 0;
  pixel_count = 0;
  count = 0;
  triangle_count = 0;
}

TriangleIndexVisitor::~TriangleIndexVisitor()
{
  count = 0;
//    ClearAllTheRecords();

}

void TriangleIndexVisitor::CreateTable()
{

    string sql_create_table_matrix = "create table "+matrixes_table_name+"   (id int(11) primary key,t_point1_x float,t_point1_y float,t_point2_x float,t_point2_y float,t_point3_x float,t_point3_y"
                                                                         " float,s_point1_x float,s_point1_y float,s_point1_z float,s_point2_x float,s_point2_y float,s_point2_z float,s_point3_x float,"
                                                                         "s_point3_y float,s_point3_z float,a11 float,a12 float,a13 float,a21 float,a22 float,a23 float,a31 float,a32 float,a33 float);";
    mysql_query(mysql_add.connection,sql_create_table_matrix.c_str());
    string sql_create_table_pixels = "create table "+pixels_table_name+"   (id int(11) primary key,t_point_x float,t_point_y float,s_point_x float,s_point_y float,s_point_z float,R int,G int,B int);";
    mysql_query(mysql_add.connection,sql_create_table_pixels.c_str());
    string sql_create_table_sides = "create table "+sides_table_name+"   (id int(11) primary key,t_point_x float,t_point_y float,s_point_x float,s_point_y float,s_point_z float,binary_pixel_value int);";
    mysql_query(mysql_add.connection,sql_create_table_sides.c_str());
    string sql_create_table_triangles = "create table "+triangles_table_name+"   (id int(11) primary key,point1_x float,point1_y float,point1_z float,point2_x float,point2_y float,point2_z float,point3_x "
                                                                             "float,point3_y float,point3_z float,class int);";
    mysql_query(mysql_add.connection,sql_create_table_triangles.c_str());

    string sql_insert_model_table = "insert into model_name values('"+ConvertToString(model_count)+"','"+triangles_table_name.c_str() +"');";
    mysql_query(mysql_add.connection,sql_insert_model_table.c_str());


}

void TriangleIndexVisitor::operator ()(const int index1, const int index2, const int index3)
{
  triangleClassification(index1,index2,index3);
//  MapMatrix(index1,index2,index3);//calculate mam matrix
}

void TriangleIndexVisitor::triangleClassification(const int index1, const int index2, const int index3)
{
  triangle_count ++;
  int classification=0;
  osg::Vec3f point1,point2,point3;
  point1.x() = ((float*)(aryvercoord->getDataPointer()))[3*index1];
  point2.x() = ((float*)(aryvercoord->getDataPointer()))[3*index2];
  point3.x() = ((float*)(aryvercoord->getDataPointer()))[3*index3];
  point1.y() = ((float*)(aryvercoord->getDataPointer()))[3*index1+1];
  point2.y() = ((float*)(aryvercoord->getDataPointer()))[3*index2+1];
  point3.y() = ((float*)(aryvercoord->getDataPointer()))[3*index3+1];
  point1.z() = ((float*)(aryvercoord->getDataPointer()))[3*index1+2];
  point2.z() = ((float*)(aryvercoord->getDataPointer()))[3*index2+2];
  point3.z() = ((float*)(aryvercoord->getDataPointer()))[3*index3+2];

   JudgedByNormalVector(point1,point2,point3);

  if((point1.z() + point2.z() + point3.z())/3 > -71)
    {
      classification = 1;
    }


  string sql = "insert into "+triangles_table_name+" values('"+ConvertToString(triangle_count)+
      "','"+ConvertToString(point1.x()) +"','"+ConvertToString(point1.y())+
      "','"+ConvertToString(point1.z())+"','"+ConvertToString(point2.x())+
      "','"+ConvertToString(point2.y())+"','"+ConvertToString(point2.z())+
      "','"+ConvertToString(point3.x())+"','"+ConvertToString(point3.y())+
      "','"+ConvertToString(point3.z())+"','"+ConvertToString(classification)+"');";
  mysql_query(mysql_add.connection,sql.c_str());//insert matrix into table

}

bool TriangleIndexVisitor::JudgedByNormalVector(osg::Vec3f point1, osg::Vec3f point2, osg::Vec3f point3)
{
  osg::Vec3f normal_vector;
  normal_vector.x() = (point2.y()-point1.y())*(point3.z()-point1.z())-(point2.z()-point1.z())*(point3.y()-point1.y());
  normal_vector.y() = (point2.z()-point1.z())*(point3.x()-point1.x())-(point2.x()-point1.x())*(point3.z()-point1.z());
  normal_vector.z() = (point2.x()-point1.x())*(point3.y()-point1.y())-(point2.y()-point1.y())*(point3.x()-point1.x());

  return true;
}

void TriangleIndexVisitor::MapMatrix(const int index1, const int index2, const int index3)
{  
  text_matrix[0][0] = ((float*)(arytexcoord->getDataPointer()))[2*index1];   //第一个顶点纹理坐标的第一个元素值
  text_matrix[0][1] = ((float*)(arytexcoord->getDataPointer()))[2*index2];   //第二个顶点纹理坐标的第一个元素值
  text_matrix[0][2] = ((float*)(arytexcoord->getDataPointer()))[2*index3];   //第三个顶点纹理坐标的第一个元素值

  text_matrix[1][0] = ((float*)(arytexcoord->getDataPointer()))[2*index1+1]; //第一个顶点纹理坐标的第二个元素值
  text_matrix[1][1] = ((float*)(arytexcoord->getDataPointer()))[2*index2+1]; //第二个顶点纹理坐标的第二个元素值
  text_matrix[1][2] = ((float*)(arytexcoord->getDataPointer()))[2*index3+1]; //第三个顶点纹理坐标的第二个元素值
  text_matrix[2][0] = 1 ;
  text_matrix[2][1] = 1 ;
  text_matrix[2][2] = 1 ;
  matrixComputation->GetMatrixInverse(text_matrix,3,inversed_text_matrix);//get inversed,and now, we have the map matrix,and we could calculate its 3 dimension coordinata

  spatial_matrix[0][0] = ((float*)(aryvercoord->getDataPointer()))[3*index1];
  spatial_matrix[0][1] = ((float*)(aryvercoord->getDataPointer()))[3*index2];
  spatial_matrix[0][2] = ((float*)(aryvercoord->getDataPointer()))[3*index3];
  spatial_matrix[1][0] = ((float*)(aryvercoord->getDataPointer()))[3*index1+1];
  spatial_matrix[1][1] = ((float*)(aryvercoord->getDataPointer()))[3*index2+1];
  spatial_matrix[1][2] = ((float*)(aryvercoord->getDataPointer()))[3*index3+1];
  spatial_matrix[2][0] = ((float*)(aryvercoord->getDataPointer()))[3*index1+2];
  spatial_matrix[2][1] = ((float*)(aryvercoord->getDataPointer()))[3*index2+2];
  spatial_matrix[2][2] = ((float*)(aryvercoord->getDataPointer()))[3*index3+2];

  matrixComputation->MatrixMutipule(spatial_matrix,inversed_text_matrix,map_matrix);//get computation,and now, we have the map matrix,and we could calculate its 3 dimension coordinata
//  write to the table
  string sql = "insert into "+matrixes_table_name+" values('"+ConvertToString(count)+"','"+ConvertToString(text_matrix[0][0]) +"','"+ConvertToString(text_matrix[1][0])+"','"
      +ConvertToString(text_matrix[0][1])+"','"+ConvertToString(text_matrix[1][1])+"','"+ConvertToString(text_matrix[0][2])+"','"+ConvertToString(text_matrix[1][2])+"','"
      +ConvertToString(spatial_matrix[0][0])+"','"+ConvertToString(spatial_matrix[1][0])+"','"+ConvertToString(spatial_matrix[2][0])+"','"+ConvertToString(spatial_matrix[0][1])+"','"
      +ConvertToString(spatial_matrix[1][1])+"','"+ConvertToString(spatial_matrix[2][1])+"','"+ConvertToString(spatial_matrix[0][2])+"','"+ConvertToString(spatial_matrix[1][2])+"','"
      +ConvertToString(spatial_matrix[2][2])+"','"+ConvertToString(map_matrix[0][0])+"','"+ConvertToString(map_matrix[0][1])+"','"+ConvertToString(map_matrix[0][2])+"','"
      +ConvertToString(map_matrix[1][0])+"','"+ConvertToString(map_matrix[1][1])+"','"+ConvertToString(map_matrix[1][2])+"','"+ConvertToString(map_matrix[2][0])+"','"
      +ConvertToString(map_matrix[2][1])+"','"+ConvertToString(map_matrix[2][2])+"');";
  mysql_query(mysql_add.connection,sql.c_str());//insert matrix into table
  count++;
}

void TriangleIndexVisitor::CreateBinaryImage()
{
  cv::Mat TextImage(texImg_->s(),texImg_->t(),CV_8UC3);
  for(int wide_index = 0 ;wide_index < texImg_->t() ; wide_index++)// rows
    {
      for(int height_index = 0 ;height_index < texImg_->s() ; height_index++) //cols
        {
          osg::Vec4 pixel_info = texImg_->getColor(wide_index,height_index,0);

//          int A = TextImage.rows;
//          int B = -TextImage.cols;

//          int x = wide_index - 2*A*(A*wide_index + B*height_index)/(A*A + B*B);
//          int y = height_index - 2*B*(A*wide_index + B*height_index)/(A*A + B*B);

//          TextImage.at<cv::Vec3b>(x,y)[0] = floor(pixel_info[0]*255);
//          TextImage.at<cv::Vec3b>(x,y)[1] = floor(pixel_info[1]*255);
//          TextImage.at<cv::Vec3b>(x,y)[2] = floor(pixel_info[2]*255);


          TextImage.at<cv::Vec3b>(wide_index,height_index)[0] = floor(pixel_info[0]*255);
          TextImage.at<cv::Vec3b>(wide_index,height_index)[1] = floor(pixel_info[1]*255);
          TextImage.at<cv::Vec3b>(wide_index,height_index)[2] = floor(pixel_info[2]*255);

        }
    }

  ColorfulImage = TextImage.clone();
  cvtColor(TextImage,GrayImage,CV_RGB2GRAY);
  Canny(GrayImage,BinaryImage,80,50,3,true);

  BinaryImage = GrayImage.clone();

//  imshow("BinaryImage",TextImage);
//  waitKey(0);
//  rebuild_textImage->Excute(GrayImage);

  DrawTriangles();

  MapToSpatialCoordinate();


//  WriteIntoBinaryFile();

//  TestFunction();
//  rebuild_textImage->Excute(GrayImage,binary_file_path);

}

void TriangleIndexVisitor::DrawSides()
{
  string sql_sellect_all_record = "select * from "+matrixes_table_name+";";
  int p = mysql_query(mysql_add.connection,sql_sellect_all_record.c_str());

  if(mysql_query(mysql_add.connection,sql_sellect_all_record.c_str()))
    {
      result = mysql_store_result(mysql_add.connection);
      bool traverse_finished = false;
      while (!traverse_finished) {
          row = mysql_fetch_row(result);

          //locate the triangle region
          int p1_x = atof(row[1])*BinaryImage.cols - 1;
          int p1_y = atof(row[2])*BinaryImage.rows - 1;
          int p2_x = atof(row[3])*BinaryImage.cols - 1;
          int p2_y = atof(row[4])*BinaryImage.rows - 1;
          int p3_x = atof(row[5])*BinaryImage.cols - 1;
          int p3_y = atof(row[6])*BinaryImage.rows - 1;
          int min_x = p1_x;
          int min_y = p1_y;
          int max_x = p1_x;
          int max_y = p1_y;
          if(p2_x < min_x) min_x = p2_x;
          if(p3_x < min_x) min_x = p3_x;
          if(p2_x > max_x) max_x = p2_x;
          if(p3_x > max_x) max_x = p3_x;

          if(p2_y < min_y) min_y = p2_y;
          if(p3_y < min_y) min_y = p3_y;
          if(p2_y > max_y) max_y = p2_y;
          if(p3_y > max_y) max_y = p3_y;

          for(int wide_index = min_x;wide_index < max_x;wide_index ++ )
            {
              for(int height_index = min_y;height_index < max_y;height_index ++ )
                {
                   if(BinaryImage.at<cv::Vec3b>(wide_index,height_index)[0] != 0)
                    {
                       float a11 = atof(row[16]);
                       float a12 = atof(row[17]);
                       float a13 = atof(row[18]);

                       float a21 = atof(row[19]);
                       float a22 = atof(row[20]);
                       float a23 = atof(row[21]);

                       float a31 = atof(row[22]);
                       float a32 = atof(row[23]);
                       float a33 = atof(row[24]);

                       osg::Vec2 point1,point2,point3,vec1,vec2,vec3;
                       point1.x() = atof(row[1])*BinaryImage.cols - 1;
                       point1.y() = atof(row[2])*BinaryImage.rows - 1;
                       point2.x() = atof(row[3])*BinaryImage.cols - 1;
                       point2.y() = atof(row[4])*BinaryImage.rows - 1;
                       point3.x() = atof(row[5])*BinaryImage.cols - 1;
                       point3.y() = atof(row[6])*BinaryImage.rows - 1;



                       osg::Vec2 current_pixel;
                       current_pixel.x() = wide_index;
                       current_pixel.y() = height_index;

                       vec1 = point1 - current_pixel;
                       vec1.normalize();//mormalize the vector
                       vec2 = point2 - current_pixel;
                       vec2.normalize();
                       vec3 = point3 - current_pixel;
                       vec3.normalize();

                       if (abs(acosf((vec1) * (vec2)) + acosf((vec1) * (vec3)) + acosf((vec2) * (vec3)) - 2 * osg::PI) < 1e-1)//current_pixel is located in current triangle
                         {
                           osg::Vec3 texture_pixel;
                           texture_pixel.x() = (float)(wide_index+1)/BinaryImage.cols;
                           texture_pixel.y() = (float)(height_index+1)/BinaryImage.rows;
                           texture_pixel.z() = 1;

                           float s_point_x = a11*texture_pixel.x() + a12*texture_pixel.y() + a13;
                           float s_point_y = a21*texture_pixel.x() + a22*texture_pixel.y() + a23;
                           float s_point_z = a31*texture_pixel.x() + a32*texture_pixel.y() + a33;
                           osg::Vec3f current_point;
                           current_point.x() = s_point_x;
                           current_point.y() = s_point_y;
                           current_point.z() = s_point_z;
                           int binary_pixel_value = BinaryImage.at<cv::Vec3b>(wide_index,height_index)[0];//pixel value, it is used for side detection

                 //          stop inserting record and connecting database to increase the speed
                           string sql = "insert into "+sides_table_name+" values('"+ConvertToString(pixel_count)+"','"+ConvertToString(current_pixel.x()) +"','"+ConvertToString(current_pixel.y())+"','"+ConvertToString(s_point_x)+"','"+ConvertToString(s_point_y)+"','"+ConvertToString(s_point_z)+"','"+ConvertToString(binary_pixel_value)+"');";
                           mysql_query(mysql_add.connection,sql.c_str());//insert matrix into table

                           pixel_count ++;
                 //          BinaryImage.at<cv::Vec3b>(wide_index,height_index)[0] == 0;
                         }


                    }

                }
            }

//          imshow("BinaryImage",BinaryImage);
//          cvWaitKey(0);

          if(!result->data_cursor->next)
            {
              traverse_finished = true;
            }
        }
    }

}

void TriangleIndexVisitor::FilterPixels(cv::Mat GrayImage)//filter pixels
{
  int _pixel_count = 0;
  for(int wide_index = 0;wide_index < BinaryImage.cols;wide_index ++)
    {
      for(int height_index = 0;height_index < BinaryImage.rows;height_index ++)
        {
          int temp_count = 0;
//          if(BinaryImage.at<cv::Vec3b>(wide_index,height_index)[0] == 255)//we also need to judge that if it locates in current region
          if(BinaryImage.at<cv::Vec3b>(wide_index,height_index)[0] != 0)//we also need to judge that if it locates in current region
          {
              _pixel_count++;
              if(wide_index == 0 || height_index == 0 || wide_index == BinaryImage.cols - 1 || height_index == BinaryImage.rows - 1)
                {
                  temp_count++;
                }
              else {

                      if(GrayImage.at<cv::Vec3b>(wide_index - 1,height_index -  1)[0]  == 0 && GrayImage.at<cv::Vec3b>(wide_index - 1,height_index -  1)[1]  == 0 && GrayImage.at<cv::Vec3b>(wide_index - 1,height_index -  1)[2]  == 0  )
                        {
                          temp_count++;
                        }

                      if(GrayImage.at<cv::Vec3b>(wide_index,height_index -  1)[0]  == 0 && GrayImage.at<cv::Vec3b>(wide_index,height_index -  1)[1]  == 0 && GrayImage.at<cv::Vec3b>(wide_index,height_index -  1)[2]  == 0  )
                        {
                          temp_count++;
                        }

                      if(GrayImage.at<cv::Vec3b>(wide_index + 1,height_index -  1)[0]  == 0 && GrayImage.at<cv::Vec3b>(wide_index + 1,height_index -  1)[1]  == 0 && GrayImage.at<cv::Vec3b>(wide_index + 1,height_index -  1)[2]  == 0  )
                        {
                          temp_count++;
                        }

                      if(GrayImage.at<cv::Vec3b>(wide_index - 1,height_index)[0]  == 0 && GrayImage.at<cv::Vec3b>(wide_index - 1,height_index)[1]  == 0 && GrayImage.at<cv::Vec3b>(wide_index - 1,height_index)[2]  == 0  )
                        {
                          temp_count++;
                        }

                      if(GrayImage.at<cv::Vec3b>(wide_index + 1,height_index)[0]  == 0 && GrayImage.at<cv::Vec3b>(wide_index + 1,height_index)[1]  == 0 && GrayImage.at<cv::Vec3b>(wide_index + 1,height_index)[2]  == 0  )
                        {
                          temp_count++;
                        }

                      if(GrayImage.at<cv::Vec3b>(wide_index - 1,height_index +  1)[0]  == 0 && GrayImage.at<cv::Vec3b>(wide_index - 1,height_index +  1)[1]  == 0 && GrayImage.at<cv::Vec3b>(wide_index - 1,height_index +  1)[2]  == 0  )
                        {
                          temp_count++;
                        }

                      if(GrayImage.at<cv::Vec3b>(wide_index,height_index +  1)[0]  == 0 && GrayImage.at<cv::Vec3b>(wide_index,height_index +  1)[1]  == 0 && GrayImage.at<cv::Vec3b>(wide_index,height_index +  1)[2]  == 0  )
                        {
                          temp_count++;
                        }

                      if(GrayImage.at<cv::Vec3b>(wide_index + 1,height_index +  1)[0]  == 0 && GrayImage.at<cv::Vec3b>(wide_index + 1,height_index +  1)[1]  == 0 && GrayImage.at<cv::Vec3b>(wide_index + 1,height_index +  1)[2]  == 0  )
                        {
                          temp_count++;
                        }

                }

              if(temp_count > 0)
                {
                  BinaryImage.at<cv::Vec3b>(wide_index,height_index)[0] = 0;
                }

            }
        }
    }
  imshow("BinaryImage",BinaryImage);
  cvWaitKey(0);

  cout<<"pixel_count="<<endl;

}

void TriangleIndexVisitor::TestFunction()
{
  osg::Vec2 current_pixel;
  current_pixel.x() = 800;//column
  current_pixel.y() = 900;//row
  while (temp_id < count){
      string sql = "select t_point1_x,t_point1_y,t_point2_x,t_point2_y,t_point3_x,t_point3_y,a11,a12,a13,a21,a22,a23,a31,a32,a33 from "+matrixes_table_name+" where id ="+ConvertToString(temp_id)+";";
      //visit matrix_info
      if(mysql_query(mysql_add.connection,sql.c_str()))//successful queried
        {
          result = mysql_store_result(mysql_add.connection);
          row = mysql_fetch_row(result);//it is managed as a two dimensions array;
//          field = mysql_fetch_field(result);

          float t_point1_x = atof(row[0])*BinaryImage.cols - 1;
          float t_point1_y = atof(row[1])*BinaryImage.rows - 1;
          float t_point2_x = atof(row[2])*BinaryImage.cols - 1;
          float t_point2_y = atof(row[3])*BinaryImage.rows - 1;
          float t_point3_x = atof(row[4])*BinaryImage.cols - 1;
          float t_point3_y = atof(row[5])*BinaryImage.rows - 1;

          float a11 = atof(row[6]);
          float a12 = atof(row[7]);
//          float a13 = atof(row[8]);
          float a21 = atof(row[9]);
          float a22 = atof(row[10]);
//          float a23 = atof(row[11]);
          float a31 = atof(row[12]);
          float a32 = atof(row[13]);
//          float a33 = atof(row[14]);

          osg::Vec2 point1,point2,point3,vec1,vec2,vec3;
          point1.x() = t_point1_x;
          point1.y() = t_point1_y;
          point2.x() = t_point2_x;
          point2.y() = t_point2_y;
          point3.x() = t_point3_x;
          point3.y() = t_point3_y;

          vec1 = point1 - current_pixel;
          vec1.normalize();//mormalize the vector
          vec2 = point2 - current_pixel;
          vec2.normalize();
          vec3 = point3 - current_pixel;
          vec3.normalize();

          float value = abs(acosf((vec1) * (vec2)) + acosf((vec1) * (vec3)) + acosf((vec2) * (vec3)) - 2 * osg::PI) ;
          if (abs(acosf((vec1) * (vec2)) + acosf((vec1) * (vec3)) + acosf((vec2) * (vec3)) - 2 * osg::PI) < 1e-2)//current_pixel is located in current triangle
            {
              cout<<"let me know it"<<endl;

              osg::Vec2 texture_pixel;
              // convert to texture coordinates
              texture_pixel.x() = (float)(800+1)/BinaryImage.cols;
              texture_pixel.y() = (float)(900+1)/BinaryImage.rows;

              float s_point_x = a11*texture_pixel.x() + a12*texture_pixel.y();
              float s_point_y = a21*texture_pixel.x() + a22*texture_pixel.y();
              float s_point_z = a31*texture_pixel.x() + a32*texture_pixel.y();

              string sql = "insert into "+pixels_table_name+" values('"+ConvertToString(pixel_count)+"','"+ConvertToString(current_pixel.x()) +"','"+ConvertToString(current_pixel.y())+"','"+ConvertToString(s_point_x)+"','"+ConvertToString(s_point_y)+"','"+ConvertToString(s_point_z)+"');";
              mysql_query(mysql_add.connection,sql.c_str());//insert matrix into table

              temp_id++;
            }
          else {
              temp_id++;
            }

        }


    }


}

void TriangleIndexVisitor::MapToSpatialCoordinate()
{
  string sql_sellect_all_record = "select * from "+matrixes_table_name+";";
  int p = mysql_query(mysql_add.connection,sql_sellect_all_record.c_str());
  if(mysql_query(mysql_add.connection,sql_sellect_all_record.c_str()))
    {
      result = mysql_store_result(mysql_add.connection);
      bool traverse_finished = false;
      while (!traverse_finished) {
          row = mysql_fetch_row(result);

          //locate the triangle region
          int p1_x = atof(row[1])*BinaryImage.cols - 1;
          int p1_y = atof(row[2])*BinaryImage.rows - 1;
          int p2_x = atof(row[3])*BinaryImage.cols - 1;
          int p2_y = atof(row[4])*BinaryImage.rows - 1;
          int p3_x = atof(row[5])*BinaryImage.cols - 1;
          int p3_y = atof(row[6])*BinaryImage.rows - 1;
          int min_x = p1_x;
          int min_y = p1_y;
          int max_x = p1_x;
          int max_y = p1_y;
          if(p2_x < min_x) min_x = p2_x;
          if(p3_x < min_x) min_x = p3_x;
          if(p2_x > max_x) max_x = p2_x;
          if(p3_x > max_x) max_x = p3_x;

          if(p2_y < min_y) min_y = p2_y;
          if(p3_y < min_y) min_y = p3_y;
          if(p2_y > max_y) max_y = p2_y;
          if(p3_y > max_y) max_y = p3_y;

          for(int wide_index = min_x;wide_index <= max_x;wide_index ++ )
            {
              for(int height_index = min_y;height_index <= max_y;height_index ++ )
                {
                  TraverseBinaryImage(wide_index,height_index,row);
                }
            }

          if(!result->data_cursor->next)
            {
              traverse_finished = true;
            }
        }
    }

}

void TriangleIndexVisitor::TraverseBinaryImage(int x_index, int y_index,MYSQL_ROW row)
{

      osg::Vec2 point1,point2,point3;
      point1.x() = atof(row[1])*BinaryImage.cols - 1;
      point1.y() = atof(row[2])*BinaryImage.rows - 1;
      point2.x() = atof(row[3])*BinaryImage.cols - 1;
      point2.y() = atof(row[4])*BinaryImage.rows - 1;
      point3.x() = atof(row[5])*BinaryImage.cols - 1;
      point3.y() = atof(row[6])*BinaryImage.rows - 1;

      osg::Vec2 current_pixel;
      current_pixel.x() = x_index;
      current_pixel.y() = y_index;

      if (IsPointInTriangle(current_pixel.x(),current_pixel.y(),point1.x(),point1.y(),point2.x(),point2.y(),point3.x(),point3.y()))//current_pixel is located in current triangle
        {
          float a11 = atof(row[16]);
          float a12 = atof(row[17]);
          float a13 = atof(row[18]);
          float a21 = atof(row[19]);
          float a22 = atof(row[20]);
          float a23 = atof(row[21]);
          float a31 = atof(row[22]);
          float a32 = atof(row[23]);
          float a33 = atof(row[24]);

          osg::Vec3 texture_pixel;
          texture_pixel.x() = (float)(x_index+1)/BinaryImage.cols;
          texture_pixel.y() = (float)(y_index+1)/BinaryImage.rows;
          texture_pixel.z() = 1;

          float s_point_x = a11*texture_pixel.x() + a12*texture_pixel.y() + a13;
          float s_point_y = a21*texture_pixel.x() + a22*texture_pixel.y() + a23;
          float s_point_z = a31*texture_pixel.x() + a32*texture_pixel.y() + a33;
          osg::Vec3f current_point;
          current_point.x() = s_point_x;
          current_point.y() = s_point_y;
          current_point.z() = s_point_z;

          int R = ColorfulImage.at<cv::Vec3b>(x_index,y_index)[0];//pixel value, it is used for side detection
          int G = ColorfulImage.at<cv::Vec3b>(x_index,y_index)[1];//pixel value, it is used for side detection
          int B = ColorfulImage.at<cv::Vec3b>(x_index,y_index)[2];//pixel value, it is used for side detection

          string sql = "insert into "+pixels_table_name+" values('"+ConvertToString(pixel_count)+
              "','"+ConvertToString(current_pixel.x()) +"','"+ConvertToString(current_pixel.y())+
              "','"+ConvertToString(s_point_x)+"','"+ConvertToString(s_point_y)+"','"+ConvertToString(s_point_z)+
              "','"+ConvertToString(R)+"','"+ConvertToString(G)+"','"+ConvertToString(B)+"');";
          mysql_query(mysql_add.connection,sql.c_str());//insert matrix into table

          pixel_count ++;
//          BinaryImage.at<cv::Vec3b>(x_index,y_index)[0] == 0;
        }

}
// 判断点P(x, y)与有向直线P1P2的关系. 小于0表示点在直线左侧，等于0表示点在直线上，大于0表示点在直线右侧
float TriangleIndexVisitor::EvaluatePointToLine(int x, int y, int x1, int y1, int x2, int y2)
{
	int a = y2 - y1;
	int b = x1 - x2;
	int c = x2 * y1 - x1 * y2;

	assert(fabs(a) > 0.00001f || fabs(b) > 0.00001f);

	return a * x + b * y + c;
}
// 判断点P(x, y)是否在点P1(x1, y1), P2(x2, y2), P3(x3, y3)构成的三角形内（包括边）
bool TriangleIndexVisitor::IsPointInTriangle(int x, int y, int x1, int y1, int x2, int y2, int x3, int y3)
{
	// 分别计算点P与有向直线P1P2, P2P3, P3P1的关系，如果都在同一侧则可判断点在三角形内
	// 注意三角形有可能是顺时针(d>0)，也可能是逆时针(d<0)。
	float d1 = EvaluatePointToLine(x, y, x1, y1, x2, y2);
	float d2 = EvaluatePointToLine(x, y, x2, y2, x3, y3);
	float d3 = EvaluatePointToLine(x, y, x3, y3, x1, y1);

	if (d1 * d2 < 0) return false;

	if (d2 * d3 < 0) return false;

	return true;
}

void TriangleIndexVisitor::DrawTriangles()
{
  IplImage imgTmp = BinaryImage;
  IplImage *img = cvCloneImage(&imgTmp);

  string sql_sellect_all_record = "select * from "+matrixes_table_name+";";
  int p = mysql_query(mysql_add.connection,sql_sellect_all_record.c_str());
  if(mysql_query(mysql_add.connection,sql_sellect_all_record.c_str()))
    {
      result = mysql_store_result(mysql_add.connection);
      bool traverse_finished = false;
      while (!traverse_finished) {
          row = mysql_fetch_row(result);
          CvPoint point1,point2,point3,_point1,_point2,_point3;

          point1.x = (atof(row[1])*BinaryImage.cols - 1) ;
          point1.y = (atof(row[2])*BinaryImage.rows - 1) ;
          point2.x = (atof(row[3])*BinaryImage.cols - 1) ;
          point2.y = (atof(row[4])*BinaryImage.rows - 1) ;
          point3.x = (atof(row[5])*BinaryImage.cols - 1) ;
          point3.y = (atof(row[6])*BinaryImage.rows - 1) ;

          int A = BinaryImage.rows;
          int B = -BinaryImage.cols;

          _point1.x = point1.x - 2*A*(A*point1.x + B*point1.y)/(A*A + B*B);
          _point1.y = point1.y - 2*B*(A*point1.x + B*point1.y)/(A*A + B*B);

          _point2.x = point2.x - 2*A*(A*point2.x + B*point2.y)/(A*A + B*B);
          _point2.y = point2.y - 2*B*(A*point2.x + B*point2.y)/(A*A + B*B);

          _point3.x = point3.x - 2*A*(A*point3.x + B*point3.y)/(A*A + B*B);
          _point3.y = point3.y - 2*B*(A*point3.x + B*point3.y)/(A*A + B*B);



//          cvLine(img,point1,point2,CV_RGB(255,255,255),1,CV_AA,0);//画直
//          cvLine(img,point2,point3,CV_RGB(255,255,255),1,CV_AA,0);
//          cvLine(img,point3,point1,CV_RGB(255,255,255),1,CV_AA,0);

          cvLine(img,_point1,_point2,CV_RGB(255,255,255),1,CV_AA,0);//画直
          cvLine(img,_point2,_point3,CV_RGB(255,255,255),1,CV_AA,0);
          cvLine(img,_point3,_point1,CV_RGB(255,255,255),1,CV_AA,0);

//          cvShowImage("triangls",img);//显示图片
//          cvWaitKey(0);


          if(!result->data_cursor->next)
            {
              traverse_finished = true;
            }
        }
    }

//  cvShowImage("triangls",img);//显示图片
//  cvWaitKey(0);

}

string TriangleIndexVisitor::ConvertToString(float Num)
{
    ostringstream oss;
    oss<<Num;
    string str(oss.str());
    return str;
}

void TriangleIndexVisitor::ClearAllTheRecords()
{
  string sql_record_delete_matrixes_table_name = "truncate "+matrixes_table_name+";";
  string sql_record_delete_pixels_table_name = "truncate "+pixels_table_name+";";

  mysql_query(mysql_add.connection,sql_record_delete_matrixes_table_name.c_str());
  mysql_query(mysql_add.connection,sql_record_delete_pixels_table_name.c_str());
}

void TriangleIndexVisitor::HoughLines()
{
//  cv::HoughLines(BinaryImage, lines, 1, CV_PI/180, 180);
//  std::vector<cv::Vec2f>::const_iterator it= lines.begin();
//  while (it!=lines.end())
//  {
//      float rho= (*it)[0]; // first element is distance rho
//      float theta= (*it)[1]; // second element is angle theta
//      if (theta < CV_PI/4. || theta > 3.*CV_PI/4.)// ~vertical line
//      {
//          // point of intersection of the line with first row
//          cv::Point pt1(rho/cos(theta), 0);
//          // point of intersection of the line with last row
//          cv::Point pt2((rho - BinaryImage.rows * sin(theta))/cos(theta), BinaryImage.rows);
//          // draw a white line
//          cv::line( BinaryImage, pt1, pt2, cv::Scalar(255), 1);
//      }
//      else
//      { // ~horizontal line
//          // point of intersection of the
//          // line with first column
//          cv::Point pt1(0,rho/sin(theta));
//          // point of intersection of the line with last column
//          cv::Point pt2(BinaryImage.cols, (rho - BinaryImage.cols * cos(theta))/sin(theta));
//          // draw a white line
//          cv::line(BinaryImage, pt1, pt2, cv::Scalar(255), 1);
//      }
//      ++it;
//  }

  LineFinder finder;
  // Set probabilistic Hough parameters
  finder.setLineLengthAndGap(100, 20);
  finder.setMinVote(80);
  // Detect lines and draw them
  lines = finder.findLines(BinaryImage);
  finder.drawDetectedLines(GrayImage, cv::Scalar(255, 0, 0));
  imshow("New_GrayImage",GrayImage);
  cvWaitKey(0);


}

void TriangleIndexVisitor::WriteIntoBinaryFile()
{
  ofstream write_to_binary_file(binary_file_path,ios::binary);
  if(write_to_binary_file)
    {
       write_to_binary_file.write((char*)&Graph_info,sizeof(Graph_info));
    }
}
//得到给定矩阵src的逆矩阵保存到des中。
bool MatrixComputation::GetMatrixInverse(float src[3][3],int n,float des[3][3])
{
    float flag=getA(src,n);
    float t[3][3];
    if(flag==0)
    {
        return false;
    }
    else
    {
        getAStart(src,n,t);
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                des[i][j]=t[i][j]/flag;
            }
        }
    }

    return true;
}

bool MatrixComputation::MatrixMutipule(float matrix_s[3][3], float matrix_t_inversed[3][3], float matrix_res[3][3])
{
  matrix_res[0][0]  =  matrix_s[0][0]*matrix_t_inversed[0][0] + matrix_s[0][1]*matrix_t_inversed[1][0] + matrix_s[0][2]*matrix_t_inversed[2][0];
  matrix_res[0][1]  =  matrix_s[0][0]*matrix_t_inversed[0][1] + matrix_s[0][1]*matrix_t_inversed[1][1] + matrix_s[0][2]*matrix_t_inversed[2][1];
  matrix_res[0][2]  =  matrix_s[0][0]*matrix_t_inversed[0][2] + matrix_s[0][1]*matrix_t_inversed[1][2] + matrix_s[0][2]*matrix_t_inversed[2][2];

  matrix_res[1][0]  =  matrix_s[1][0]*matrix_t_inversed[0][0] + matrix_s[1][1]*matrix_t_inversed[1][0] + matrix_s[1][2]*matrix_t_inversed[2][0];
  matrix_res[1][1]  =  matrix_s[1][0]*matrix_t_inversed[0][1] + matrix_s[1][1]*matrix_t_inversed[1][1] + matrix_s[1][2]*matrix_t_inversed[2][1];
  matrix_res[1][2]  =  matrix_s[1][0]*matrix_t_inversed[0][2] + matrix_s[1][1]*matrix_t_inversed[1][2] + matrix_s[1][2]*matrix_t_inversed[2][2];

  matrix_res[2][0]  =  matrix_s[2][0]*matrix_t_inversed[0][0] + matrix_s[2][1]*matrix_t_inversed[1][0] + matrix_s[2][2]*matrix_t_inversed[2][0];
  matrix_res[2][1]  =  matrix_s[2][0]*matrix_t_inversed[0][1] + matrix_s[2][1]*matrix_t_inversed[1][1] + matrix_s[2][2]*matrix_t_inversed[2][1];
  matrix_res[2][2]  =  matrix_s[2][0]*matrix_t_inversed[0][2] + matrix_s[2][1]*matrix_t_inversed[1][2] + matrix_s[2][2]*matrix_t_inversed[2][2];

}
//按第一行展开计算|A|
float MatrixComputation::getA(float arcs[3][3],int n)
{
    if(n==1)
    {
        return arcs[0][0];
    }
    float ans = 0;
    float temp[3][3]={0.0};
    int i,j,k;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n-1;j++)
        {
            for(k=0;k<n-1;k++)
            {
                temp[j][k] = arcs[j+1][(k>=i)?k+1:k];

            }
        }
        float t = getA(temp,n-1);
        if(i%2==0)
        {
            ans += arcs[0][i]*t;
        }
        else
        {
            ans -=  arcs[0][i]*t;
        }
    }
    return ans;
}
//计算每一行每一列的每个元素所对应的余子式，组成A*
void  MatrixComputation::getAStart(float arcs[3][3],int n,float ans[3][3])
{
    if(n==1)
    {
        ans[0][0] = 1;
        return;
    }
    int i,j,k,t;
    float temp[3][3];
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            for(k=0;k<n-1;k++)
            {
                for(t=0;t<n-1;t++)
                {
                    temp[k][t] = arcs[k>=i?k+1:k][t>=j?t+1:t];
                }
            }


            ans[j][i]  =  getA(temp,n-1);
            if((i+j)%2 == 1)
            {
                ans[j][i] = - ans[j][i];
            }
        }
    }
}

void LineFinder::setAccResolution(double dRho, double dTheta)
{
    deltaRho= dRho;
    deltaTheta= dTheta;
}

void LineFinder::setMinVote(int minv)
{
    minVote= minv;
}

void LineFinder::setLineLengthAndGap(double length, double gap)
{
    minLength= length;
    maxGap= gap;
}

std::vector<cv::Vec4i> LineFinder::findLines(cv::Mat &binary)
{
    lines.clear();
    cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
    return lines;
}

void LineFinder::drawDetectedLines(cv::Mat &image, cv::Scalar color)
{
//   Draw the lines
    std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();
    while (it2 != lines.end())
    {
        cv::Point pt1((*it2)[0],(*it2)[1]);
        cv::Point pt2((*it2)[2],(*it2)[3]);
        cv::line( image, pt1, pt2, color, 2);
        ++it2;
    }
}

