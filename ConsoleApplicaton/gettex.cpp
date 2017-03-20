#include <osg/Group>
#include <gettex.h>

void GetTexture::getImage(osg::Geode &geode)
{
    GetTexture::teximg = NULL;
    for(unsigned int i=0;i<geode.getNumDrawables();i++)
    {
        osg::StateSet* state = geode.getDrawable(i)->getStateSet();
        osg::StateSet::TextureAttributeList& texAttribList = state->getTextureAttributeList();
        for(unsigned int j=0;j<texAttribList.size();j++)
        {
            osg::Texture2D* tex2D = NULL;
            if(tex2D = dynamic_cast<osg::Texture2D*>(state->getTextureAttribute(j,osg::StateAttribute::TEXTURE)))
            {
                teximg = tex2D->getImage();
            }
        }


    }
    traverse(geode);
}

void TrianglePrinter_::operator ()(const osg::Vec3 &v1, const osg::Vec3 &v2, const osg::Vec3 &v3, bool)
{
    count++;
    cout<<"三角面片顶点： "<<v1.x()<<"  "<<v1.y()<<"   "<<v1.z()<<endl;
    cout<<"三角面片顶点： "<<v2.x()<<"  "<<v2.y()<<"   "<<v2.z()<<endl;
    cout<<"三角面片顶点： "<<v3.x()<<"  "<<v3.y()<<"   "<<v3.z()<<endl;
    std::cout<<"  "<<std::endl;
    osg::Vec3 m1 = v1 - point_;
    m1.normalize();
    osg::Vec3 m2 = v2 - point_;
    m2.normalize();
    osg::Vec3 m3 = v3 - point_;
    m3.normalize();

    if (abs(acosf((m1) * (m2)) + acosf((m1) * (m3)) + acosf((m2) * (m3)) - 2 * osg::PI) < 6)
    {
        tricount = count;
        v1_.push_back(v1);
        v2_.push_back(v2);
        v3_.push_back(v3);

    }
}

void FindGeometryVisitorm::loadpoint(osg::Vec3 &point)
{
    triPrinter.point_ = point;
}

void FindGeometryVisitorm::outvertex(std::vector<osg::Vec3> &v1, std::vector<osg::Vec3> &v2, std::vector<osg::Vec3> &v3)
{
    v1 = triPrinter.v1_;
    v2 = triPrinter.v2_;
    v3 = triPrinter.v3_;
}

void FindGeometryVisitorm::loadindices(int indices1, int indices2, int indices3)
{
    index1 = indices1;
    index2 = indices2;
    index3 = indices3;
}
//加载贴图
void FindGeometryVisitorm::loadteximage(osg::Image* image)
{
    teximage = image;
}

