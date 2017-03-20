#ifndef TREE_H
#define TREE_H

#endif // TREE_H
#pragma once

#include <list>
#include <algorithm>
#include <vector>
#include <osgViewer/Viewer>
using namespace std;

struct TreeNode; //定义一个结构体原形
struct FileNameInformation;//定义一个结构体原形
class Tree;      //定义一个类原形
class Iterator; //定义一个类原形



struct FileNameInformation //文件名信息
{
//	char* filename;
	bool include_level_info;
	char level_info[3];
	int level_info_position;
	vector<char> tail_info;
	int file_length;
	bool loadable;
};


struct TreeNode
{
   char* file_path;//节点路径
   char* file_name;//节点名字
   int id;//节点id
   int father_location_index;//父节点索引
   vector<int> children_location_index;//子节点索引组
   bool loadable;//选择是否加载
   FileNameInformation* file_name_info;//文件信息
};




class Tree
{

public:

 //下面是构造器和运算符重载
   Tree();                            //默认构造函数
   ~Tree();                           //析构函数

   //下面是成员函数
   void Clear();                      //清空
   bool IsEmpty()const;               //判断是否为空
   int Size()const;                   //计算节点数目
   int Leaves();                      //计算叶子数
   int Root()const;                   //返回根元素
   int Height();                      //计算树的高度

   //下面是静态成员函数
   static TreeNode* initialRoot(char& file_path);
   static bool IsRoot(Iterator);     //判断是否是根
   static bool isLeaf(Iterator);     //判断是否是叶子
   static Iterator Parent(Iterator); //返回其父节点
   static int NumChildren(Iterator); //返回其子节点数目

   TreeNode* CreatANode(char* file_path);//创建一个节点

   //跌代器函数
   Iterator begin();                  //Tree Begin
   Iterator end();                    //Tree End
   friend class Iterator;             //Iterator SubClass
private:
   list<TreeNode*> nodes;         //节点数组
   list<TreeNode*>::iterator LIt; //一个节点迭代器
   int height(TreeNode*);
   int level(TreeNode*,Iterator);
};

class Functions
{
public:
  Functions();
  ~Functions();

  char* FindTheLevelInformation(char* filename);
  FileNameInformation* AnalysisFileName(char* file_name);
  void FindFatherAndChildrenNode(vector<TreeNode*> r_searching_tree,TreeNode* current_node);//传指针的引用进来,直接修改searching_tree和current_node，与直接对象的引用没什么区别
  bool JudgeStringsEqual(char* char1,char* char2);//深度比较字符串是否相等

private:

};
