#include <malloc.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <cassert>
#include <iostream>
#include <document.h>

DocumentManage::DocumentManage()
{

}

DocumentManage::~DocumentManage()
{

}

std::vector <TreeNode*> DocumentManage::OnFileFolderOpen()
{
  const char* dirname = "/home/shilei/Data";

  RecurrentReadFiles(dirname);
  FindFatherAndChildrenNode(searchingTree);

  int a = 0;
  for(int i = 0;i < int(searchingTree.size());i++)
    {
      if(searchingTree[i]->loadable)
        {
          a++;
        }
    }
  return searchingTree;
}

char* DocumentManage::CutTheTail(char* file_path)
{
  char* tail=new char[256];
  for (int i = strlen(file_path); i >0  ; i--)
  {
          if (ispunct(file_path[i])&&file_path[i]!='_'&&file_path[i]!='+'&&file_path[i]!='.')
          {
                  int temp_i=i;
                  for (int j = 0; j <int( strlen(file_path)-temp_i); j++,i++)
                  {
                          tail[j]=file_path[i+1];
                  }
                  break;
          }
  }
  return tail;
}

bool DocumentManage::MatchFileName(TreeNode* ergodic_element,TreeNode* current_element)
{
	bool is_matched=false , finished=false;
	int finishi_position_index = strlen(ergodic_element->file_name)-6;//减6是因为最后一个空字符会占一位
	int vernier_position_index = 0;
	while (!finished)
	{
		if (vernier_position_index == ergodic_element->file_name_info->level_info_position)//如果到了level信息的位置,跳三个位置
			vernier_position_index=vernier_position_index+3;

		if (vernier_position_index != finishi_position_index)//没有到最后一个元素
		{
			if (ergodic_element->file_name[vernier_position_index] != current_element->file_name[vernier_position_index]) //找到了不匹配的对象
			{
				break;
			}
		}
		else//到最后一个元素
		{
			if (ergodic_element->file_name[vernier_position_index] == current_element->file_name[vernier_position_index])//正在遍历的元素 与 当前元素
			{
				is_matched=true;
			}

			finished=true;
		}

		vernier_position_index++;
	}

	return is_matched;
}

void DocumentManage::FindFatherAndChildrenNode(vector<TreeNode*> searching_tree)//初始化搜索树的索引信息
{

  int current_element_index = 0;
  while (current_element_index < int(searchingTree.size()))
  {
      if (!searchingTree[current_element_index]->file_name_info->include_level_info)//文件名不包含level信息
        {
          searchingTree[current_element_index]->father_location_index = -1;//father node index
          searchingTree[current_element_index]->loadable = false;
  //      searchingTree[0]->children_location_index.push_back(current_element_index);
        }
        else
          {
            for (int index = searching_tree.size()-1; index > 0; index--)
              {
                if(!searchingTree[index]->file_name_info->include_level_info && searchingTree[index]->loadable)
                  {
                    searchingTree[index]->loadable = false;
                  }

                else if (searchingTree[current_element_index]->file_name_info->file_length - searchingTree[index]->file_name_info->file_length ==1)//长一位
                  {

                    if (MatchFileName(searchingTree[index],searchingTree[current_element_index]))//fore match
                      {
                        searchingTree[current_element_index]->father_location_index=index;//设置父节点索引
                        searchingTree[index]->children_location_index.push_back(current_element_index);//设置子节点索引
                        searchingTree[index]->loadable = false;
                        break;
                      }

                  }
              }

          }
        current_element_index++;
        while (current_element_index <= int(searchingTree.size()) && !searchingTree[current_element_index - 1]->loadable)
          {
            current_element_index++;
          }

  }
}

int DocumentManage::GetPositionIndex(int myArray[])//子节点的位置
{
	int position_index=0;
	for (; position_index < 64; position_index++)
	{
		if (myArray[position_index] == -842150451)
		{
			break;
		}
	}
	return position_index;
}

void DocumentManage::InitialStringValue(char string[])
{
  for(int pos_index = 0; pos_index < 512;pos_index++)
    {
      string[pos_index] = 0;
    }
}

void DocumentManage::initarray(char s[],char myarray[])
{
        unsigned int i;
    for(i=0;i<strlen(myarray);i++)
    {
            s[i]=myarray[i];
    }
    s[i]='\0';
    strcat(s,"\\");
}

