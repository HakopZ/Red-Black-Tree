#pragma once
#include <memory>
#include <string>
#include <vector>
template<typename T>
struct Node
{
public:
	std::shared_ptr<Node<T>> Left{};
	std::shared_ptr<Node<T>> Right{};
	T Value{};
	bool isRed{};
	Node(T val, std::shared_ptr<Node<T>> l = nullptr, std::shared_ptr<Node<T>> r = nullptr, bool isR = true)
	{
		Value = val;
		Left = std::move(l);
		Right = std::move(r);
		isRed = isR;
	}
	bool isLeaf()
	{
		return !Left && !Right;
	}
};

template<typename T>
class RBTree
{
public: 
	std::shared_ptr<Node<T>> Root{};
	int Count{};
	RBTree();
	void Add(T val);
	bool Remove(T val);
	bool Contains(T val);
	std::vector<T> PreOrder();
private:
	bool Contains(T val, std::shared_ptr<Node<T>> curr);
	std::shared_ptr<Node<T>> add(T val, std::shared_ptr<Node<T>> curr);
	std::shared_ptr<Node<T>> remove(T val, std::shared_ptr<Node<T>> curr);
	void FlipColor(std::shared_ptr<Node<T>> node);
	std::shared_ptr<Node<T>> FixUp(std::shared_ptr<Node<T>> node);
	std::shared_ptr<Node<T>> RotateLeft(std::shared_ptr<Node<T>> node);
	std::shared_ptr<Node<T>> RotateRight(std::shared_ptr<Node<T>> node);
	std::shared_ptr<Node<T>> MoveRedLeft(std::shared_ptr<Node<T>> node);
	std::shared_ptr<Node<T>> MoveRedRight(std::shared_ptr<Node<T>> node);
	std::shared_ptr<Node<T>> Minimum(std::shared_ptr<Node<T>> node);
	std::vector<T> Recurse(std::shared_ptr<Node<T>> curr, std::vector<T> t);
};

template<typename T>
RBTree<T>::RBTree()
{
	Root = nullptr;
	Count = 0;
}
template<typename T>
std::vector<T> RBTree<T>::PreOrder()
{
	std::vector<T> values{};
	return Recurse(Root, values);
}
template<typename T>
std::vector<T> RBTree<T>::Recurse(std::shared_ptr<Node<T>> curr, std::vector<T> t)
{
	t.push_back(curr->Value);
	if (curr->Left) t = Recurse(curr->Left, t);
	if (curr->Right) t = Recurse(curr->Right, t);
	return t;
}
template<typename T>
bool RBTree<T>::Contains(T val)
{
	return Contains(val, Root);
}
template<typename T>
bool RBTree<T>::Contains(T val, std::shared_ptr<Node<T>> curr)
{
	if (!curr)
		return false;
	if (val > curr->Value)
	{
		return Contains(val, curr->Right);
	}
	else if (val < curr->Value)
	{
		return Contains(val, curr->Left);
	}
	return true;
}
template<typename T>
std::shared_ptr<Node<T>> RBTree<T>::Minimum(std::shared_ptr<Node<T>> Node)
{
	auto current = Node;
	while (Node->Left)
	{
		Node = Node->Left;
	}
	return current;
}
template<typename T>
void RBTree<T>::Add(T val)
{
	Root = std::move(add(val, Root));
	Root->isRed = false;
	Count++;
}

