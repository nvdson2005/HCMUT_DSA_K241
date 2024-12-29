/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
    class BWDIterator
    {
        // TODO implement
    private:
        Node *current_node;
        DLinkedList<T> *plist;

    public:
        BWDIterator(DLinkedList<T> *plist, bool begin)
        {
            if (begin)
            {
                if (plist != 0)
                {
                    this->current_node = plist->tail->prev;
                }
                else
                    this->current_node = 0;
            }
            else
            {
                if (plist != 0)
                {
                    this->current_node = plist->head;
                }
                else
                    this->current_node = 0;
            }
        };
        BWDIterator &operator=(const BWDIterator &iterator)
        {
            this->plist = iterator.plist;
            this->current_node = iterator->current_node;
            return *this;
        }
        BWDIterator &operator--()
        {
            if (current_node == nullptr)
            {
                current_node = plist->tail;
            }
            else
            {
                current_node = current_node->prev;
            }
            return *this;
        }
        BWDIterator operator--(int)
        {
            BWDIterator iterator = *this;
            --*this;
            return iterator;
        }
        BWDIterator &operator++()
        {
            return --*this;
        }
        BWDIterator operator++(int)
        {
            BWDIterator iterator = *this;
            --*this;
            return iterator;
        }
        bool operator!=(const BWDIterator &iterator)
        {
            return current_node != iterator.current_node;
        }
        T &operator*()
        {
            return current_node->data;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            current_node->prev->next = current_node->next;
            current_node->next->prev = current_node->prev;
            Node *curr_Next = current_node->next; // MUST next, so iterator-- will go to end
            if (removeItemData != 0)
                removeItemData(current_node->data);
            delete current_node;
            current_node = nullptr;
            current_node = curr_Next;
            plist->count -= 1;
        }
    };
};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    // TODO
    this->itemEqual = itemEqual;
    this->deleteUserData = deleteUserData;
    count = 0;
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->next = nullptr;
    head->prev = nullptr;
    tail->prev = head;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // TODO
    copyFrom(list);
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    // TODO
    if (this != &list)
    {
        copyFrom(list);
    }
    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    // TODO
    if (deleteUserData)
    {
        deleteUserData(this);
    }
    // TODO implement
    Node *tmp = head->next;
    while (tmp != tail)
    {
        Node *tmp2 = tmp->next;
        delete tmp;
        tmp  = tmp2;
    }
    delete head;
    delete tail;
    head = nullptr;
    tail = nullptr;
    itemEqual = nullptr;
    deleteUserData = nullptr;
    count = 0;
}

template <class T>
void DLinkedList<T>::add(T e)
{
    // TODO
    Node *tmp = new Node(e);
    tmp->prev = tail->prev;
    tail->prev->next = tmp;
    tmp->next = tail;
    tail->prev = tmp;
    count++;
}
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    // TODO
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    /**
     * Returns the node preceding the specified index in the doubly linked list.
     * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
     * Efficiently navigates to the node by choosing the shorter path based on the index's position.
     */
    // TODO
    if (index < 0 || index > count - 1)
    throw std::out_of_range("The index is out of range!");
  if (index == 0)
    return head;

  if (index <= count / 2)
  {
    // The equal operator means that it also includes the middle node in case of odd number of nodes
    Node *tmp = head->next;
    for (int i = 0; i < index; i++)
    {
      tmp = tmp->next;
    }
    return tmp->prev;
  }
  else
  {
    Node *tmp = tail->prev;
    for (int i = count - 1; i > index; i--)
    {
      tmp = tmp->prev;
    }
    return tmp->prev;
  }
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    // TODO
    if (!(index >= 0 && index < count))
        throw std::out_of_range("Index is out of range!");
    if (index == 0)
    {
        Node *tmp = head->next;
        head->next = tmp->next;
        tmp->next->prev = head;
        T val = tmp->data;
        delete tmp;
        count--;
        return val;
    }
    if (index == count - 1)
    {
        Node *tmp = tail->prev;
        tmp->prev->next = tail;
        tail->prev = tmp->prev;
        T val = tmp->data;
        delete tmp;
        count--;
        return val;
    }
    Node *tmp = getPreviousNodeOf(index);
    Node *deleteNode = tmp->next;
    tmp->next = deleteNode->next;
    deleteNode->next->prev = tmp;
    T value = deleteNode->data;
    delete deleteNode;
    count--;
    return value;
}

