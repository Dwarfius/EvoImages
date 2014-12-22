#include "Tree.h"

#pragma warning ( disable: 4244 ) //int to float conversion

Tree::Tree()
{
	coeff = 1;
	head = Tree::Generate(0);
	head->Simplify();
	tNext = NULL;
}

Tree::~Tree()
{
	delete head;
	delete tNext;
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

void Tree::ModifyCoeff(float mod)
{
	coeff *= mod;
	if(tNext != NULL)
		tNext->ModifyCoeff(mod);
}

string Tree::GetString()
{
	string res = to_string(static_cast<long double>(coeff));
	res += " * (" + head->GetString() + ")";
	if(tNext != NULL)
		res += " + " + tNext->GetString();
	return res;
}

float Tree::GetResult(int x, int y)
{
	float res = head->Calculate(x, y) * coeff;
	if(tNext != NULL)
		res += tNext->GetResult(x, y);
	return res;
}

void Tree::AttachTrees(Tree *t1, Tree *t2)
{
	if(tNext != NULL)
	{
		tNext->AttachTrees(t1, t2);
	}
	else if(t1 != NULL)
	{
		tNext = t1;
		tNext->AttachTrees(t2, NULL);
	}
}