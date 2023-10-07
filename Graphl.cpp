#include"Graphl.h"
#include <QDebug>
template <class Elem>
LinkNode<Elem>::LinkNode(Elem element,  QSharedPointer<LinkNode<Elem>> next)
    : element(element), next(next)
{

}
template <class Elem>
LinkNode<Elem>::LinkNode()
{

}

template <class Elem>
LinkList<Elem>::LinkList() {
    head = QSharedPointer<LinkNode<Elem>>(new LinkNode<Elem>); // 分配空间！
}


Edge::Edge()
{
    from = -1;  to = -1; weight = 0;
}

Edge::Edge(int f, int t, int w)
{
    from = f;  to = t; weight = w;
}

Graph::Graph(int numVert)
{
    numVertex = numVert;//初始化顶点
    numEdge = 0;
    Indegree = new int[numVertex];
    Mark = new int[numVertex];
    for (int i = 0; i < numVertex; i++) {
        Mark[i] = 0;
        Indegree[i] = 0;
    }
}
Graph::~Graph() {
    delete[] Mark;
    delete[] Indegree;
}

Graphl::Graphl(int numVert) :Graph(numVert)
{
    head = new LinkList<listUnit>[numVert];
}
Edge Graphl::FirstEdge(int oneVertex) {
    Edge myEdge;
    myEdge.from = oneVertex;
    QSharedPointer<LinkNode<listUnit>>temp = head[oneVertex].head;
    if (temp->next != NULL) {
        myEdge.to = temp->next->element.vertex;
        myEdge.weight = temp->next->element.weight;
    }
    return myEdge;
}
bool Graphl::IsEdge(Edge oneEdge) {
    if (oneEdge.to == -1) return false;
    return true;
}
Edge Graphl::NextEdge(Edge preEdge) {
    Edge myEdge;
    myEdge.from = preEdge.from;
    QSharedPointer<LinkNode<listUnit>>temp = head[preEdge.from].head;
    while (temp->next != NULL && temp->next->element.vertex <= preEdge.to)
        temp = temp->next;
    if (temp->next != NULL) {
        myEdge.to = temp->next->element.vertex;
        myEdge.weight = temp->next->element.weight;
    }
    return myEdge;
}
void Graphl::setEdge(int from, int to, int weight) {
    QSharedPointer<LinkNode<listUnit>>temp= head[from].head;
    while (temp->next != NULL && temp->next->element.vertex < to)
        temp = temp->next;
    if (temp->next == NULL) {
        //边不存在且最后一条边
        temp->next = QSharedPointer<LinkNode<listUnit>>(new LinkNode<listUnit>);//新建一个边的链表节点
        temp->next->element.vertex = to;//初始化
        temp->next->element.weight = weight;
        numEdge++;   Indegree[to]++;
        return;
    }
    if (temp->next->element.vertex == to) {
        // 边存在(更新权重)
        temp->next->element.weight = weight;
        return;
    }
    if (temp->next->element.vertex > to) {
        // 边不存在，但后边还有边
        QSharedPointer<LinkNode<listUnit>>tmp(new LinkNode<listUnit>);
        tmp->next = temp->next;
        temp->next = tmp;
        tmp->element.vertex = to;
        tmp->element.weight = weight;
        numEdge++;
        Indegree[to]++;
        return;
    }
}
void Graphl::delEdge(int from, int to) {
    QSharedPointer<LinkNode<listUnit>>temp= head[from].head;
    while (temp->next != NULL
           && temp->next->element.vertex < to)
        temp = temp->next;
    if (temp->next == NULL) return;
    if (temp->next->element.vertex > to) return;
    if (temp->next->element.vertex == to) {
        QSharedPointer<LinkNode<listUnit>>other= temp->next->next;
        temp->next = other;
        Indegree[to]--;
    }
}
