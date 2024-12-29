/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
template <class T, class V>
using XHashMap = xMap<T, V>;

// TODO
template <class T>
class TopoSorter
{
public:
    static int DFS;
    static int BFS;

protected:
    DGraphModel<T> *graph;
    int (*hash_code)(T &, int);

public:
    TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0)
    {
        // TODO
        this->graph = graph;
        this->hash_code = hash_code;
    }

    DLinkedList<T> sort(int mode = 0, bool sorted = true)
    {
        // TODO
        DLinkedList<T> sortedList;
        if (mode == DFS)
        {
            sortedList = dfsSort(sorted);
        }
        else if (mode == BFS)
            sortedList = bfsSort(sorted);

        return sortedList;
    }

    DLinkedList<T> bfsSort(bool sorted = true)
    {
        // TODO
        // If sorted is true, sort in ascending order before doing topo sort.
        DLinkedList<T> vertices = this->graph->vertices();
        DLinkedListSE<T> SEVertices = DLinkedListSE<T>(vertices);
        if (sorted)
        {
            SEVertices.sort();
        }
        vertices.clear();

        // Reset and add the vertices back to vertices list.
        for (auto vertex : SEVertices)
        {
            vertices.add(vertex);
        }

        // Initialize an xMap to storing indegree of each vertex.
        xMap<T, int> inDegrees = vertex2inDegree(this->hash_code);

        // //Initialize a queue with all the vertices with 0 in-degree.
        Queue<T> q;

        // Get the list of vertices with 0 in-degree.
        DLinkedList<T> zeroDegrees = listOfZeroInDegrees();

        // If sorted is true, sort the zeroDegrees in ascending order.
        DLinkedListSE<T> SEZeroDegrees = DLinkedListSE<T>(zeroDegrees);
        if (sorted)
        {
            SEZeroDegrees.sort();
        }

        // Push the zeroDegrees into the queue.
        for (auto zeroVertex : SEZeroDegrees)
        {
            q.push(zeroVertex);
        }

        // Initialize a result list to store the sorted vertices.
        DLinkedList<T> result;

        // Start BFS
        while (!q.empty())
        {
            T topVertex = q.peek();
            q.pop();
            result.add(topVertex);
            DLinkedList<T> outwardEdges = this->graph->getOutwardEdges(topVertex);
            //DLinkedListSE<T> SEOutwardEdges = DLinkedListSE<T>(outwardEdges);

            // // If sorted is true, sort the outwardEdges in ascending order.
            // if (sorted)
            // {
            //     // SEOutwardEdges.sort();
            // }
            for (auto vertex : outwardEdges)
            {
                inDegrees.get(vertex)--;
                if (inDegrees.get(vertex) == 0)
                {
                    q.push(vertex);
                }
            }
        }

        // Return the result
        return result;
    }

    DLinkedList<T> dfsSort(bool sorted = true)
    {
        // TODO
        // If sorted is true, sort the vertex in ascending order before topo sort.
        DLinkedList<T> vertices = this->graph->vertices();
        DLinkedListSE<T> SEVertices = DLinkedListSE<T>(vertices);
        if (sorted)
        {
            SEVertices.sort();
        }
        vertices.clear();

        // Reset and add the vertices back to vertices list.
        for (auto vertex : SEVertices)
        {
            vertices.add(vertex);
        }

        // Initialize members for TopoSort
        Stack<T> s;
        xMap<T, bool> visited(this->hash_code);
        for (auto vertex : vertices)
        {
            visited.put(vertex, false);
        }

        // If sorted is true, sort the vertices in descending order.
        // else sort the vertices in ascending order.
        //  if (sorted)
        //  {
        //      for (auto it = vertices.bbegin(); it != vertices.bend(); it--)
        //      {
        //          if (!visited.get(*it))
        //          {
        //              DFSSortedRecursion(*it, visited, s);
        //          }
        //      }
        //  }
        //  else{
        //      for (auto vertex : vertices)
        //      {
        //          if (!visited.get(vertex))
        //          {
        //              DFSRecursion(vertex, visited, s);
        //          }
        //      }
        //  }
        for (auto vertex : vertices)
        {
            if (!visited.get(vertex))
            {
                DFSRecursion(vertex, visited, s);
            }
        }
        // Push into result to return.
        DLinkedList<T> result;
        while (!s.empty())
        {
            result.add(s.pop());
        }
        return result;
    }

    // Iterative approach for DFS is currently false, so it is not used.
    //  void DFSIterative(T start, xMap<T, bool>& visited, stack<T>& result){
    //      stack<T> st;
    //      st.push(start);
    //      while(!st.empty()){
    //          T vertex = st.top();
    //          st.pop();
    //          if(visited.get(vertex)) continue;
    //          visited.get(vertex) = true;
    //          bool allVisited = false;
    //          for(auto adV : this->graph->getOutwardEdges(vertex)){
    //              if(!visited.get(adV)){
    //                  allVisited = true;
    //                  st.push(adV);
    //              }
    //          }
    //          if(!allVisited && !st.empty()){
    //              result.push(st.top());
    //              st.pop();
    //          }
    //      }
    //  }

    // Recursive approach for DFS with unsorted vertices.
    void DFSRecursion(T vertex, xMap<T, bool> &visited, Stack<T> &result)
    {
        visited.get(vertex) = true;
        for (auto adV : this->graph->getOutwardEdges(vertex))
        {
            if (!visited.get(adV))
            {
                DFSRecursion(adV, visited, result);
            }
        }
        result.push(vertex);
    }

    // Recursive approach for DFS with sorted vertices.
    // Whenever taking out the adjacent vertices, sort them in ascending order.
    void DFSSortedRecursion(T vertex, xMap<T, bool> &visited, Stack<T> &result)
    {
        visited.get(vertex) = true;
        DLinkedList<T> adVertices = this->graph->getOutwardEdges(vertex);
        DLinkedListSE<T> SEVertices = DLinkedListSE<T>(adVertices);
        // SEVertices.sort();
        for (auto it = SEVertices.bbegin(); it != SEVertices.bend(); it--)
        {
            if (!visited.get(*it))
            {
                DFSSortedRecursion(*it, visited, result);
            }
        }
        result.push(vertex);
    }