void FindGeometryVisitorm::apply(osg::Group &group)
{
    osg::Node* node = group.getChild(0);
    osg::Geode* geode = node->asGroup()->getChild(0)->asGeode();
    for (unsigned int i = 0; i < geode->getNumDrawables(); i++)
    {
        //float tex_x = 0.0;  //纹理x坐标
        //float tex_y = 0.0;  //纹理y坐标
        triPrinter.count = 0;
        osg::Geometry* geo=dynamic_cast<osg::Geometry*>(geode->getDrawable(i));
        osg::Array* texcoord = geo->getTexCoordArray(i);
        int s = texcoord->getNumElements();
        osg::Array* vercoord = geo->getVertexArray();
        int ss = vercoord->getNumElements();


        osg::Drawable* drawable = geode->getDrawable(i);
        if (!drawable) continue;

        drawable->accept(triPrinter);

        //triPrinter.count;
        std::vector<osg::Vec3> &v1 = triPrinter.v1_;  //三角形三个顶点坐标
        std::vector<osg::Vec3> &v2 = triPrinter.v2_;
        std::vector<osg::Vec3> &v3 = triPrinter.v3_;

        osg::Vec3 &point_ = triPrinter.point_;//获取鼠标点击点的位置

        std::cout<<"鼠标点击点的三维坐标:  x: "<<point_.x()<<"  y: "<<point_.y()<<"  z: "<<point_.z()<<std::endl;
        //std::cout<<"该点所在三角面片第一个顶点的三维坐标:  x: "<<v1[v1.size()-1].x()<<"  y: "<<v1[v1.size()-1].y()<<"  z: "<<v1[v1.size()-1].z()<<std::endl;
        //std::cout<<"该点所在三角面片第二个顶点的三维坐标:  x: "<<v2[v1.size()-1].x()<<"  y: "<<v2[v1.size()-1].y()<<"  z: "<<v2[v1.size()-1].z()<<std::endl;
        //std::cout<<"该点所在三角面片第三个顶点的三维坐标:  x: "<<v3[v1.size()-1].x()<<"  y: "<<v3[v1.size()-1].y()<<"  z: "<<v3[v1.size()-1].z()<<std::endl;

        //三维坐标值
        float ex1 = ((float*)(vercoord->getDataPointer()))[3*index1];   //第一个顶点三维坐标的第一个值
        float ey1 = ((float*)(vercoord->getDataPointer()))[3*index1+1]; //第一个顶点三维坐标的第二个值
        float ez1 = ((float*)(vercoord->getDataPointer()))[3*index1+2]; //第一个顶点三维坐标的第三个值

        float ex2 = ((float*)(vercoord->getDataPointer()))[3*index2];  	//第二个顶点三维坐标的第一个值
        float ey2 = ((float*)(vercoord->getDataPointer()))[3*index2+1];	//第二个顶点三维坐标的第二个值
        float ez2 = ((float*)(vercoord->getDataPointer()))[3*index2+2]; //第二个顶点三维坐标的第三个值

        float ex3 = ((float*)(vercoord->getDataPointer()))[3*index3];	 //第三个顶点三维坐标的第一个值
        float ey3 = ((float*)(vercoord->getDataPointer()))[3*index3+1];	 //第三个顶点三维坐标的第二个值
        float ez3 = ((float*)(vercoord->getDataPointer()))[3*index3+3]; //第三个顶点三维坐标的第三个值

        //纹理坐标值
        float tx1 = ((float*)(texcoord->getDataPointer()))[2*index1];   //第一个顶点纹理坐标的第一个元素值
        float ty1 = ((float*)(texcoord->getDataPointer()))[2*index1+1];  //第一个顶点纹理坐标的第二个元素值
        float tx2 = ((float*)(texcoord->getDataPointer()))[2*index2];   //第二个顶点纹理坐标的第一个元素值
        float ty2 = ((float*)(texcoord->getDataPointer()))[2*index2+1];   //第二个顶点纹理坐标的第二个元素值
        float tx3 = ((float*)(texcoord->getDataPointer()))[2*index3];    //第三个顶点纹理坐标的第一个元素值
        float ty3 = ((float*)(texcoord->getDataPointer()))[2*index3+1];   //第三个顶点纹理坐标的第二个元素值

        /*int teximage_x1 = (int)((teximage->s())*(tx1));
                        int teximage_y1 = (int)((teximage->t())*(ty1));
                        int teximage_x2 = (int)((teximage->s())*(tx2));
                        int teximage_y2 = (int)((teximage->t())*(ty2));
                        int teximage_x3 = (int)((teximage->s())*(tx3));
                        int teximage_y3 = (int)((teximage->t())*(ty3));*/

        float x1 = v1[v1.size()-1].x();
        float x2 = v2[v1.size()-1].x();
        float x3 = v3[v1.size()-1].x();
        float y1 = v1[v1.size()-1].y();
        float y2 = v2[v1.size()-1].y();
        float y3 = v3[v1.size()-1].y();
        float z1 = v1[v1.size()-1].z();
        float z2 = v2[v1.size()-1].z();
        float z3 = v3[v1.size()-1].z();

        //计算转换关系
        float a11 = (((tx1*y2-tx2*y1)*(z2*y3-z3*y2))-((tx2*y3-tx3*y2)*(z1*y2-z2*y1)))/(((x1*y2-x2*y1)*(z2*y3-z3*y2))-((x2*y3-x3*y2)*(z1*y2-z2*y1)));
        float a12 = (((tx1*x2-x1*tx2)*(z2*x3-z3*x2))-((x3*tx2-x2*tx3)*(z1*x2-z2*x1)))/(((y1*x2-y2*x1)*(z2*x3-z3*x2))-((y2*x3-y3*x2)*(z1*x2-z2*x1)));
        float a13 = (((tx1*x2-tx2*x1)*(y2*x3-y3*x2))-((tx2*x3-tx3*x2)*(y1*x2-y2*x1)))/(((z1*x2-z2*x1)*(y2*x3-y3*x2))-((z2*x3-z3*x2)*(y1*x2-y2*x1)));

        float a21 = (((ty1*y2-ty2*y1)*(z2*y3-z3*y2))-((ty2*y3-ty3*y2)*(z1*y2-z2*y1)))/(((x1*y2-x2*y1)*(z2*y3-z3*y2))-((x2*y3-x3*y2)*(z1*y2-z2*y1)));
        float a22 = (((ty1*x2-ty2*x1)*(z2*x3-z3*x2))-((ty2*x3-ty3*x2)*(z1*x2-z2*x1)))/(((y1*x2-y2*x1)*(z2*x3-z3*x2))-((y2*x3-y3*x2)*(z1*x2-z2*x1)));
        float a23 = (((ty1*x2-ty2*x1)*(y2*x3-y3*x2))-((ty2*x3-ty3*x2)*(y1*x2-y2*x1)))/(((z1*x2-z2*x1)*(y2*x3-y3*x2))-((z2*x3-z3*x2)*(y1*x2-y2*x1)));

        // a31、a32、a33没有计算的必要
        /*float a31 = (((0*y2-0*y1)*(z2*y3-z3*y2))-((0*y3-ty3*y2)*(z1*y2-z2*y1)))/(((x1*y2-x2*y1)*(z2*y3-z3*y2))-((x2*y3-x3*y2)*(z1*y2-z2*y1)));
                        float a32 = (((0*x2-0*x1)*(z2*x3-z3*x2))-((0*x3-ty3*x2)*(z1*x2-z2*x1)))/(((y1*x2-y2*x1)*(z2*x3-z3*x2))-((y2*x3-y3*x2)*(z1*x2-z2*x1)));
                        float a33 = (((0*x2-0*x1)*(y2*x3-y3*x2))-((0*x3-ty3*x2)*(y1*x2-y2*x1)))/(((z1*x2-z2*x1)*(y2*x3-y3*x2))-((z2*x3-z3*x2)*(y1*x2-y2*x1)));*/

        //鼠标点击点的纹理坐标，由点击点的三维坐标的X值和Y值来计算对应的纹理坐标值
        float tx = a11*point_.x()+a12*point_.y()+a13*point_.z();
        float ty = a21*point_.x()+a22*point_.y()+a23*point_.z();

        std::cout<<"鼠标点击点的纹理坐标： x: "<<tx<<"  y: "<<ty<<std::endl;

        //找到纹理坐标对应的像元的位置
        int teximage_x = (int)((teximage->s())*(tx));
        int teximage_y = (int)((teximage->t())*(ty)); //对应像元

        osg::Vec4 teximage_color = teximage->getColor(teximage_x,teximage_y,0);

        std::cout<<"鼠标点击点的颜色值： r: "<<teximage_color.r()*255<<" g:"<<teximage_color.g()*255;
        std::cout<<" b:"<<teximage_color.b()*255<<" a:"<<teximage_color.a()<<std::endl;
        std::cout<<"  "<<std::endl;
    }

}

