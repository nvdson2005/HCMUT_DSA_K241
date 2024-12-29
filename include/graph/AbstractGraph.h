/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   AbstractGraph.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:48
 */

#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H

#include <sstream>
#include <string>

#include "graph/IGraph.h"
using namespace std;
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
class AbstractGraph : public IGraph<T>
{
public:
    class Edge;       // forward declaration
    class VertexNode; // forward declaration
    class Iterator;   // forward declaration

private:
protected:
    // Using the adjacent list technique, so need to store list of nodes (nodeList)
    DLinkedList<VertexNode *> nodeList;

    // Function pointers:
    bool (*vertexEQ)(T &, T &); // to compare two vertices
    string (*vertex2str)(T &);  // to obtain string representation of vertices

    VertexNode *getVertexNode(T &vertex)
    {
        typename DLinkedList<VertexNode *>::Iterator it = nodeList.begin();
        while (it != nodeList.end())
        {
            VertexNode *node = *it;
            if (vertexEQ(node->vertex, vertex))
                return node;
            it++;
        }
        return 0;
    }
    string vertex2Str(VertexNode &node)
    {
        return vertex2str(node.vertex);
    }
    string edge2Str(Edge &edge)
    {
        stringstream os;
        os << "E("
           << vertex2str(edge.from->vertex)
           << ","
           << vertex2str(edge.to->vertex)
           << ")";
        return os.str();
    }

public:
    AbstractGraph(
        bool (*vertexEQ)(T &, T &) = 0,
        string (*vertex2str)(T &) = 0)
    {

        this->vertexEQ = vertexEQ;
        this->vertex2str = vertex2str;
    }
    virtual ~AbstractGraph() {}

    typedef bool (*vertexEQFunc)(T &, T &);
    typedef string (*vertex2strFunc)(T &);
    vertexEQFunc getVertexEQ()
    {
        return this->vertexEQ;
    }
    vertex2strFunc getVertex2Str()
    {
        return this->vertex2str;
    }

    //////////////////////////////////////////////////////////////////////
    //////////////////// IMPLEMENTATION of IGraph API ////////////////////
    //////////////////////////////////////////////////////////////////////
    /* connect, disconnect, and remove: will be overridden in:
     *  >> UGraphModel
     *  >> DGraphModel
     */
    virtual void connect(T from, T to, float weight = 0) = 0;
    virtual void disconnect(T from, T to) = 0;
    virtual void remove(T vertex) = 0;

