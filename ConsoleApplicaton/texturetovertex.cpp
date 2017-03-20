#include <texturetovertex.h>

float IndexToTextureCoord::getRowTexCoord(int col_index, int S)
{
    float tx;
    tx = (float)(col_index+1)/S;
    return tx;
}

float IndexToTextureCoord::getColuTexCoord(int row_index,int T)
{
    float ty;
    ty = (float)(row_index+1)/T;
    return ty;
}

TriangleIndexVisitor::TriangleIndexVisitor()
{  
  mysql_add.initDB("localhost" , "root", "shilei744534" , "convert_matrix" );  
  temp_id = 0;
  find_map_matrix = false;
  pixel_count = 0;
//  mysql_add.executeSQL("select * from matrix_info;");
  count = 0;

}

TriangleIndexVisitor::~TriangleIndexVisitor()
{
  count = 0;
//    ClearAllTheRecords();

}

void TriangleIndexVisitor::CreateTable()
{
//  ClearAllTheRecords();
  string sql_create_table_matrix = "create table "+matrixes_table_name+"   (id int(11) primary key,t_point1_x float,t_point1_y float,t_point2_x float,t_point2_y float,t_point3_x float,t_point3_y float,s_point1_x float,s_point1_y float,s_point1_z float,s_point2_x float,s_point2_y float,s_point2_z float,s_point3_x float,s_point3_y float,s_point3_z float,a11 float,a12 float,a13 float,a21 float,a22 float,a23 float,a31 float,a32 float,a33 float);";
  mysql_query(mysql_add.connection,sql_create_table_matrix.c_str());
  string sql_create_table_lines = "create table "+lines_table_name+"   (id int(11) primary key,t_point_x float,t_point_y float,s_point_x float,s_point_y float,s_point_z float);";
  mysql_query(mysql_add.connection,sql_create_table_lines.c_str());

}

void TriangleIndexVisitor::operator ()(const int index1, const int index2, const int index3)
{
  MapMatrix(index1,index2,index3);//calculate mam matrix
}

