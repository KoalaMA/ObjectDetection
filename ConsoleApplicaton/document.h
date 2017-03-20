#ifndef READOSGFILES_H
#define READOSGFILES_H

#endif // READOSGFILES_H

#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <tree.h>

using namespace std;
//class DocumentManage;

class DocumentManage
{
//    Q_OBJECT

//  signals:
//    void begin();

  public:
    DocumentManage();
    ~DocumentManage();

  public/* slots*/:
    std::vector <TreeNode*> OnFileFolderOpen();
    char* CutTheTail(char* file_path);
    bool JudgeStringsEqual(char* char1,char* char2);
    void copyString(string string1,char* string2) ;
    void initialChildrenParent(char children_path[256]);
    void initarray(char s[],char myarray[]);
    void RecurrentReadFiles(const char* path);
    void FindFatherAndChildrenNode(vector<TreeNode*> searching_tree);
    FileNameInformation* AnalysisFileName(char* file_name);
    bool MatchFileName(TreeNode* ergodic_element,TreeNode* current_element);
    //static bool PreMatchFileName(TreeNode* ergodic_element,TreeNode* current_element);
    int GetPositionIndex(int myArray[]);

    void InitialStringValue(char string[]);


  public:
    std::vector <TreeNode*> searchingTree;//搜索树对象

//    std::vector <TreeNode*>::iterator Iter;//iterator


  protected:


};