    /* The following are common methods for UGraphModel and DGraphModel
     */
    virtual void add(T vertex)
    {
        // TODO
        VertexNode* temp = this->getVertexNode(vertex);
        if(temp != 0) return;
        VertexNode *newVertex = new VertexNode(vertex, this->vertexEQ, this->vertex2str);
        nodeList.add(newVertex);
    }
    virtual bool contains(T vertex)
    {
        // TODO
        for (auto node : nodeList)
        {
            if (vertexEQ && vertexEQ(node->vertex, vertex))
                return true;
            else if (node->vertex == vertex)
                return true;
        }
        return false;
    }
    virtual float weight(T from, T to)
    {
        // TODO
        bool fromVertexFound = false;
        VertexNode *fromNode = nullptr;
        bool toVertexFound = false;
        VertexNode *toNode = nullptr;
        for (auto node : nodeList)
        {
            if (vertexEQ && vertexEQ(node->vertex, from))
            {
                fromVertexFound = true;
                fromNode = node;
                break;
            }
            else if (node->vertex == from)
            {
                fromVertexFound = true;
                fromNode = node;
                break;
            }
        }
        for (auto node : nodeList)
        {
            if (vertexEQ && vertexEQ(node->vertex, to))
            {
                toVertexFound = true;
                toNode = node;
                break;
            }
            else if (node->vertex == to)
            {
                toVertexFound = true;
                toNode = node;
                break;
            }
        }
        if (!fromVertexFound || !toVertexFound)
        {
            stringstream vertexInfo;
            if (!fromVertexFound)
            {
                vertex2str != nullptr ? vertexInfo << vertex2str(from) : vertexInfo << from;
                throw VertexNotFoundException(vertexInfo.str());
            }
            if (!toVertexFound)
            {
                vertex2str != nullptr ? vertexInfo << vertex2str(to) : vertexInfo << to;
                throw VertexNotFoundException(vertexInfo.str());
            }
            throw VertexNotFoundException("");
        }
        if (fromVertexFound && toVertexFound)
        {
            Edge *connectEdge = nullptr;
            if (fromNode && toNode)
                connectEdge = fromNode->getEdge(toNode);
            if (connectEdge != nullptr)
                return connectEdge->weight;
            else
            {
                Edge returnEdge(fromNode, toNode, 0);
                throw EdgeNotFoundException(edge2Str(returnEdge));
            }
        }
        Edge returnEdge(fromNode, toNode, 0);
        throw EdgeNotFoundException(edge2Str(returnEdge));
    }
    virtual DLinkedList<T> getOutwardEdges(T from)
    {
        // TODO
        DLinkedList<T> returnList;
        VertexNode *fromNode = nullptr;
        for (auto node : nodeList)
        {
            if (vertexEQ && vertexEQ(node->vertex, from))
            {
                fromNode = node;
                break;
            }
            else if (node->vertex == from)
            {
                fromNode = node;
                break;
            }
        }
        if (fromNode == nullptr)
        {
            stringstream vertexInfo;
            vertex2str != nullptr ? vertexInfo << vertex2str(from) : vertexInfo << from;
            throw VertexNotFoundException(vertexInfo.str());
        }
        for (auto edge : fromNode->adList)
        {
            if (edge->from == fromNode)
            {
                returnList.add(edge->to->vertex);
            }
        }
        return returnList;
    }

    virtual DLinkedList<T> getInwardEdges(T to)
    {
        // TODO
        DLinkedList<T> returnList;
        VertexNode *toNode = nullptr;
        for (auto node : nodeList)
        {
            if (vertexEQ && vertexEQ(node->vertex, to))
            {
                toNode = node;
                break;
            }
            else if (node->vertex == to)
            {
                toNode = node;
                break;
            }
        }
        if (toNode == nullptr)
        {
            stringstream vertexInfo;
            vertex2str != nullptr ? vertexInfo << vertex2str(to) : vertexInfo << to;
            throw VertexNotFoundException(vertexInfo.str());
        }
        for (auto node : nodeList)
        {
            for (auto edge : node->adList)
            {
                if (edge->to == toNode)
                {
                    returnList.add(edge->from->vertex);
                }
            }
        }
        return returnList;
    }

    virtual int size()
    {
        // TODO
        return nodeList.size();
    }
    virtual bool empty()
    {
        // TODO
        return nodeList.empty();
    }
    virtual void clear()
    {
        // TODO
        for(auto it = nodeList.begin(); it != nodeList.end(); it++){
            (*it)->adList.clear();
        }
        nodeList.clear();
    }
    virtual int inDegree(T vertex)
    {
        // TODO
        VertexNode *vertexNode = nullptr;
        for (auto node : nodeList)
        {
            if (vertexEQ && vertexEQ(node->vertex, vertex))
            {
                vertexNode = node;
                break;
            }
            else if (node->vertex == vertex)
            {
                vertexNode = node;
                break;
            }
        }
        if (vertexNode == nullptr)
        {
            stringstream vertexInfo;
            vertex2str != nullptr ? vertexInfo << vertex2str(vertex) : vertexInfo << vertex;
            throw VertexNotFoundException(vertexInfo.str());
        }
        return vertexNode->inDegree();
    }
    virtual int outDegree(T vertex)
    {
        // TODO
        VertexNode *vertexNode = nullptr;
        for (auto node : nodeList)
        {
            if (vertexEQ && vertexEQ(node->vertex, vertex))
            {
                vertexNode = node;
                break;
            }
            else if (node->vertex == vertex)
            {
                vertexNode = node;
                break;
            }
        }
        if (vertexNode == nullptr)
        {
            stringstream vertexInfo;
            vertex2str != nullptr ? vertexInfo << vertex2str(vertex) : vertexInfo << vertex;
            throw VertexNotFoundException(vertexInfo.str());
        }
        return vertexNode->outDegree();
    }