void TriangleIndexVisitor::MapMatrix(const int index1, const int index2, const int index3)
{

  float tx1,ty1,tx2,ty2,tx3,ty3,x1,y1,z1,x2,y2,z2,x3,y3,z3;
  tx1 = ((float*)(arytexcoord->getDataPointer()))[2*index1];   //第一个顶点纹理坐标的第一个元素值
  tx2 = ((float*)(arytexcoord->getDataPointer()))[2*index2];   //第二个顶点纹理坐标的第一个元素值
  tx3 = ((float*)(arytexcoord->getDataPointer()))[2*index3];   //第三个顶点纹理坐标的第一个元素值

  ty1 = ((float*)(arytexcoord->getDataPointer()))[2*index1+1]; //第一个顶点纹理坐标的第二个元素值
  ty2 = ((float*)(arytexcoord->getDataPointer()))[2*index2+1]; //第二个顶点纹理坐标的第二个元素值
  ty3 = ((float*)(arytexcoord->getDataPointer()))[2*index3+1]; //第三个顶点纹理坐标的第二个元素值

  x1 = ((float*)(aryvercoord->getDataPointer()))[3*index1];
  x2 = ((float*)(aryvercoord->getDataPointer()))[3*index2];
  x3 = ((float*)(aryvercoord->getDataPointer()))[3*index3];


  y1 = ((float*)(aryvercoord->getDataPointer()))[3*index1+1];
  y2 = ((float*)(aryvercoord->getDataPointer()))[3*index2+1];
  y3 = ((float*)(aryvercoord->getDataPointer()))[3*index3+1];

  z1 = ((float*)(aryvercoord->getDataPointer()))[3*index1+2];
  z2 = ((float*)(aryvercoord->getDataPointer()))[3*index2+2];
  z3 = ((float*)(aryvercoord->getDataPointer()))[3*index3+2];

//  计算转换关系
  float a11 = (((tx1*y2-tx2*y1)*(z2*y3-z3*y2))-((tx2*y3-tx3*y2)*(z1*y2-z2*y1)))/(((x1*y2-x2*y1)*(z2*y3-z3*y2))-((x2*y3-x3*y2)*(z1*y2-z2*y1)));
  float a12 = (((tx1*x2-x1*tx2)*(z2*x3-z3*x2))-((x3*tx2-x2*tx3)*(z1*x2-z2*x1)))/(((y1*x2-y2*x1)*(z2*x3-z3*x2))-((y2*x3-y3*x2)*(z1*x2-z2*x1)));
  float a13 = (((tx1*x2-tx2*x1)*(y2*x3-y3*x2))-((tx2*x3-tx3*x2)*(y1*x2-y2*x1)))/(((z1*x2-z2*x1)*(y2*x3-y3*x2))-((z2*x3-z3*x2)*(y1*x2-y2*x1)));

  float a21 = (((ty1*y2-ty2*y1)*(z2*y3-z3*y2))-((ty2*y3-ty3*y2)*(z1*y2-z2*y1)))/(((x1*y2-x2*y1)*(z2*y3-z3*y2))-((x2*y3-x3*y2)*(z1*y2-z2*y1)));
  float a22 = (((ty1*x2-ty2*x1)*(z2*x3-z3*x2))-((ty2*x3-ty3*x2)*(z1*x2-z2*x1)))/(((y1*x2-y2*x1)*(z2*x3-z3*x2))-((y2*x3-y3*x2)*(z1*x2-z2*x1)));
  float a23 = (((ty1*x2-ty2*x1)*(y2*x3-y3*x2))-((ty2*x3-ty3*x2)*(y1*x2-y2*x1)))/(((z1*x2-z2*x1)*(y2*x3-y3*x2))-((z2*x3-z3*x2)*(y1*x2-y2*x1)));

  float a31 = (((0*y2-0*y1)*(z2*y3-z3*y2))-((0*y3-ty3*y2)*(z1*y2-z2*y1)))/(((x1*y2-x2*y1)*(z2*y3-z3*y2))-((x2*y3-x3*y2)*(z1*y2-z2*y1)));
  float a32 = (((0*x2-0*x1)*(z2*x3-z3*x2))-((0*x3-ty3*x2)*(z1*x2-z2*x1)))/(((y1*x2-y2*x1)*(z2*x3-z3*x2))-((y2*x3-y3*x2)*(z1*x2-z2*x1)));
  float a33 = (((0*x2-0*x1)*(y2*x3-y3*x2))-((0*x3-ty3*x2)*(y1*x2-y2*x1)))/(((z1*x2-z2*x1)*(y2*x3-y3*x2))-((z2*x3-z3*x2)*(y1*x2-y2*x1)));

  raw_matrix[0][0] = a11;
  raw_matrix[0][1] = a12;
  raw_matrix[0][2] = a13;
  raw_matrix[1][0] = a21;
  raw_matrix[1][1] = a22;
  raw_matrix[1][2] = a23;
  raw_matrix[2][0] = a31;
  raw_matrix[2][1] = a32;
  raw_matrix[2][2] = a33;
  matrixInverse->GetMatrixInverse(raw_matrix,3,inversed_matrix);//get inversed,and now, we have the map matrix,and we could calculate its 3 dimension coordinata
//  write to the table
  string sql = "insert into "+matrixes_table_name+" values('"+ConvertToString(count)+"','"+ConvertToString(tx1) +"','"+ConvertToString(ty1)+"','"+ConvertToString(tx2)+"','"+ConvertToString(ty2)+"','"+ConvertToString(tx3)+"','"+ConvertToString(ty3)+"','"+ConvertToString(x1)+"','"+ConvertToString(y1)+"','"+ConvertToString(z1)+"','"+ConvertToString(x2)+"','"+ConvertToString(y2)+"','"+ConvertToString(z2)+"','"+ConvertToString(x3)+"','"+ConvertToString(y3)+"','"+ConvertToString(z3)+"','"+ConvertToString(inversed_matrix[0][0])+"','"+ConvertToString(inversed_matrix[0][1])+"','"+ConvertToString(inversed_matrix[0][2])+"','"+ConvertToString(inversed_matrix[1][0])+"','"+ConvertToString(inversed_matrix[1][1])+"','"+ConvertToString(inversed_matrix[1][2])+"','"+ConvertToString(inversed_matrix[2][0])+"','"+ConvertToString(inversed_matrix[2][1])+"','"+ConvertToString(inversed_matrix[2][2])+"');";
  mysql_query(mysql_add.connection,sql.c_str());//insert matrix into table
  count++;
}

