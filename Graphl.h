#ifndef GRAPHL_H
#define GRAPHL_H
#include <QSharedPointer>

template <class Elem>
class LinkNode {
public:
    Elem element;
    QSharedPointer<LinkNode<Elem>> next;  // 使用 QSharedPointer 管理下一个节点
    LinkNode(Elem element, QSharedPointer<LinkNode<Elem>> next = nullptr);
    LinkNode();
};


template <class Elem>
class LinkList {
public:
    QSharedPointer<LinkNode<Elem>> head;  // 使用 QSharedPointer 管理链表头节点
    LinkList();
};

struct listUnit {//边结点的结构体定义
    int vertex;
    int weight;
};

class Edge {
public:
    int from, to, weight;
    Edge();
    Edge(int f, int t, int w);
};
class Graph {                              //图的ADT
public:
    int numVertex;   //顶点个数
    int numEdge;     //边条数
    int* Mark;          //访问标记
    int* Indegree;    //入度
    Graph(int numVert);
    ~Graph();
    bool IsEdge(Edge oneEdge);
    Edge FirstEdge(int oneVertex);   //返回与关联的第一条边
    Edge NextEdge(Edge preEdge);  //返回有相同关联顶点的下一条边
    bool setEdge(int fromVertex, int toVertex, int weight); //添加一条边
    bool delEdge(int fromVertex, int toVertex); //删一条边
};
class  Graphl : public Graph {//邻接表
private:
    LinkList<listUnit>* head;
public:
    Graphl();
    Graphl(int numVert);
    Edge FirstEdge(int oneVertex);
    Edge NextEdge(Edge preEdge);
    void setEdge(int from, int to, int weight);
    void delEdge(int from, int to);
    bool IsEdge(Edge oneEdge);
};
#endif // GRAPHL_H
