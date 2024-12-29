/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
// #include "stacknqueue/PriorityQueue.h"

// TODO
template <class T>
class UGraphModel : public AbstractGraph<T>
{
    using VertexNode = typename AbstractGraph<T>::VertexNode;

private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);
        if (!fromNode)
        {
            stringstream vertexInfo;
            this->vertex2str != nullptr ? vertexInfo << this->vertex2str(from) : vertexInfo << from;
            throw VertexNotFoundException(vertexInfo.str());
        }
        if (!toNode)
        {
            stringstream vertexInfo;
            this->vertex2str != nullptr ? vertexInfo << this->vertex2str(to) : vertexInfo << to;
            throw VertexNotFoundException(vertexInfo.str());
        }

        //If there exists an edge between the two vertices, update the weight
        typename AbstractGraph<T>::Edge* temp = fromNode->getEdge(toNode);
        if(temp){
            toNode->getEdge(fromNode)->getWeight() = weight;
            temp->getWeight() = weight;
            return;
        }

        //Else continue to add the edge
        typename AbstractGraph<T>::Edge *newEdge = new typename AbstractGraph<T>::Edge(fromNode, toNode, weight);
        if (fromNode == toNode)
        {
            fromNode->adList.add(newEdge);
            fromNode->inDegree_++;
            fromNode->outDegree_++;
            return;
        }
        else
        {
            typename AbstractGraph<T>::Edge *newToEdge = new typename AbstractGraph<T>::Edge(toNode, fromNode, weight);
            for (auto edge : fromNode->adList)
            {
                if (edge->getFrom() == fromNode && edge->getTo() == toNode)
                {
                    edge->getWeight() = weight;
                    return;
                }
            }
            // Undirected graph so we increment both the inDegree and outDegree
            fromNode->adList.add(newEdge);
            fromNode->getOutDegree()++;
            fromNode->getInDegree()++;
            toNode->adList.add(newToEdge);
            // toNode->adList.add(newEdge);
            toNode->getInDegree()++;
            toNode->getOutDegree()++;
        }
    }
    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = nullptr;
        typename AbstractGraph<T>::VertexNode *toNode = nullptr;
        for (auto node : this->nodeList)
        {
            if (this->vertexEQ && this->vertexEQ(node->vertex, from))
            {
                fromNode = node;
            }
            else if (node->vertex == from)
            {
                fromNode = node;
            }
            if (this->vertexEQ && this->vertexEQ(node->vertex, to))
            {
                toNode = node;
            }
            else if (node->vertex == to)
            {
                toNode = node;
            }
        }
        if (!fromNode)
        {
            stringstream vertexInfo;
            this->vertex2str != nullptr ? vertexInfo << this->vertex2str(from) : vertexInfo << from;
            throw VertexNotFoundException(vertexInfo.str());
        }
        if (!toNode)
        {
            stringstream vertexInfo;
            this->vertex2str != nullptr ? vertexInfo << this->vertex2str(to) : vertexInfo << to;
            throw VertexNotFoundException(vertexInfo.str());
        }
        typename AbstractGraph<T>::Edge *removeEdge = fromNode->getEdge(toNode);
        if (!removeEdge){
            typename AbstractGraph<T>::Edge throwEdge(fromNode, toNode, 0);
            throw EdgeNotFoundException(this->edge2Str(throwEdge));
        }
        if (fromNode == toNode)
        {
            fromNode->adList.removeItem(removeEdge);
            fromNode->inDegree_--;
            fromNode->outDegree_--;
            return;
        }
        else
        {
            fromNode->adList.removeItem(removeEdge);
            fromNode->outDegree_--;
            fromNode->inDegree_--;
            removeEdge = toNode->getEdge(fromNode);
            toNode->adList.removeItem(removeEdge);
            toNode->inDegree_--;
            toNode->outDegree_--;
            return;
        }
    }
    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *vertexNode = this->getVertexNode(vertex);
        if(!vertexNode){
            stringstream vertexInfo;
            this->vertex2str != nullptr ? vertexInfo << this->vertex2str(vertex) : vertexInfo << vertex;
            throw VertexNotFoundException(vertexInfo.str());
        }
        DLinkedList<T> connectedVertices = this->getOutwardEdges(vertex);
        for (auto connectedVertex : connectedVertices)
        {
            disconnect(vertex, connectedVertex);
        }
        this->nodeList.removeItem(vertexNode);
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T> *model = new UGraphModel<T>(vertexEQ, vertex2str);
        for (int idx = 0; idx < nvertices; idx++)
        {
            model->add(vertices[idx]);
        }
        for (int idx = 0; idx < nedges; idx++)
        {
            // VertexNode* fromNode = new VertexNode(edges[idx].from, vertexEQ, vertex2str);
            // VertexNode* toNode = new VertexNode(edges[idx].to, vertexEQ, vertex2str);
            model->connect(edges[idx].from, edges[idx].to, edges[idx].weight);
        }
        return model;
    }
};

#endif /* UGRAPHMODEL_H */