void TriangleIndexVisitor::CreateBinaryImage()
{
  cv::Mat TextImage(texImg_->t(),texImg_->s(),CV_8UC3);
  for(int wide_index = 0 ;wide_index < texImg_->s() ; wide_index++)// rows
    {
      for(int height_index = 0 ;height_index < texImg_->t() ; height_index++) //cols
        {
//          int row_num = texImg_->t();
//          int col_num = texImg_->s();
          osg::Vec4 pixel_info = texImg_->getColor(wide_index,height_index,0);
          TextImage.at<cv::Vec3b>(height_index,wide_index)[0] = floor(pixel_info[0]*255);
          TextImage.at<cv::Vec3b>(height_index,wide_index)[1] = floor(pixel_info[1]*255);
          TextImage.at<cv::Vec3b>(height_index,wide_index)[2] = floor(pixel_info[2]*255);
        }
    }


  cvtColor(TextImage,GrayImage,CV_RGB2GRAY);
  Canny(GrayImage,GrayImage,500,300,3,true);

//  imshow("TextImage",TextImage);
//  cvWaitKey(0);

//  imshow("GrayImage",GrayImage);
//  cvWaitKey(0);
  MapToSpatialCoordinate();
  sideManage->DrawSidesOnSpatialModel();//draw sides

//  ClearAllTheRecords();
}

void TriangleIndexVisitor::MapToSpatialCoordinate()
{
//  find those white pixels
  for(int col_index = 0;col_index < GrayImage.cols;col_index ++)
    {
      for(int row_index = 0;row_index < GrayImage.rows;row_index ++)
        {
          if(GrayImage.at<cv::Vec3b>(col_index,row_index)[0] == 255)//we also need to judge that if it locates in current region
            {
              osg::Vec2 current_pixel;
              current_pixel.x() = col_index;
              current_pixel.y() = row_index;

//              current_pixel.x() = 895;
//              current_pixel.y() = 408;



              // find the location of current_pixel,tranverse all the records in table matrix_info
              while (!find_map_matrix && temp_id < count) {
                  string sql = "select t_point1_x,t_point1_y,t_point2_x,t_point2_y,t_point3_x,t_point3_y,a11,a12,a13,a21,a22,a23,a31,a32,a33 from "+matrixes_table_name+" where id ="+ConvertToString(temp_id)+";";
                  //visit matrix_info
                  if(mysql_query(mysql_add.connection,sql.c_str()))//successful queried
                    {
                      result = mysql_store_result(mysql_add.connection);
                      row = mysql_fetch_row(result);//it is managed as a two dimensions array;
                      field = mysql_fetch_field(result);

                      float t_point1_x = atof(row[0])*GrayImage.cols - 1;
                      float t_point1_y = atof(row[1])*GrayImage.rows - 1;
                      float t_point2_x = atof(row[2])*GrayImage.cols - 1;
                      float t_point2_y = atof(row[3])*GrayImage.rows - 1;
                      float t_point3_x = atof(row[4])*GrayImage.cols - 1;
                      float t_point3_y = atof(row[5])*GrayImage.rows - 1;

                      float a11 = atof(row[6]);
                      float a12 = atof(row[7]);
                      float a13 = atof(row[8]);
                      float a21 = atof(row[9]);
                      float a22 = atof(row[10]);
                      float a23 = atof(row[11]);
                      float a31 = atof(row[12]);
                      float a32 = atof(row[13]);
                      float a33 = atof(row[14]);

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

                      if (abs(acosf((vec1) * (vec2)) + acosf((vec1) * (vec3)) + acosf((vec2) * (vec3)) - 2 * osg::PI) < 10/*1e-2*/)//current_pixel is located in current triangle
                        {
                          osg::Vec2 texture_pixel;
                          // convert to texture coordinates
                          texture_pixel.x() = (float)(col_index+1)/GrayImage.cols;
                          texture_pixel.y() = (float)(row_index+1)/GrayImage.rows;

//                          texture_pixel.x() = (float)(895+1)/GrayImage.cols;
//                          texture_pixel.y() = (float)(408+1)/GrayImage.rows;

                          float s_point_x = a11*texture_pixel.x() + a12*texture_pixel.y();
                          float s_point_y = a21*texture_pixel.x() + a22*texture_pixel.y();
                          float s_point_z = a31*texture_pixel.x() + a32*texture_pixel.y();

                          string sql = "insert into "+lines_table_name+" values('"+ConvertToString(pixel_count)+"','"+ConvertToString(current_pixel.x()) +"','"+ConvertToString(current_pixel.y())+"','"+ConvertToString(s_point_x)+"','"+ConvertToString(s_point_y)+"','"+ConvertToString(s_point_z)+"');";
                          int p =mysql_query(mysql_add.connection,sql.c_str());//insert matrix into table
                          pixel_count ++;
                          find_map_matrix = true;
                        }
                      else {
                          temp_id ++;
                        }


                    }

                }

//              find_map_matrix = false;
              find_map_matrix = false;
              temp_id = 0;
            }
        }
    }

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
  string sql_record_delete_lines_table_name = "truncate "+lines_table_name+";";

  mysql_query(mysql_add.connection,sql_record_delete_matrixes_table_name.c_str());
  mysql_query(mysql_add.connection,sql_record_delete_lines_table_name.c_str());
}

