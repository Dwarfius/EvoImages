#include "Tree.h"

#pragma warning( disable : 4244 )

Tree::Tree()
{
	coeff = 1;
	head = Tree::Generate(0);
	string s = head->GetString();
	head->Simplify();
	s = head->GetString();
}

Tree::~Tree()
{
	delete head;
}

TreeNode* Tree::Generate(int depth) //static
{
	if(depth > Tree::MaxDepth)
		return NULL;

	NodeType type = depth == Tree::MaxDepth ? Value : (NodeType)(rand() % 7);
	TreeNode *node = new TreeNode(type, rand() % 21 - 10);
	if(node->GetType() != Value && node->GetType() != ArgX && node->GetType() != ArgY)
	{
		node->AddNode(true, Tree::Generate(depth+1));
		node->AddNode(false, Tree::Generate(depth+1));
	}
	return node;
}

float Tree::GetCoeff()
{
	return coeff;
}

void Tree::SetCoeff(float value)
{
	coeff = value;
}

string Tree::GetString()
{
	return head->GetString();
}

float Tree::GetResult(int x, int y)
{
	return head->Calculate(x, y) * coeff;
}