void DocumentManage::copyString(string string1,char* string2)
{

    int i;
    for(i=0;i<int(strlen(string2));i++)
    {
            string1[i]=string2[i];
    }
    string1[i]='\0';
}

void DocumentManage::initialChildrenParent(char children_path[256])
{
	for (int index = strlen(children_path); index > 0; index--)
	{
		if (children_path[index]=='\\')
		{
			for (int index2 = index+1; index2 <int( strlen(children_path)); index2++)
			{
				children_path[index2]='\0';
			}
			break;
		}
	}
}

void DocumentManage::RecurrentReadFiles(const char* path)
{
      char file_path[256] = {0};
      char file_name[256] = {0};
      DIR *dir = NULL;
      struct dirent *FileInfo = NULL;//file inforomation
      struct stat buf;
      int i, j;
      /****************浏览目录***************************/
      if ((dir = opendir(path)) == NULL)
      {
          perror("opendir failed!");
          return ;
      }
      while((FileInfo = readdir(dir)) != NULL)// first class
      {

          if (FileInfo->d_name[0] != '.') //除去根文件目录
          {
              strcpy(file_path, path);
              if (path[strlen(path) - 1] != '/')
                strcat(file_path, "/");
              strcat(file_path, FileInfo->d_name); //构建完整的文件名
              assert(stat(file_path, &buf) != -1);

              if(S_ISREG(buf.st_mode))  //判断的是文件 file
              {

                  TreeNode* current_node = new TreeNode;
                  current_node->father_location_index = 0;
                  current_node->id = searchingTree.size();
                  current_node->loadable = true;//当前节点是否加载
                  for(i = 0; i < int(strlen(file_path)); i++)
                  {
                      if(file_path[i] == '/')
                      {
                          memset(file_name, 0, strlen(file_name));
                          j = 0;
                          file_name[j] = '/';
                          continue;
                      }
                      file_name[j++] = file_path[i];
                  }

                  char* temp_name = new char[256];
                  char* temp_path = new char[256];
                  strcpy(temp_name,file_name);
                  strcpy(temp_path,file_path);
                  current_node->file_name = temp_name;
                  current_node->file_path = temp_path;
                  current_node->file_name_info=AnalysisFileName(file_name);
                  searchingTree.push_back(current_node);//error occur
              }
              else if(S_ISDIR(buf.st_mode))//判断的是目录 filefolder
              {
                  const char* const_file_path = file_path;//replace char* in constant char*;
                  RecurrentReadFiles(const_file_path);//recurrent read files
              }

          }
      }

      closedir(dir);
      return ;


}

bool DocumentManage::JudgeStringsEqual(char* char1,char* char2)//深度比较字符串是否相等
{
	bool IsEqual=false;
	int index=0;
	while (char1[index] == char2[index])
	{
		index++;
		if (index == int (strlen(char1)))
		{
			IsEqual=true;
		}

	}
	return IsEqual;
}
//识别字符串中的固定字符
FileNameInformation* DocumentManage::AnalysisFileName(char* filename)
{
	FileNameInformation* filename_info=new	FileNameInformation();
//	filename_info->filename=filename;
	filename_info->file_length=strlen(filename);

	for (int i = 0; i <int(strlen( filename)); i++)
	{
		if (filename[i]=='L')
		{
			filename_info->include_level_info=true;
			filename_info->level_info_position=i;
			filename_info->level_info[0]=filename[i];//mark_info的前三个元素代表的是文件的等级信息，level
			filename_info->level_info[1]=filename[i+1];
			filename_info->level_info[2]=filename[i+2];
			i=i+4;

			//char* tail_info;//找尾巴
			//vector<char> tail_info;//减5是因为减去小尾巴“.osgb”

			for (int j=i; j < (filename_info->file_length)-5; j++)//去掉小尾巴 “.osgb”之后，还要去掉“\0”，所以是减6
			{
				filename_info->tail_info.push_back(filename[j]);//获得小尾巴的信息
			}
			//filename_info->tail_info=tail_info;
			break;

		}
		else if (i == int(strlen(filename)-1)) //文件名中没有level信息
		{
			vector<char> tail_info;
			filename_info->include_level_info=false;
			filename_info->level_info[0]=0;
			filename_info->level_info[1]=0;
			filename_info->level_info[2]=0;

			filename_info->file_length=strlen(filename);
			filename_info->tail_info=tail_info;
			break;
		}
	}



	return filename_info;
}