void FindGeometryVisitor::apply(osg::Group &group)
{    
    for(int node_index = 0;node_index < group.getNumChildren();node_index ++)
      {
        int s = group.getNumChildren();
        osg::Node* node = group.getChild(node_index);
        osg::Geode*  geode = node->asGeode();
        if (geode)
          {
            string file_name_table = GetTableName(geode);
            for(unsigned int i = 0; i<1;  i++ ) //
    //        for(unsigned int i = 0; i<geode->getNumDrawables();  i++ ) //
            {
    //            int p = geode->getNumDrawables();
                osg::Geometry* geo = dynamic_cast<osg::Geometry*>(geode->getDrawable(i));
                osg::Array* vercoord = geo->getVertexArray();
                osg::Array* texcoord = geo->getTexCoordArray(i);
                osg::Drawable* drawable = geode->getDrawable(i);

                triIndex.count = 0;
                triIndex.matrixes_table_name = file_name_table + "_matrix";
                triIndex.lines_table_name = file_name_table + "_lines";
                triIndex.arytexcoord = texcoord;
                triIndex.aryvercoord = vercoord;
                triIndex.CreateTable();
//                triIndex.ClearAllTheRecords();
                drawable->accept(triIndex);
            }

            getImage(geode);//getting image behind computing convert_matrix

          }
        break;

      }

}

void FindGeometryVisitor::getImage(osg::Geode *geode)
{
//    osg::Image* texImg = NULL;
    for(unsigned int i = 0; i<1;  i++ ) //
//    for(unsigned int i=0;i<geode->getNumDrawables();i++)
    {
        osg::StateSet* state = geode->getDrawable(i)->getStateSet();
        osg::StateSet::TextureAttributeList& texAttribList = state->getTextureAttributeList();
        for(unsigned int j=0;j<texAttribList.size();j++)
        {
//            int p = texAttribList.size();
            osg::Texture2D* tex2D = NULL;
            if(tex2D = dynamic_cast<osg::Texture2D*>(state->getTextureAttribute(j,osg::StateAttribute::TEXTURE)))
            {
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
//得到给定矩阵src的逆矩阵保存到des中。
bool MatrixInverse::GetMatrixInverse(float src[3][3],int n,float des[3][3])
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
//按第一行展开计算|A|
float MatrixInverse::getA(float arcs[3][3],int n)
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
void  MatrixInverse::getAStart(float arcs[3][3],int n,float ans[3][3])
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
