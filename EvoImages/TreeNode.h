#include <stdlib.h>
#include <string>

using namespace std;

enum NodeType { Value, Add, Substract, Multiply, Divide, ArgX, ArgY };

class TreeNode
{
public:
	TreeNode(NodeType type, float value);
	~TreeNode() { Release(); };

	void Release();
	void AddNode(bool isLeft, TreeNode *node);
	void SetParent(TreeNode *parent);
	void ChangeNode(NodeType type, float value);
	float Calculate(int x, int y);
	void Simplify();
	NodeType GetType() { return type; };
	float GetValue() { return value; };
	string GetString();
	bool IsLeftChild(TreeNode *node);
private:
	void BringNodeUp(bool isLeft);

	NodeType type;
	float value;
	TreeNode *leftNode, *rightNode, *parent;
};