    virtual DLinkedList<T> vertices()
    {
        // TODO
        DLinkedList<T> returnList;
        for(auto v : nodeList){
            returnList.add(v->vertex);
        }
        return returnList;
    }
    virtual bool connected(T from, T to)
    {
        // TODO
        VertexNode* fromNode = nullptr;
        VertexNode* toNode = nullptr;
        for(auto node : nodeList){
            if(vertexEQ && vertexEQ(node->vertex, from)){
                fromNode = node;
            }else if(node->vertex == from){
                fromNode = node;
            }
            if(vertexEQ && vertexEQ(node->vertex, to)){
                toNode = node;
            }else if(node->vertex == to){
                toNode = node;
            }
        }
        if(!fromNode){
            stringstream vertexInfo;
            vertex2str != nullptr ? vertexInfo << vertex2str(from) : vertexInfo << from;
            throw VertexNotFoundException(vertexInfo.str());
        }
        if(!toNode){
            stringstream vertexInfo;
            vertex2str != nullptr ? vertexInfo << vertex2str(to) : vertexInfo << to;
            throw VertexNotFoundException(vertexInfo.str());
        }
        if(fromNode->getEdge(toNode) != nullptr){
            return true;
        }
        return false;
    }
    void println()
    {
        cout << this->toString() << endl;
    }
    virtual string toString()
    {
        string mark(50, '=');
        stringstream os;
        os << mark << endl;
        os << left << setw(12) << "Vertices:" << endl;
        typename DLinkedList<VertexNode *>::Iterator nodeIt = nodeList.begin();
        while (nodeIt != nodeList.end())
        {
            VertexNode *node = *nodeIt;
            os << node->toString() << endl;
            nodeIt++;
        }
        string mark2(30, '-');
        os << mark2 << endl;

        os << left << setw(12) << "Edges:" << endl;

        nodeIt = nodeList.begin();
        while (nodeIt != nodeList.end())
        {
            VertexNode *node = *nodeIt;

            typename DLinkedList<Edge *>::Iterator edgeIt = node->adList.begin();
            while (edgeIt != node->adList.end())
            {
                Edge *edge = *edgeIt;
                os << edge->toString() << endl;

                edgeIt++;
            }

            nodeIt++;
        }
        os << mark << endl;
        return os.str();
    }

