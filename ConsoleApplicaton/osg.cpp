#include <osg.h>
#include <document.h>
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
    int temp_count = 0;
    for(int index = 0;index < int(searchingTree.size());index++)
    {
        if(searchingTree[index]->loadable)
        {
            osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(searchingTree[index]->file_path);
            group->addChild(node.get());
            temp_count++;
//            break;
            if (temp_count >100)
              {
                break;
              }

        }
    }

//    RebuildTextImage* rebuild_image;
//    rebuild_image->ReadBinaryFile("/home/shilei/桌面/ObjectDettection/ConsoleApplicaton/bin/Tile_002_004_L22_0002210_lines.bin");

    FindGeometryVisitor fgev;
    group->accept(fgev);

//    osg::ref_ptr<osg::Geode> geode_for_points = DrawSpatialPoints();
//    group->addChild(geode_for_points);


    osg::ref_ptr<osg::Geode> geode_for_triangles = DrawSpatialTriangles();
    group->addChild(geode_for_triangles);

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

//draw triangles
osg::ref_ptr<osg::Geode> OSG::DrawSpatialTriangles()
{
  vector<string> _model_name;
  CMyDBAdd database;
  database.initDB("localhost" , "root", "shilei744534" , "convert_matrix");
  ReadModelNames(database,_model_name);
  return CreateGemotry(database,_model_name);
//  return geode.release();

}

void OSG::ReadModelNames(CMyDBAdd & database,vector<string> & _model_name)
{
  string sql_sellect_all_models = "select * from model_name;";
  int p = mysql_query(database.connection,sql_sellect_all_models.c_str());
  if(mysql_query(database.connection,sql_sellect_all_models.c_str()))
    {
      MYSQL_RES* result_model;
      MYSQL_ROW row_model;
      result_model = mysql_store_result(database.connection);
      bool traverse_models_finished = false;

      while (!traverse_models_finished) {
          row_model = mysql_fetch_row(result_model);
          string current_model_name = string(row_model[1]);
          _model_name.push_back(current_model_name);

          if(!result_model->data_cursor->next)
            {
              traverse_models_finished = true;
            }
        }
    }

}

osg::ref_ptr<osg::Geode>  OSG::CreateGemotry(CMyDBAdd &database, vector<string> &_model_name)
{
  osg::ref_ptr<osg::Geode> geode = new osg::Geode;

  for(int model_index = 0;model_index < _model_name.size();model_index ++)
    {
      string modelName = string(_model_name[model_index]);
//      database.initDB("localhost" , "root", "shilei744534" , "convert_matrix");
      string sql_sellect_all_triangles = "select * from "+modelName+";";
      cout<<sql_sellect_all_triangles<<endl;

      int q = mysql_query(database.connection,sql_sellect_all_triangles.c_str());
      if(mysql_query(database.connection,sql_sellect_all_triangles.c_str()))
        {
          MYSQL_RES* result_triangle;
          MYSQL_ROW row_triangle;

          result_triangle = mysql_store_result(database.connection);
          bool traverse_triangles_finished = false;
          while (!traverse_triangles_finished) {
              row_triangle = mysql_fetch_row(result_triangle);


              if (atof(row_triangle[10]))//if current triangle is showable,show it
                {
                  osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
                  osg::ref_ptr<osg::Vec3Array> vecarray = new osg::Vec3Array;     //顶点坐标数组

                  //point set about a triangle
                  vecarray->push_back(osg::Vec3(atof(row_triangle[1]), atof(row_triangle[2]), atof(row_triangle[3])));
                  vecarray->push_back(osg::Vec3(atof(row_triangle[4]), atof(row_triangle[5]), atof(row_triangle[6])));
                  vecarray->push_back(osg::Vec3(atof(row_triangle[7]), atof(row_triangle[8]), atof(row_triangle[9])));

                  osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array();
                  color->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
                  color->push_back(osg::Vec4(0.0f,1.0f,0.0f,1.0f));
                  color->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
                  color->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));

                  geometry->setVertexArray(vecarray);
                  geometry->setColorArray(color);
                  geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,3));
                  geode->addDrawable(geometry);
                }

              if(!result_triangle->data_cursor->next)
                {
                  traverse_triangles_finished = true;
                }

            }


        }
    }
  return geode;

}
//draw points
osg::ref_ptr<osg::Geode> OSG::DrawSpatialPoints()
{
  DrawPoints* draw_point;
  CMyDBAdd database;
  MYSQL_RES* result;
  MYSQL_ROW row;
  std::vector<PIXEL_INFO*> Graph_info;

//  database.initDB("localhost" , "root", "shilei744534" , "convert_matrix");

  string sql_sellect_all_record = "select * from Tile_002_004_L22_0002210_pixels;";
  int p = mysql_query(database.connection,sql_sellect_all_record.c_str());
  if(mysql_query(database.connection,sql_sellect_all_record.c_str()))
    {
      result = mysql_store_result(database.connection);
      bool traverse_finished = false;
      while (!traverse_finished) {
//          row = result->current_row;
          row = mysql_fetch_row(result);
          PIXEL_INFO* current_point = (PIXEL_INFO*)malloc(sizeof(PIXEL_INFO));
          current_point->pixel_count = Graph_info.size();
          current_point->spatial_x = atof(row[3]);
          current_point->spatial_y = atof(row[4]);
          current_point->spatial_z = atof(row[5]);
          current_point->R = atof(row[6]);
          current_point->G = atof(row[7]);
          current_point->B = atof(row[8]);

          Graph_info.push_back(current_point);

          if(!result->data_cursor->next)
            {
              traverse_finished = true;
            }

        }
    }

  osg::ref_ptr<osg::Geode> geode = new osg::Geode;

  for(int point_index = 0;point_index < int(Graph_info.size());point_index++)
    {
      float x,y,z,R,G,B;
      x = Graph_info[point_index]->spatial_x;
      y = Graph_info[point_index]->spatial_y;
      z = Graph_info[point_index]->spatial_z;
      R= Graph_info[point_index]->R;
      G= Graph_info[point_index]->G;
      B= Graph_info[point_index]->B;

      osg::Drawable* point = draw_point->DrawPoint(x,y,z,R,G,B);
      geode->addDrawable(point);

    }

  return geode.release();

}

