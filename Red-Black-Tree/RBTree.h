#pragma once
#include <memory>
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
};

template<typename T>
class RBTree
{
public: 
	std::shared_ptr<Node<T>> Root{};
	int Count{};
	RBTree();
	void Add(T val);
	void Remove(T val);
private:
	std::shared_ptr<Node<T>> add(T val, std::shared_ptr<Node<T>> curr);
	void FlipColor(std::shared_ptr<Node<T>> node);
	std::shared_ptr<Node<T>> RotateLeft(std::shared_ptr<Node<T>> node);
	std::shared_ptr<Node<T>> RotateRight(std::shared_ptr<Node<T>> node);
	std::shared_ptr<Node<T>> MoveRedLeft(std::shared_ptr<Node<T>> node);
	std::shared_ptr<Node<T>> MoveRedRight(std::shared_ptr<Node<T>> node);
};

template<typename T>
RBTree<T>::RBTree()
{
	Root = nullptr;
	Count = 0;
}

template<typename T>
void RBTree<T>::Add(T val)
{
	Root = add(val, Root);
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
