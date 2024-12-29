/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"
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
class DLinkedListSE : public DLinkedList<T>
{
    using Node = typename DLinkedList<T>::Node;
public:
    DLinkedListSE(
        void (*removeData)(DLinkedList<T> *) = 0,
        bool (*itemEQ)(T &, T &) = 0) : DLinkedList<T>(removeData, itemEQ) {

                                        };

    DLinkedListSE(const DLinkedList<T> &list)
    {
        this->copyFrom(list);
    }

    void sort(int (*comparator)(T &, T &) = 0) {
        // TODO: implement this function
        //     - You should implement the merge sort algorithm
        if(this->size() <= 1) return;
        Node* left = this->head->next;
        Node* right = this->tail->prev;

        //Isolate from the head and tail node
        left->prev = nullptr;
        this->head->next = nullptr;
        right->next = nullptr;
        this->tail->prev = nullptr;

        left = MergeSort(left, comparator);

        //Reconnect to the head and tail node
        this->head->next = left;
        left->prev = this->head;
        Node* iter = left;
        while(iter->next != nullptr){
            iter = iter->next;
        }
        iter->next = this->tail;
        this->tail->prev = iter;
    };

    // Helper Functions:
    Node *split(Node *left)
    {
        if (left == nullptr)
            return nullptr;
        Node *slow = left;
        Node *fast = left;
        while (fast != NULL && fast->next != NULL && fast->next->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node *tmp = slow->next;
        slow->next = NULL;
        if (tmp != NULL)
            tmp->prev = NULL;
        return tmp;
    }
    Node *Merge(Node *first, Node *second, int (*comparator)(T &, T &) = 0)
    {
        if (first == NULL)
            return second;
        if (second == NULL)
            return first;
        if (compare(first->data, second->data, comparator) < 0)
        {
            first->next = Merge(first->next, second, comparator);
            if (first->next != nullptr)
                first->next->prev = first;
            first->prev = NULL;
            return first;
        }
        else if (compare(first->data, second->data, comparator) > 0)
        {
            second->next = Merge(first, second->next, comparator);
            if (second->next != nullptr)
                second->next->prev = second;
            second->prev = NULL;
            return second;
        } else {
            first->next = Merge(first->next, second, comparator);
            if (first->next != nullptr)
                first->next->prev = first;
            first->prev = NULL;
            return first;
        }
    }
    Node *MergeSort(Node *left, int (*comparator)(T &, T &) = 0)
    {
        if (left == nullptr || left->next == nullptr)
            return left;
        Node *right = split(left);
        left = MergeSort(left, comparator);
        right = MergeSort(right, comparator);
        return Merge(left, right, comparator);
    }

protected:
    static int compare(T &lhs, T &rhs, int (*comparator)(T &, T &) = 0)
    {
        if (comparator != 0)
            return comparator(lhs, rhs);
        else
        {
            if (lhs < rhs)
                return -1;
            else if (lhs > rhs)
                return +1;
            else
                return 0;
        }
    }
};

#endif /* DLINKEDLISTSE_H */