template<typename T>
std::shared_ptr<Node<T>> RBTree<T>::add(T val, std::shared_ptr<Node<T>> curr)
{
	if (!curr)
	{
		return std::move(std::make_unique<Node<T>>(val));
	}
	if (curr->Right && curr->Left && curr->Left->isRed && curr->Right->isRed)
	{
		FlipColor(curr);
	}
	if (val < curr->Value)
	{
		curr->Left = add(val, curr->Left);
	}
	else if (val >= curr->Value)
	{
		curr->Right = add(val, curr->Right);
	}
	if (curr->Right && curr->Right->isRed)
	{
		curr = std::move(RotateLeft(curr));
	}
	if (curr->Left && curr->Left->Left && curr->Left->isRed && curr->Left->Left->isRed)
	{
		curr = std::move(RotateRight(curr));
	}

	return curr;
}
template<typename T>
bool RBTree<T>::Remove(T val)
{
	if (!Contains(val))
	{
		return false;
	}
	Root = std::move(remove(val, Root));
	Count--;
	return true;
}
template<typename T>
std::shared_ptr<Node<T>> RBTree<T>::remove(T val, std::shared_ptr<Node<T>> curr)
{
	if (val < curr->Value)
	{
		if (curr->Left && curr->Left->Left && !curr->Left->isRed && !curr->Left->Left->isRed)
		{
			curr = std::move(MoveRedLeft(curr));
		}
		curr->Left = std::move(remove(val, curr->Left));
	}
	else
	{
		if (curr->Left && curr->Left->isRed)
		{
			curr = std::move(RotateRight(curr));
		}
		if (val == curr->Value && curr->isLeaf())
		{
			curr = std::move(nullptr);
			return curr;
		}
		if (curr->Right)
		{
			if (curr->Right->Left && !curr->Right->isRed && !curr->Right->Left->isRed)
			{
				curr = std::move(MoveRedRight(curr));
			}
			if (curr->Value == val)
			{
				auto minNode = Minimum(curr->Right);
				curr->Value = minNode->Value;
				curr->Right = std::move(remove(minNode->Value, curr->Right));
			}
			else
			{
				curr->Right = std::move(remove(val, curr->Right));
			}
		}
	}

	return FixUp(curr);

}
template<typename T>
std::shared_ptr<Node<T>> RBTree<T>::MoveRedLeft(std::shared_ptr<Node<T>> node)
{
	FlipColor(node);
	if (node->Right && node->Right->Left && node->Right->isRed && node->Right->Left->isRed)
	{
		node->Right = std::move(RotateRight(node->Right));
		node = std::move(RotateLeft(node));
		FlipColor(node);
	}
	if (node->Right && node->Right->Right && node->Right->Right->isRed)
	{
		node->Right = std::move(RotateLeft(node->Right));
	}
	return node;
}
template<typename T>
std::shared_ptr<Node<T>> RBTree<T>::MoveRedRight(std::shared_ptr<Node<T>> node)
{
	FlipColor(node);
	if (node->Left && node->Left->Left && !node->Left->Left->isRed)
	{
		node = std::move(RotateRight(node));
		FlipColor(node);
	}
	return node;
}
template<typename T>
std::shared_ptr<Node<T>> RBTree<T>::RotateLeft(std::shared_ptr<Node<T>> node)
{
	auto curr = node->Right;
	curr->isRed = node->isRed;
	node->isRed = true;
	node->Right = std::move(curr->Left);

	auto TempN = node;
	curr->Left = std::move(TempN);
	
	return curr;
}

template<typename T>
std::shared_ptr<Node<T>> RBTree<T>::RotateRight(std::shared_ptr<Node<T>> node)
{
	auto curr = node->Left;
	curr->isRed = node->isRed;
	node->isRed = true;
	node->Left = std::move(curr->Right);

	auto TempN = node;
	curr->Right = std::move(TempN);

	return curr;
}

template<typename T>
void RBTree<T>::FlipColor(std::shared_ptr<Node<T>> node)
{
	node->isRed = !node->isRed;
	if (node->Left)
	{
		node->Left->isRed = !node->Left->isRed;
	}
	if (node->Right)
	{
		node->Right->isRed = !node->Right->isRed;
	}
}

template<typename T>
std::shared_ptr<Node<T>> RBTree<T>::FixUp(std::shared_ptr<Node<T>> node)
{
	if (node->Right && node->Right->isRed)
	{
		node = std::move(RotateLeft(node));
	}
	if (node->Left && node->Left->Left && node->Left->isRed && node->Left->Left->isRed)
	{
		node = std::move(RotateRight(node));
	}
	if (node->Right && node->Left && node->Right->isRed && node->Left->isRed)
	{
		FlipColor(node);
	}
	if (node->Left && node->Left->Right && node->Left->Left && node->Left->Right->isRed && !node->Left->Left->isRed)
	{
		node->Left = std::move(RotateLeft(node->Left));
		if (node->Left->isRed)
		{
			node = std::move(RotateRight(node));
		}
	}

	return node;
}
