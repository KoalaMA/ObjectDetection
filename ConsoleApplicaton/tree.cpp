#include <vector>
#include "tree.h"
#include "cstdlib"

FileNameInformation* Functions::AnalysisFileName(char* filename)
{
	FileNameInformation* filename_info=new	FileNameInformation();
//	filename_info->filename=filename;
	filename_info->file_length=strlen(filename);

	for (int i = 0; i <int(strlen( filename)); i++)
	{
		if (filename[i]=='L')
		{
			filename_info->level_info[0]=filename[i];//mark_info的前三个元素代表的是文件的等级信息，level
			filename_info->level_info[1]=filename[i+1];
			filename_info->level_info[2]=filename[i+2];
			i=i+4;

			//char* tail_info;//找尾巴
			vector<char> tail_info((filename_info->file_length-5)-i);//减6是因为减去小尾巴“.osgb”之后，还要减去一个空白的字符尾巴“\0"

			for (int j=0; i < (filename_info->file_length)-5; i++)//去掉小尾巴 “.osgb”之后，还要去掉“\0”，所以是减6
			{
				tail_info[j]=filename[i];//获得小尾巴的信息
				j++;
			}
			filename_info->tail_info=tail_info;

		}
		else //文件名中没有level信息
		{
			filename_info->level_info[0]=0;
			filename_info->level_info[1]=0;
			filename_info->level_info[2]=0;

			vector<char> tail_info((filename_info->file_length-5)-i);//减6是因为减去小尾巴“.osgb”之后，还要减去一个空白的字符尾巴“\0"

			for (int j=0; i < (filename_info->file_length)-5; i++)//去掉小尾巴 “.osgb”之后，还要去掉“\0”，所以是减6
			{
				tail_info[j]=filename[i];//获得小尾巴的信息
				j++;
			}
			filename_info->tail_info=tail_info;
		}

	}



	return filename_info;
}

TreeNode* Tree::CreatANode(char* children_path)
{
	TreeNode* current_node ;
	current_node=(TreeNode *)malloc (sizeof(TreeNode)); //开辟内存空间
	if (current_node==NULL)
	{
		return nullptr;
	}
	current_node->file_path=children_path;
	return current_node;
}

void Functions::FindFatherAndChildrenNode(vector<TreeNode*> r_searching_tree,TreeNode* r_current_node)//传引用进来，直接修改对象的值
{
	r_current_node->id=r_searching_tree.size();
	if (r_searching_tree.empty())//如果当前的搜索树为空，则只将当前节点添加即可
	{
		r_searching_tree.push_back(r_current_node);//将节点添加进树中，然后再修改树的树的节点的各种属性值
	}
	else
	{
		r_searching_tree.push_back(r_current_node);//将节点添加进树中，然后再修改树的树的节点的各种属性值
		for (int index = 0; index <int( r_searching_tree.size()-1); index++)//可以与自身作比较，不影响
		{
			char temp_father_name[256]="";
			char temp_children_name[256]="";
			char joint_result[256]="";
			strcat(temp_father_name, r_searching_tree[index]->file_name);
			strcat(temp_children_name, r_current_node->file_name);

			//此处不能够盲目的取出小尾巴，应该先做一个判断

			//去掉父节点的小尾巴“.osgb”
			for (int index1 = 0 ; index1 < int(strlen(temp_father_name)-5) ; index1++)//截取父节点的前半部分
			{
				joint_result[index1]=temp_father_name[index1];
			}
			//将子节点的小尾巴接上“x.osgb”
			for (int index2 = strlen(joint_result) ; index2 < int(strlen(temp_children_name)); index2++)
			{
				joint_result[index2]=temp_children_name[index2];
			}

			if ( JudgeStringsEqual(joint_result,r_current_node->file_name))//如果拼接后的文件名与当前节点的文件名相同，则证明当前节点为找到的节点的子节点
			{
				//int location_index=0;
				//location_index=(r_searching_tree.size()-1);
				r_searching_tree[index]->children_location_index[sizeof(r_searching_tree[index]->children_location_index)]= r_current_node->id;//设定子节点的索引值
				r_searching_tree[r_searching_tree.size()-1]->father_location_index=index;//设定父节点的索引值
				break;//只会出现一个父节点
			}


		}
	}



}

bool Functions::JudgeStringsEqual(char* char1,char* char2)//深度比较字符串是否相等
{
	bool IsEqual=false;
	int index=0;
	while (char1[index] == char2[index])
	{
		index++;
		if (index == int(strlen(char1)))
		{
			IsEqual=true;
		}

	}
	return IsEqual;
}

Functions::Functions(){    }
Functions::~Functions(){    }
Tree::Tree(){   }                           //默认构造函数
Tree::~Tree(){   }
