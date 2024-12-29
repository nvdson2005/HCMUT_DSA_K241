/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"
// #include "stacknqueue/PriorityQueue.h"
#include "sorting/DLinkedListSE.h"
/*
 ! build code graph :   g++ -fsanitize=address -fsanitize=undefined -std=c++17
 -o main -Iinclude -Itest main.cpp test/unit_test/sort_topo/unit_test.cpp
 test/unit_test/sort_topo/test/*.cpp  -DTEST_SORT_TOPO

  ! build code topo : g++ -fsanitize=address -fsanitize=undefined -std=c++17 -o
 main -Iinclude -Itest main.cpp
 test/unit_test/graph/unit_test.cpptest/unit_test/graph/test/*.cpp  -DTEST_GRAPH
 */
// TODO
template<class T>
class DGraphModel: public AbstractGraph<T>{
using VertexNode = typename AbstractGraph<T>::VertexNode;
private:
public:
    DGraphModel(
            bool (*vertexEQ)(T&, T&), 
            string (*vertex2str)(T&) ): 
        AbstractGraph<T>(vertexEQ, vertex2str){

    }
    
    void connect(T from, T to, float weight=0){
        //TODO
        typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(to);
        if(!fromNode){
            stringstream vertexInfo;
            this->vertex2str != nullptr ? vertexInfo << this->vertex2str(from) : vertexInfo << from;
            throw VertexNotFoundException(vertexInfo.str());
        }
        if(!toNode){
            stringstream vertexInfo;
            this->vertex2str != nullptr ? vertexInfo << this->vertex2str(to) : vertexInfo << to;
            throw VertexNotFoundException(vertexInfo.str());
        }
        typename AbstractGraph<T>::Edge* newEdge = fromNode->getEdge(toNode);
        if(newEdge){
            newEdge->getWeight() = weight;
            return;
        }
        newEdge = new typename AbstractGraph<T>::Edge(fromNode, toNode, weight);
        fromNode->getAdList().add(newEdge);
        fromNode->getOutDegree()++;
        toNode->getInDegree()++;
    }
    void disconnect(T from, T to){
        //TODO
        typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(to);
        if(!fromNode){
            stringstream vertexInfo;
            this->vertex2str != nullptr ? vertexInfo << this->vertex2str(from) : vertexInfo << from;
            throw VertexNotFoundException(vertexInfo.str());
        }
        if(!toNode){
            stringstream vertexInfo;
            this->vertex2str != nullptr ? vertexInfo << this->vertex2str(to) : vertexInfo << to;
            throw VertexNotFoundException(vertexInfo.str());
        }
        typename AbstractGraph<T>::Edge* removeEdge = fromNode->getEdge(toNode);
        if(!removeEdge){
            typename AbstractGraph<T>::Edge throwEdge(fromNode, toNode, 0);
            throw EdgeNotFoundException(this->edge2Str(throwEdge));
        }
        if(fromNode == toNode){
            fromNode->getAdList().removeItem(removeEdge);
            fromNode->getInDegree()--;
            fromNode->getOutDegree()--;
            return;
        }
        else {
            fromNode->getAdList().removeItem(removeEdge);
            fromNode->getOutDegree()--;
            toNode->getInDegree()--;
            return;
        }
        // for(auto edge : fromNode->getAdList()){
        //     if(edge->getFrom() == fromNode && edge->getTo() == toNode){
        //         if(fromNode == toNode){
        //             fromNode->getAdList().removeItem(edge);
        //             fromNode->getInDegree()--;
        //             fromNode->getOutDegree()--;
        //             return;
        //         }
        //         fromNode->getAdList().removeItem(edge);
        //         fromNode->getOutDegree()--;
        //         toNode->getInDegree()--;
        //         return;
        //     }
        // }
        typename AbstractGraph<T>::Edge throwEdge(fromNode, toNode, 0);
        throw EdgeNotFoundException(this->edge2Str(throwEdge));
    }
    void remove(T vertex){
        //TODO
        typename AbstractGraph<T>::VertexNode* vertexNode = this->getVertexNode(vertex);
        if(vertexNode == nullptr){
            stringstream vertexInfo;
            this->vertex2str != nullptr ? vertexInfo << this->vertex2str(vertex) : vertexInfo << vertex;
            throw VertexNotFoundException(vertexInfo.str());
        }
        DLinkedList<T> connectedToVertices = this->getOutwardEdges(vertex);
        DLinkedList<T> connectedFromVertices = this->getInwardEdges(vertex);
        for(auto connectedToVertex : connectedToVertices){
            this->disconnect(vertex, connectedToVertex);
        }
        for(auto connectedFromVertex : connectedFromVertices){
            if(connectedFromVertex == vertex) continue;
            this->disconnect(connectedFromVertex, vertex);
        }
        this->nodeList.removeItem(vertexNode);
    }
    
    static DGraphModel<T>* create(
            T* vertices, int nvertices, Edge<T>* edges, int nedges,
            bool (*vertexEQ)(T&, T&),
            string (*vertex2str)(T&)){
        //TODO
        DGraphModel<T>* newGraph = new DGraphModel<T>(vertexEQ, vertex2str);
        for(int idx = 0; idx < nvertices; idx++){
            newGraph->add(vertices[idx]);
        }
        for(int idx = 0; idx < nedges; idx++){
            newGraph->connect(edges[idx].from, edges[idx].to, edges[idx].weight);
        }
        return newGraph;
    }
};

#endif /* DGRAPHMODEL_H */
