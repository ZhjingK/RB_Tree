#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<stdlib.h>
using namespace std;

enum Color
{
	RED,
	BLACK
};
template<class T,class V>
struct RBTreeNode
{
	T _key;
	V _val;
	Color _col;
	RBTreeNode<T,V>* _left;
	RBTreeNode<T,V>* _right;
	RBTreeNode<T,V>* _parent;

	RBTreeNode(const T& x,const V& val)
		:_key(x)
		, _val(val)
		, _col(RED)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
	{}
};
template<class T,class V>
class RBTree
{
	typedef RBTreeNode<T, V> Node;
public:
	RBTree()
		:_root(NULL)
	{}
	bool Insert(const T& key, const V& val)
	{
		if (_root == NULL)
		{
			_root = new Node(key, val);
			_root->_col = BLACK;
			return true;
		}
		//找需要插入节点的位置
		Node* cur = _root;
		Node* parent = NULL;
		while (cur)
		{
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return true;
			}
		}
		//判断到底插到parent节点的左子树还是右子树
		cur = new Node(key, val);
		if (parent->_key > key)
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		else if (parent->_key < key)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		//判断插入节点，父亲节点，叔叔节点之间节点的颜色是不是符合规
		while (parent&&parent->_col == RED)
		{
			Node* pparent = parent->_parent;
			Node* uncle = NULL;
			if (pparent->_left == parent)
			{
				uncle = pparent->_right;
			}
			else if (pparent->_right == parent)
			{
				uncle = pparent->_left;
			}
			//uncle节点分为两种情况
			//1.uncle节点存在且为RED
			if (uncle&&uncle->_col == RED)
			{
				pparent->_col = RED;
				parent->_col = uncle->_col = BLACK;
				cur = pparent;
				parent = cur->_parent;
			}
			//2.uncle节点不存在或存在为黑(不用再往上进行判断)
			if (uncle == NULL || uncle->_col == BLACK)
			{
				//parent在pparent的左，cur在parent的左
				if (pparent->_left == parent)
				{
					if (parent->_left == cur)
					{
						Rotate_R(pparent);
						parent->_col = BLACK;
						/*pparent->_col = RED;*/
					}
					else if (parent->_right == cur)
					{
						Rotate_LR(pparent);
						cur->_col = BLACK;
						/*pparent->_col = RED;*/
					}
				}
				else if (pparent->_right == parent)
				{
					if (parent->_left == cur)
					{
						Rotate_RL(pparent);
						cur->_col = BLACK;
						//pparent->_col = RED;
					}
					else if (parent->_right == cur)
					{
						Rotate_L(pparent);
						parent->_col = BLACK;
						//pparent->_col = RED;
					}
				}
				pparent->_col = RED;
			}
			_root->_col = BLACK;
		}
		return false;
	}
	void Rotate_L(Node*& parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* pparent = parent->_parent;
		parent->_right = subRL;
		if (subRL)
		{
			subRL->_parent = parent;
		}
		subR->_left = parent;
		parent->_parent = subR;
		if (parent == _root)
		{
			_root = subR;
			_root->_parent = NULL;
		}
		else
		{
			if (pparent->_left == parent)
			{
				pparent->_left = subR;
				subR->_parent = pparent;
			}
			else
			{
				pparent->_right = subR;
				subR->_parent = pparent;

			}
		}
	}
	void Rotate_R(Node*& parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* pparent = parent->_parent;
		parent->_left = subLR;
		if (subLR != NULL)
		{
			subLR->_parent = parent;
		}
		subL->_right = parent;
		parent->_parent = subL;
		if (parent == _root)
		{
			_root = subL;
			_root->_parent = NULL;
		}
		else
		{
			if (pparent->_left == parent)
			{
				pparent->_left = subL;
				subL->_parent = pparent;
			}
			else
			{
				pparent->_right = subL;
				subL->_parent = pparent;
			}
		}
	}
	void Rotate_LR(Node*& parent)
	{
		Rotate_L(parent->_left);
		Rotate_R(parent);
	}
	void Rotate_RL(Node*& parent)
	{
		Rotate_R(parent->_right);
		Rotate_L(parent);
	}
	//判断红黑树
	bool Is_RBTree()
	{
		int num = 0;
		int count = 0;
		Node* cur = _root;
		//以最左子树上的黑色节点做基准
		while (cur)
		{
			if (cur->_col == BLACK)
			{
				num++;
			}
			cur = cur->_left;
		}
		return _Is_RBTree(_root, count, num);
	}
	bool _Is_RBTree(Node* root, int count, int num)
	{
		//用递归去查找每一条子树上的黑色节点，到叶子节点的时候将所得黑节点的个数和基准做比较
		if (root == NULL)
		{
			if (count == num)
			{
				return true;
			}
			return false;
		}
		if (root->_col == BLACK)
		{
			++count;
		}
		if (root->_col == RED&&root->_parent&&root->_parent->_col == RED)
		{
			return false;
		}
		return _Is_RBTree(root->_left, count, num) && _Is_RBTree(root->_right, count, num);
	}
private:
	Node* _root;
};

int main()
{
	RBTree<int, int> r;
	int arr[] = { 8, 9, 1, 4, 2, 6, 3, 5 };
	int size = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < size; ++i)
	{
		r.Insert(arr[i], i);
	}
	cout << "IsRBTree" << r.Is_RBTree() << endl;
	system("pause");
}
