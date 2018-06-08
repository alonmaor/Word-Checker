
#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"

	
template <typename T>
class AVLSet : public Set<T>
{
public:
    // Initializes an AVLSet to be empty.
    AVLSet();

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.
    virtual void add(const T& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.
    virtual bool contains(const T& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.
    int height() const;


    // preorder() visits all of the elements in the AVL tree in preorder,
    // calling the given "visit" function and passing it each element.
    void preorder(std::function<void(const T&)> visit) const;


    // inorder() visits all of the elements in the AVL tree in order
    // calling the given "visit" function and passing it each element.
    void inorder(std::function<void(const T&)> visit) const;


    // postorder() visits all of the elements in the AVL tree in postorder
    // calling the given "visit" function and passing it each element.
    void postorder(std::function<void(const T&)> visit) const;


private:
    struct Node
    {
        Node* left = nullptr;
        T value;
        int height = 0;
        Node* right =  nullptr;
    };
	
    Node* root;

	void addNode(Node* curr, const T& element);
    void copy(Node* copyNode, const Node* originNode);
	void removeAll(Node* curr);
	bool containsR(const T& element,Node* curr) const;
    Node* createNode(const T& element);
    int nodeHeight(Node* node) const;
	void inorderR(std::function<void(const T&)> visit, Node* curr) const;
	void preorderR(std::function<void(const T&)> visit, Node* curr) const;
	void postorderR(std::function<void(const T&)> visit, Node* curr) const;
	Node* llRotation(Node* unbalanced);
    Node* rrRotation(Node* unbalanced);
    Node* rlRotation(Node* unbalanced);
    Node* lrRotation(Node* unbalanced);
	bool balanceVal(Node* node) const;
	int checkBalance(Node* node) const;
    void balance(Node* parent, int bal);
};


template <typename T>
AVLSet<T>::AVLSet() : root{nullptr}
{
}


template <typename T>
AVLSet<T>::~AVLSet() noexcept
{
    if (root != nullptr)
        removeAll(root);
}


template <typename T>
AVLSet<T>::AVLSet(const AVLSet& s)
{
    if(s.root != nullptr)
	    copy(root, s.root);
    else
        root = nullptr;
}


template <typename T>
AVLSet<T>::AVLSet(AVLSet&& s) noexcept
{
    if (s.root != nullptr)
        std::swap(root, s.root);
    else
        root = nullptr;
}


template <typename T>
AVLSet<T>& AVLSet<T>::operator=(const AVLSet& s)
{
    if (this != &s && s.root != nullptr)
    {
        Node* newRoot;
        copy(newRoot, s.root);
        removeAll(root);
        root = newRoot;
    }
    return *this;
}


template <typename T>
AVLSet<T>& AVLSet<T>::operator=(AVLSet&& s) noexcept
{
    std::swap(root, s.root);
    return *this;
}


template <typename T>
bool AVLSet<T>::isImplemented() const noexcept
{
    return true;
}


template <typename T>
void AVLSet<T>::add(const T& element)
{	
	if(root == nullptr)
        root = createNode(element);
    else
 	{
		Node* curr = root;
		addNode(curr, element);
	}  		 
}

template <typename T>
void AVLSet<T>::addNode(Node* curr, const T& element)
{
	if(curr->value < element)
	{
		if(curr->right == nullptr)
		{
			curr->right = createNode(element);
            curr->height = 1;
            return;
		}
		else
			addNode(curr->right,element);
            curr->height = nodeHeight(curr);
	}
	else if(curr->value > element)
	{
		if(curr->left == nullptr)
		{
			curr->left = createNode(element);
			curr->height = 1;
            return;
		}
		else
			addNode(curr->left,element);
            curr->height = nodeHeight(curr);
	}
	else
		return;
    /*
	if(curr->right != nullptr){
		int bal = checkBalance(curr->right);
        if (bal != 0)
            balance(curr, bal);
    }
	if(curr->left != nullptr)
    {
		int bal = checkBalance(curr->left);
        if (bal != 0)
            balance(curr, bal);
    }
    curr->height = nodeHeight(curr);
    */
}


template <typename T>
bool AVLSet<T>::contains(const T& element) const
{
    if (root == nullptr)
        return false;
	Node* curr = root;
	return containsR(element,curr);
}


template <typename T>
bool AVLSet<T>::containsR(const T& element,Node* curr) const
{   
    if(curr->value < element)
    {   
        if(curr->right == nullptr)
            return false;
        else
            return containsR(element, curr->right);
    }
    else if(curr->value > element)
    {   
        if(curr->left == nullptr)
            return false;
        else
            return containsR(element,curr->left);
    }
    else
        return true;
}


template <typename T>
unsigned int AVLSet<T>::size() const noexcept
{
    return 0;
}


template <typename T>
int AVLSet<T>::height() const
{
    if (root == nullptr)
        return -1;
    return root->height;
}


template <typename T>
void AVLSet<T>::preorder(std::function<void(const T&)> visit) const
{
	if (root != nullptr)
		preorderR(visit,root);
}


template <typename T>
void AVLSet<T>::preorderR(std::function<void(const T&)> visit, Node* curr) const
{
	visit(curr->value);
   	if(curr->left != nullptr)
        preorderR(visit,curr->left);
    if(curr->right != nullptr)
        preorderR(visit,curr->right);
}


template <typename T>
void AVLSet<T>::inorder(std::function<void(const T&)> visit) const
{
	if (root != nullptr)
        inorderR(visit,root);
}


template <typename T>
void AVLSet<T>::inorderR(std::function<void(const T&)> visit, Node* curr) const
{  
    if(curr->left != nullptr)
        inorderR(visit,curr->left);
	visit(curr->value);
    if(curr->right != nullptr)
        inorderR(visit,curr->right);
}


template <typename T>
void AVLSet<T>::postorder(std::function<void(const T&)> visit) const
{
	if (root != nullptr)
        postorderR(visit,root);
}


template <typename T>
void AVLSet<T>::postorderR(std::function<void(const T&)> visit, Node* curr) const
{
	if(curr->left != nullptr)
        postorderR(visit,curr->left);
    if(curr->right != nullptr)
        postorderR(visit,curr->right);
	visit(curr->value);
}


template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::createNode(const T& element)
{
    Node* new_ptr = nullptr;
    try
    {
        new_ptr = new Node;
        new_ptr->value = element;
    }catch(std::bad_alloc &)
    {
        delete new_ptr;
        throw;
    }
    return new_ptr; 
}


template <typename T>
void AVLSet<T>::copy(Node* copyNode, const Node* originNode)
{
    copyNode = createNode(originNode->value);
    copyNode->height = originNode->height;
    if(originNode->left != nullptr)
        copy(copyNode->left, originNode->left);
    if(originNode->right != nullptr)
        copy(copyNode->right, originNode->right);
}


template <typename T>
void AVLSet<T>::removeAll(Node* curr)
{
    if(curr->left != nullptr)
        removeAll(curr->left);
    if(curr->right != nullptr)
        removeAll(curr->right);
    delete curr;
}


template <typename T>
int AVLSet<T>::nodeHeight(Node* node) const
{
    if (node->right == nullptr && node->left == nullptr)
        return 0;
    else if (node->right != nullptr && node-> left == nullptr)
        return node->right->height+1;
    else if (node->right == nullptr && node-> left != nullptr)
        return node->left->height+1;
    else
    {
        if (node->right->height > node->left->height)
            return node->right->height+1;
        else
            return node->left->height+1;
    }
}

/*
template <typename T>
int AVLSet<T>::checkBalance(Node* node) const
{
    int balance = balanceVal(node);
    if(balance == 2)
    {
        balance = balanceVal(node->left);
        if(balance == 2)
            return 1; // ll
        else if(balance == -2)
            return 2; // lr
    }
    else if(balance == -2)
    {
        balance = balanceVal(node->left);
        if(balance == 2)
            return 3; // rl
        else if(balance == -2)
            return 4; // rr
    }
    return 0;
}


template <typename T>
bool AVLSet<T>::balanceVal(Node* node) const
{
    return nodeHeight(node->left) - nodeHeight(node->right);
}


template <typename T>
void AVLSet<T>::balance(Node* parent, int bal)
{
    if (bal == 1)
        llRotation(parent);
    else if (bal == 2)
        lrRotation(parent);
    else if (bal == 3)
        rlRotation(parent);
    else
        rrRotation(parent);
}


template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::llRotation(Node* parent)
{
    Node* temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
   	return temp; 
}


template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::rrRotation(Node* parent)
{
	Node* temp;
    temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    return temp;
}


template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::lrRotation(Node* parent)
{
	Node* temp;
    temp = parent->left;
    parent->left = rrRotation(temp);
    return llRotation(parent);
}


template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::rlRotation(Node* parent)
{
	Node* temp;
    temp = parent->right;
    parent->right = llRotation(temp);
    return rrRotation(parent);
}
*/

#endif // AVLSET_HPP