protected:
    // Helper functions
    //  XHashMap<T, int> vertex2inDegree(int (*hash)(T&, int)){
    //      //TODO
    //  };
    //  XHashMap<T, int> vertex2outDegree(int (*hash)(T&, int)){
    //      XHashMap
    //      for(auto vertex : this->graph->nodeList){
    //          outDegrees.
    //      }
    //      return outDegrees;
    //  };
    xMap<T, int> vertex2inDegree(int (*hash)(T &, int))
    {
        xMap<T, int> inDegree(hash);
        DLinkedList<T> vertices = this->graph->vertices();
        for (auto vertex : vertices)
        {
            inDegree.put(vertex, graph->inDegree(vertex));
        }
        return inDegree;
    }
    xMap<T, int> vertex2outDegree(int (*hash)(T &, int))
    {
        xMap<T, int> outDegree(hash);
        for (auto vertex : this->graph->vertices())
        {
            outDegree.put(vertex, this->graph->outDegree(vertex));
        }
        return outDegree;
    }
    DLinkedList<T> listOfZeroInDegrees()
    {
        DLinkedList<T> zeroInDegrees;
        for (auto vertex : this->graph->vertices())
        {
            if (this->graph->inDegree(vertex) == 0)
            {
                zeroInDegrees.add(vertex);
            }
        }
        return zeroInDegrees;
    };

}; // TopoSorter
template <class T>
int TopoSorter<T>::DFS = 0;
template <class T>
int TopoSorter<T>::BFS = 1;

#endif /* TOPOSORTER_H */