#include "TreeNode.h"

TreeNode::TreeNode(NodeType type, float value)
{
	this->ChangeNode(type, value);
	parent = leftNode = rightNode = NULL;
}

void TreeNode::Release()
{
	if(leftNode)
	{
		delete leftNode;
		delete rightNode;
		leftNode = rightNode = NULL;
	}
}

void TreeNode::AddNode(bool isLeft, TreeNode *node)
{
	if(isLeft)
	{
		if(leftNode)
			delete leftNode;
		leftNode = node;
	}
	else
	{
		if(rightNode)
			delete rightNode;
		rightNode = node;
	}
	node->SetParent(this);
}

void TreeNode::SetParent(TreeNode *parent)
{
	this->parent = parent;
}

void TreeNode::ChangeNode(NodeType type, float value)
{
	this->value = 0;
	this->type = type;
	if(this->type == Value)
		this->value = value;
}

float TreeNode::Calculate(int x, int y)
{
	if(type == Value)
		return value;
	else if(type == ArgX)
		return (float)x;
	else if(type == ArgY)
		return (float)y;
	else
	{
		float leftResult = leftNode->Calculate(x, y);
		float rightResult = rightNode->Calculate(x, y);
		switch(type)
		{
		case Add:
			return leftResult + rightResult;
		case Substract:
			return leftResult - rightResult;
		case Multiply:
			return leftResult * rightResult;
		case Divide:
			if(rightResult != 0)
				return leftResult / rightResult;
			else
				return 0;
		default:
			return 0;
		}
	}
}

void TreeNode::Simplify()
{
	if(leftNode)
	{
		leftNode->Simplify();
		rightNode->Simplify();

		bool sameNode = leftNode->GetType() == rightNode->GetType();
		if(sameNode)
		{ //same node simplifications
			if(leftNode->GetType() == Value)
			{ //removing useless calculation
				float res = Calculate(0, 0);
				ChangeNode(Value, res);
				Release();
			}
			else if(leftNode->GetType() == ArgX || leftNode->GetType() == ArgY)
			{ //x-x = 0 || y/y = 1
				if(type == Substract)
				{
					ChangeNode(Value, 0);
					Release();
				}
				else if(type == Divide)
				{
					ChangeNode(Value, 1);
					Release();
				}
			}
		}
		else
		{
			if(type == Multiply)
			{
				if((leftNode->GetType() == Value && leftNode->GetValue() == 0) ||
					(rightNode->GetType() == Value && leftNode->GetValue() == 0))
				{ //multiplication by 0
					ChangeNode(Value, 0);
					Release();
				}
				else if(leftNode->GetType() == Value && leftNode->GetValue() == 1)
					BringNodeUp(false);
				else if(rightNode->GetType() == Value && rightNode->GetValue() == 1)
					BringNodeUp(true);
			}
		}
	}
}

string TreeNode::GetString()
{
	if(type == Value)
		return to_string(static_cast<long double>(value));
	else if(type == ArgX)
		return "x";
	else if(type == ArgY)
		return "y";
	string result = "(";
	result += leftNode->GetString();
	switch(type)
	{
	case Add:
		result += " + ";
		break;
	case Substract:
		result += " - ";
		break;
	case Multiply:
		result += " * ";
		break;
	case Divide:
		result += " / ";
		break;
	default:
		result += " # ";
		break;
	}
	result += rightNode->GetString() + ")";
	return result;
}

//shifts the left/right node up, discarding this
void TreeNode::BringNodeUp(bool isLeft) 
{
	if(!parent)
		return;

	TreeNode *node;
	if(isLeft)
	{
		node = leftNode;
		delete rightNode;
		rightNode = NULL;
	}
	else
	{
		node = rightNode;
		delete leftNode;
		leftNode = NULL;
	}
	parent->AddNode(parent->IsLeftChild(this), node);
}

bool TreeNode::IsLeftChild(TreeNode *node)
{
	return leftNode == node;
}