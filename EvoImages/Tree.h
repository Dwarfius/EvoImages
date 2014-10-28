#include <stdlib.h>
#include <string>
#include "TreeNode.h"

using namespace std;

class Tree
{
public:
	Tree();
	~Tree();

	float GetCoeff();
	void SetCoeff(float value);
	string GetString();
	float GetResult(int x, int y);

	static const int MaxDepth = 5;
private:
	static TreeNode* Generate(int depth);

	float coeff;
	TreeNode *head;
};