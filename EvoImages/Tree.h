#include <stdlib.h>
#include <string>
#include "TreeNode.h"

using namespace std;

class Tree
{
public:
	Tree();
	~Tree();

	void ModifyCoeff(float mod);
	string GetString();
	float GetResult(int x, int y);
	void AttachTrees(Tree *t1, Tree *t2);

	static const int MaxDepth = 5;
private:
	static TreeNode* Generate(int depth);

	float coeff;
	TreeNode *head;
	Tree *tNext;
};