
#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename T>
class HashSet : public Set<T>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const T
    // and returns an unsigned int.
    typedef std::function<unsigned int(const T&)> HashFunction;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.
    virtual void add(const T& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.
    virtual bool contains(const T& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const T& element, unsigned int index) const;


private:
	HashFunction hashFunction;
    struct hashNode
    {
        int key;
        T value;
        hashNode* next = nullptr;
    };
    hashNode** hashArr;
    int tableSize;
    int capacity;
	void copyAll(const HashSet& s);
    void resize();
    hashNode* createNode();
	void deleteAll(hashNode** arr);
};



template <typename T>
HashSet<T>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
	capacity = DEFAULT_CAPACITY;
	tableSize = 0;
    try
    {
        hashArr = new hashNode*[capacity];
    }catch(...)
    {
        delete hashArr;
        throw;
    }
    for (int i = 0; i < capacity; i++)
        hashArr[i] = nullptr;
}


template <typename T>
HashSet<T>::~HashSet() noexcept
{
	deleteAll(hashArr);
}


template <typename T>
HashSet<T>::HashSet(const HashSet& s)
    : hashFunction{nullptr}
{
	capacity = s.capacity;
	tableSize = s.tableSize;
	copyAll(s);
}


template <typename T>
HashSet<T>::HashSet(HashSet&& s) noexcept
    : hashFunction{nullptr}
{
	std::swap(hashArr, s.hashArr);
}


template <typename T>
HashSet<T>& HashSet<T>::operator=(const HashSet& s)
{
	capacity = s.capacity;
    tableSize = s.tableSize;
    copyAll(s);
    return *this;
}


template <typename T>
HashSet<T>& HashSet<T>::operator=(HashSet&& s) noexcept
{
	std::swap(hashArr, s.hashArr);
    return *this;
}


template <typename T>
bool HashSet<T>::isImplemented() const noexcept
{
    return true;
}


template <typename T>
void HashSet<T>::add(const T& element)
{
	int key = hashFunction(element);
	int index = key%capacity;
    if(isElementAtIndex(element,index))
        return;
    if (hashArr[index] != nullptr)
    {
        hashNode* curr = hashArr[index];
        while(curr->next != nullptr)
        { 
            curr = curr->next;
        }
		curr->next = createNode();
		curr->next->value = element;
		curr->next->key = key;
		tableSize++;
    }
	else
	{
		hashArr[index] = createNode();
		hashArr[index]->value = element;
		hashArr[index]->key = key;
		tableSize++;
	}

    if(double(tableSize)/capacity > 0.8)
        resize();
}


template <typename T>
bool HashSet<T>::contains(const T& element) const
{
    int key = hashFunction(element);
    int index = key%capacity;
    hashNode* curr = hashArr[index];
    while (curr != nullptr)
    {
        if (curr->value == element)
            return true;
        curr = curr->next;
    }
    return false;
}


template <typename T>
unsigned int HashSet<T>::size() const noexcept
{
    return tableSize;
}


template <typename T>
unsigned int HashSet<T>::elementsAtIndex(unsigned int index) const
{
	int count = 0;
    hashNode* curr = hashArr[index];
    while (curr != nullptr)
    {
		count++;		
        curr = curr->next;
    }
	return count;
}


template <typename T>
bool HashSet<T>::isElementAtIndex(const T& element, unsigned int index) const
{
    hashNode* curr = hashArr[index];
	while (curr != nullptr)
	{
		if (curr->value == element)
			return true;
		curr = curr->next;
	}
	return false;
}


template <typename T>
typename HashSet<T>::hashNode* HashSet<T>::createNode()
{
    hashNode* new_node = nullptr;
    try
    {
        new_node = new hashNode;
    }
    catch(...)
    {
        delete new_node;
        throw;
    }
    return new_node;
}


template <typename T>
void HashSet<T>::resize()
{
	int newCapacity = capacity*2;
   	hashNode** newArr;
    try
    {
        newArr = new hashNode*[newCapacity];
    }catch(...)
    {
        delete hashArr;
        throw;
    }
	for (int i = 0; i < newCapacity; i++)
		newArr[i] = nullptr;
	
	for (int i = 0; i < capacity; i++)
    {
		if(hashArr[i] != nullptr)
		{
			hashNode* originCurr = hashArr[i];
			while(originCurr != nullptr)
			{
				int newIndex = (originCurr->key)%newCapacity;
				if (newArr[newIndex] != nullptr)
				{
					hashNode* curr = newArr[newIndex];
					while(curr->next != nullptr)
					{
						curr = curr->next;
					}
					curr->next = createNode();
					curr->next->value = originCurr->value;
					curr->next->key = originCurr->key;
				}
				else
				{
					newArr[newIndex] = createNode();
			        newArr[newIndex]->value = originCurr->value;
                    newArr[newIndex]->key = originCurr->key;
				}
                originCurr = originCurr->next;
            }
	    }
	}
	deleteAll(hashArr);
    hashArr = newArr;
    capacity = newCapacity;
}


template <typename T>
void HashSet<T>::copyAll(const HashSet& s)
{
	try
	{
		hashArr = new hashNode*[capacity];
	}catch(...)
	{
		delete hashArr;
		throw;
	}
	
	for (int i = 0; i < capacity; i++)
	{
		hashNode* originCurr = s.hashArr[i];
		hashNode* copyCurr = hashArr[i];
		while(originCurr != nullptr)
		{
			try
			{
				copyCurr = new hashNode;
			}
			catch(...)
			{
				delete hashArr[i];
				throw;
			}
			copyCurr->value = originCurr->value;
			copyCurr->key = originCurr->key;
			copyCurr = copyCurr->next;
			originCurr = originCurr->next;
		}
    }
}

template <typename T>
void HashSet<T>::deleteAll(hashNode** arr)
{
	for (int i = 0; i < capacity; i++)
	{
		hashNode* curr = arr[i];
		while (curr != nullptr)
		{
			hashNode* temp = curr;
			curr = curr->next;
			delete temp;
		}
	}
}



#endif // HASHSET_HPP