    /* begin, end: will be used to traverse on graph's vertices
     * example: assume that "graph" is a graph.
     * Code:
     *      AbstractGraph<T>::Iterator it;
     *      for(it = graph.begin(); it != graph.end(); it++){
     *          T vertex = *it;
     *          //continue to process vertex here!
     *      }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////

public:
    // BEGIN of VertexNode
    class VertexNode
    {
    private:
        template <class U>
        friend class UGraphModel; // UPDATED: added
        T vertex;
        int inDegree_, outDegree_;
        DLinkedList<Edge *> adList;
        friend class Edge;
        friend class AbstractGraph;

        bool (*vertexEQ)(T &, T &);
        string (*vertex2str)(T &);

    public:
        VertexNode() : adList(&DLinkedList<Edge *>::free, &Edge::edgeEQ) {}
        VertexNode(T vertex, bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
            : adList(&DLinkedList<Edge *>::free, &Edge::edgeEQ)
        {
            this->vertex = vertex;
            this->vertexEQ = vertexEQ;
            this->vertex2str = vertex2str;
            this->outDegree_ = this->inDegree_ = 0;
        }
        int &getInDegree(){
            return this->inDegree_;
        }
        int &getOutDegree(){
            return this->outDegree_;
        }
        T &getVertex()
        {
            return vertex;
        }
        DLinkedList<Edge*> &getAdList(){
            return this->adList;
        }
        void connect(VertexNode *to, float weight = 0)
        {
            // TODO
            for(auto edge : this->adList){
                if(edge->from == this && edge->to == to || vertexEQ && vertexEQ(this->vertex, edge->from->vertex) && vertexEQ(to->vertex, edge->to->vertex)){
                    edge->weight = weight;
                    return;
                }
            }
            Edge* newEdge = new Edge(this, to, weight);
            adList.add(newEdge);
            outDegree_++;
            to->inDegree_++;
        }
        DLinkedList<T> getOutwardEdges()
        {
            // TODO
            DLinkedList<T> returnList;
            for(auto edge : adList){
                if(edge->from == this){
                    returnList.add(edge->to->vertex);
                }
            }
            return returnList;
        }

        Edge *getEdge(VertexNode *to)
        {
            // TODO
            Edge* returnEdge = nullptr;
            for(auto edge : adList){
                if(edge->from == this && edge->to == to){
                    returnEdge = edge;
                    break;
                }
            }
            return returnEdge;
        }
        bool equals(VertexNode *node)
        {
            // TODO
            if(vertexEQ)
                return vertexEQ(this->vertex, node->vertex);
            else
                return this->vertex == node->vertex;
        }

        void removeTo(VertexNode *to)
        {
            // TODO
            for(auto edge : adList){
                if(edge->from == this && edge->to == to || vertexEQ && vertexEQ(this->vertex, edge->from->vertex) && vertexEQ(to->vertex, edge->to->vertex)){
                    adList.removeItem(edge);
                    this->outDegree_--;
                    to->inDegree_--;
                    return;
                }
            }
        }
        int inDegree()
        {
            // TODO
            return this->inDegree_;
        }
        int outDegree()
        {
            // TODO
            return this->outDegree_;
        }
        string toString()
        {
            stringstream os;
            os << "V("
               << this->vertex << ", "
               << "in: " << this->inDegree_ << ", "
               << "out: " << this->outDegree_ << ")";
            return os.str();
        }
    };
    // END of VertexNode

    // BEGIN of Edge
    class Edge
    {
    private:
        VertexNode *from;
        VertexNode *to;
        float weight;
        friend class VertexNode;
        friend class AbstractGraph;

    public:
        Edge() {}
        Edge(VertexNode *from, VertexNode *to, float weight = 0)
        {
            this->from = from;
            this->to = to;
            this->weight = weight;
        }
        VertexNode *getFrom()
        {
            return this->from;
        }
        VertexNode *getTo()
        {
            return this->to;
        }
        float & getWeight(){
            return this->weight;
        }
        bool equals(Edge *edge)
        {
            // TODO
            if(this->from == edge->from && this->to == edge->to){
                return true;
            }
            return false;
        }

        static bool edgeEQ(Edge *&edge1, Edge *&edge2)
        {
            return edge1->equals(edge2);
        }
        string toString()
        {
            stringstream os;
            os << "E("
               << this->from->vertex
               << ","
               << this->to->vertex
               << ","
               << this->weight
               << ")";
            return os.str();
        }
    };
    // END of Edge

    // BEGIN of Iterator
public:
    class Iterator
    {
    private:
        typename DLinkedList<VertexNode *>::Iterator nodeIt;

    public:
        Iterator(AbstractGraph<T> *pGraph = 0, bool begin = true)
        {
            if (begin)
            {
                if (pGraph != 0)
                    nodeIt = pGraph->nodeList.begin();
            }
            else
            {
                if (pGraph != 0)
                    nodeIt = pGraph->nodeList.end();
            }
        }
        Iterator &operator=(const Iterator &iterator)
        {
            this->nodeIt = iterator.nodeIt;
            return *this;
        }

        T &operator*()
        {
            return (*nodeIt)->vertex;
        }

        bool operator!=(const Iterator &iterator)
        {
            return nodeIt != iterator.nodeIt;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            nodeIt++;
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
    // END of Iterator
};

#endif /* ABSTRACTGRAPH_H */