EventHandler::EventHandler() //osg::Image* image
{
    point_ = osg::Vec3(0.0f, 0.0f, 0.0f);
    v1_.clear();
    v2_.clear();
    v3_.clear();
    //texImage = image;
}

void EventHandler::addTriangles(osg::Group *root, std::vector<osg::Vec3> &v1, std::vector<osg::Vec3> &v2, std::vector<osg::Vec3> &v3)
{
    osg::ref_ptr<osg::Geode> triangles = new osg::Geode;
    osg::ref_ptr<osg::StateSet> stateset = triangles->getOrCreateStateSet();
    osg::ref_ptr<osg::Vec3Array> vertices;
    osg::ref_ptr<osg::Geometry> triangle;
    osg::ref_ptr<osg::Vec4Array> vcolor = new osg::Vec4Array;

    vcolor->push_back(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

    stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    stateset->setRenderBinDetails(-1, "RenderBin");
    //stateset->setRenderBinDetails(-1, "DepthSortedBin");

    for (unsigned int i = 0; i < v1.size(); i++)
    {
        vertices = new osg::Vec3Array;
        vertices->push_back(v1.at(i));
        vertices->push_back(v2.at(i));
        vertices->push_back(v3.at(i));

        triangle = new osg::Geometry;
        triangle->setVertexArray(vertices.get());
        triangle->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::TRIANGLES, 0, 3));

        triangle->setColorArray(vcolor.get());
        //triangle->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
        triangle->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

        triangles->addDrawable(triangle);

    }

    root->addChild(triangles);
}

bool EventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*>(&aa);

    if (!view) return false;

    osg::Group *root = dynamic_cast<osg::Group*>(view->getSceneData());
    if (!root) return false;

    switch (ea.getEventType())
    {
    case osgGA::GUIEventAdapter::PUSH:
    {
        if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
        {
            osgUtil::LineSegmentIntersector::Intersections intersections;
            if (view->computeIntersections(ea.getX(), ea.getY(), intersections))
            {
                osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
                point_ = hitr->getWorldIntersectPoint();
                std::cout<<"x"<<point_.x()<<" "<<point_.y()<<point_.z()<<std::endl;
                osg::Vec3 sm = (*hitr).drawable->getBound().center();
                const osgUtil::LineSegmentIntersector::Intersection::IndexList& vi1 = hitr->indexList;


                //root->accept(fgv);
                fgv.loadpoint(point_);
                indices1 = vi1[0];
                indices2 = vi1[1];
                indices3 = vi1[2];
                std::cout<<indices1<<" "<<indices2<<" "<<indices3<<std::endl;
                fgv.loadindices(indices1,indices2,indices3);
                fgv.loadteximage(texImage);
                root->accept(fgv);
                fgv.outvertex(v1_, v2_, v3_);


                addTriangles(root, v1_, v2_, v3_);
                return true;
            }
        }
        break;
    }

    case osgGA::GUIEventAdapter::KEYDOWN:
    {

    }

    default: break;
    }

    return false;
}


