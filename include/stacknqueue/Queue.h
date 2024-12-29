/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Queue.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 22:33
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"
/*
 ! build code graph :   g++ -fsanitize=address -fsanitize=undefined -std=c++17
 -o main -Iinclude -Itest main.cpp test/unit_test/sort_topo/unit_test.cpp
 test/unit_test/sort_topo/test/*.cpp  -DTEST_SORT_TOPO

  ! build code topo : g++ -fsanitize=address -fsanitize=undefined -std=c++17 -o
 main -Iinclude -Itest main.cpp
 test/unit_test/graph/unit_test.cpptest/unit_test/graph/test/*.cpp  -DTEST_GRAPH
 */
// TODO
template <class T>
class Queue : public IDeck<T>
{
public:
    class Iterator; // forward declaration

protected:
    DLinkedList<T> list;
    void (*deleteUserData)(DLinkedList<T> *);
    bool (*itemEqual)(T &lhs, T &rhs);

public:
    Queue(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0)
    {
        this->itemEqual = itemEqual;
        this->deleteUserData = deleteUserData;
        list = DLinkedList<T>(deleteUserData, itemEqual);
    }
    void push(T item)
    {
        // TODO: add item to the rear
        list.add(item);
    }
    T pop()
    {
        // TODO: remove and return the front item
        if (list.size() <= 0)
            throw Underflow("Queue");
        T val = list.get(0);
        list.removeAt(0);
        return val;
    }
    T &peek()
    {
        // TODO: return the front item
        if (list.size() <= 0)
            throw Underflow("Queue");
        return list.get(0);
    }
    bool empty()
    {
        // TODO: check if the queue is empty
        return list.empty();
    }
    int size()
    {
        // TODO: return the number of items in the queue
        return list.size();
    }
    void clear()
    {
        // TODO: remove all items in the queue
        list.clear();
    }
    bool remove(T item)
    {
        // TODO: remove the item from the queue
        //      return true if the item is removed successfully
        return list.removeItem(item);
    }
    bool contains(T item)
    {
        // TODO: check if the item is in the queue
        return list.contains(item);
    }
    string toString(string (*item2str)(T &) = 0)
    {
        stringstream os;
        os << "FRONT-TO-REAR: " << list.toString(item2str);
        return os.str();
    }
    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }

    ///

    Iterator front()
    {
        return Iterator(this, true);
    }
    Iterator rear()
    {
        return Iterator(this, false);
    }

private:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////

public:
    // Iterator: BEGIN
    class Iterator
    {
    private:
        Queue<T> *queue;
        typename DLinkedList<T>::Iterator listIt;

    public:
        Iterator(Queue<T> *queue = 0, bool begin = true)
        {
            this->queue = queue;
            if (begin)
            {
                if (queue != 0)
                    this->listIt = queue->list.begin();
                else
                    this->listIt = 0;
            }
            else
            {
                if (queue != 0)
                    this->listIt = queue->list.end();
                else
                    this->listIt = 0;
            }
        }
        Iterator &operator=(const Iterator &iterator)
        {
            this->queue = iterator.queue;
            this->listIt = iterator.listIt;
            return *this;
        }

        T &operator*()
        {
            return *(this->listIt);
        }
        bool operator!=(const Iterator &iterator)
        {
            return this->listIt != iterator.listIt;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            listIt++;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
        void remove(void (*removeItem)(T) = 0)
        {
            listIt.remove(removeItem);
        }
    };
    // Iterator: END
};

#endif /* QUEUE_H */
