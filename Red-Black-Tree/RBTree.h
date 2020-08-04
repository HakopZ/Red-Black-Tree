#pragma once
#include <memory>
template<typename T>
struct Node
{
public:
	std::unique_ptr<Node<T>> Left{};
	std::unique_ptr<Node<T>> Right{};
	T Value{};
	bool isRed{};
	Node(T val, std::unique_ptr<Node<T>> l = nullptr, std::unique_ptr<Node<T>> r = nullptr, bool isR = true)
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
	std::unique_ptr<Node<T>> Root{};
	int Count{};
	RBTree();
	void Add(T val);
	void Remove(T val);
private:
	std::unique_ptr<Node<T>> add(T val, Node<T>* curr);
	void FlipColor(Node<T>* node);
	void RotateLeft(Node<T>* node);
	void RotateRight(Node<T>* node);
	
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
	if (!Root)
	{
		Root = std::make_unique<Node<T>>(val);
	}
	else
	{
		Root = add(val, Root.get());
	}
	Count++;
}

template<typename T>
std::unique_ptr<Node<T>> RBTree<T>::add(T val, Node<T>* curr)
{
	if (!curr)
	{
		return std::move(std::make_unique<Node<T>>(val));
	}
	if (curr->Left->isRed && curr->Right->isRed)
	{
		FlipColor(curr);
	}
	if (val < curr->Value)
	{
		curr->Left = add(val, curr->Left.get());
	}
	else if (val >= curr->Value)
	{
		curr->Right = add(val, curr->Right.get());
	}
	if (curr->Right->isRed)
	{
		RotateLeft(curr);
	}
	if (curr->Left->isRed && curr->Left->Left->isRed)
	{
		RotateRight(curr);
	}

	return std::move(std::make_unique<Node<T>>(curr->Value));
}

template<typename T>
void RBTree<T>::RotateLeft(Node<T>* node)
{
	auto curr = node->Right.get();
	
	node->Right = std::move(curr->Left);
	curr->Left = std::move(std::make_unique<Node<T>>(node));
	bool t = curr->isRed;
	curr->isRed = node->isRed;
	node->isRed = t;
	if (node->Value == Root->Value)
	{
		Root = std::move(std::make_unique<Node<T>>(node));
	}
}

template<typename T>
void RBTree<T>::RotateRight(Node<T>* node)
{
	auto curr = node->Left.get();

	node->Left = std::move(curr->Right);
	curr->Right = std::move(std::make_unique<Node<T>>(node));
	bool t = curr->isRed;
	curr->isRed = node->isRed;
	node->isRed = t;
	if (node->Value == Root->Value)
	{
		Root = std::move(std::make_unique<Node<T>>(node));
	}
}

template<typename T>
void RBTree<T>::FlipColor(Node<T>* node)
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