template <class T>
bool DLinkedList<T>::empty()
{
    // TODO
    return (count <= 0);
}

template <class T>
int DLinkedList<T>::size()
{
    // TODO
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    // TODO
    Node *tmp = head->next;
    while (tmp != tail)
    {
        Node *tmp2 = tmp->next;
        delete tmp;
        tmp = tmp2;
    }
    count = 0;
    head->next = tail;
    tail->prev = head;
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    // TODO
    if (!(index >= 0 && index < count))
        throw std::out_of_range("Index is out of range!");
    Node *tmp = getPreviousNodeOf(index);
    return tmp->next->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    // TODO
    Node *tmp = head->next;
    int cnt = 0;
    while (tmp != tail)
    {
        if (itemEqual && itemEqual(tmp->data, item))
            return cnt;
        else if (tmp->data == item)
            return cnt;
        tmp = tmp->next;
        cnt++;
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    // TODO
    Node *tmp = head->next;
    if (itemEqual)
    {
        while (tmp != tail)
        {
            if (itemEqual(tmp->data, item))
                break;
            tmp = tmp->next;
        }
    }
    else
    {
        while (tmp != tail)
        {
            if (tmp->data == item)
                break;
            tmp = tmp->next;
        }
    }
    if (tmp && tmp != tail)
    {
        if (removeItemData)
        {
            removeItemData(tmp->data);
        }
        removeAt(indexOf(item));
        return true;
    }
    else
        return false;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    // TODO
    Node *tmp = head->next;
    while (tmp != tail)
    {
        if (itemEqual)
        {
            if (itemEqual(tmp->data, item))
                return true;
        }
        else if (tmp->data == item)
            return true;
        tmp = tmp->next;
    }
    return false;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    // TODO
    if (empty())
        return "[]";
    stringstream ss;
    ss << "[";
    Node *tmp = head->next;
    if (!item2str)
    {
        while (tmp != tail)
        {
            if (tmp->next == tail)
            {
                ss << tmp->data;
                ss << "]";
                break;
            }
            ss << tmp->data << ", ";
            tmp = tmp->next;
        }
    }
    else
    {
        while (tmp != tail)
        {
            if (tmp->next == tail)
            {
                string num = item2str(tmp->data);
                ss << num << "]";
                break;
            }
            string num = item2str(tmp->data);
            ss << num << ", ";
            tmp = tmp->next;
        }
    }
    return ss.str();
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    /**
     * Copies the contents of another doubly linked list into this list.
     * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
     * Iterates through the source list and adds each element, preserving the order of the nodes.
     */
    // TODO
    count = 0;
    head = new Node();
    tail = new Node();
    head->prev = nullptr;
    tail->next = nullptr;
    head->next = tail;
    tail->prev = head;
    this->itemEqual = list.itemEqual;
    this->deleteUserData = list.deleteUserData;
    this->count = list.count;
    // In case the head of the list does not contain user's data
    Node *headlist2 = list.head->next;
    Node *headlist1 = this->head;
    while (headlist2 && headlist2 != list.tail)
    {
        Node *tmp = new Node(headlist2->data);
        headlist1->next = tmp;
        tmp->prev = headlist1;
        headlist1 = tmp;
        tmp->next = tail;
        tail->prev = tmp;
        headlist2 = headlist2->next;
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    /**
     * Clears the internal data of the list by deleting all nodes and user-defined data.
     * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
     * Traverses and deletes each node between the head and tail to release memory.
     */
    // TODO
    if (deleteUserData)
    {
        deleteUserData(this);
    }
    Node *tmp = head->next;
    while (tmp != tail)
    {
        Node *tmp2 = tmp;
        tmp = tmp->next;
        delete tmp2;
    }
    count = 0;
    delete head;
    delete tail;
    head = nullptr;
    tail = nullptr;
    deleteUserData = nullptr;
    itemEqual = nullptr;
}

#endif /* DLINKEDLIST_H */