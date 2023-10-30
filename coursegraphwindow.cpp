// coursegraphwindow.cpp
#include "coursegraphwindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include"Course.h"
#include"Graphl.h"
#include"HashTable.h"
#include"HashTable2.h"
#define MAX_COURSES 300
extern Course courses[MAX_COURSES];
extern int courseNum;
extern int Depth[MAX_COURSES];
extern HashTable h1;
extern HashTable2 h2;
extern bool valid[MAX_COURSES];
bool hasdepth[MAX_COURSES];
extern int needthis[MAX_COURSES][MAX_COURSES];
extern int needcnt[MAX_COURSES];
extern Graphl g;
extern bool Compulsory[MAX_COURSES];
CourseGraphWindow::CourseGraphWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // 设置窗口标题
    setWindowTitle("课程拓扑图");
    QIcon newIcon(":/icons/pic.png");
    setWindowIcon(newIcon);

    // 创建图形视图和场景
    view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);

    // 设置视图和场景
    view->setScene(scene);
    setCentralWidget(view);
    setStyleSheet("background-color: lightblue;"); // 使用您想要的颜色
}

CourseGraphWindow::~CourseGraphWindow()
{

}
//当一个点有多条入边时，深度要取最大的那个
void dfs(int v, int Depth[], int currentDepth)
{
    Depth[v] = currentDepth; // 记录节点深度
    for (Edge e = g.FirstEdge(v); g.IsEdge(e); e = g.NextEdge(e)) {
        int j = e.to;
        if (Depth[j] < currentDepth + 1) { // 只有当前深度更大时才更新
            dfs(j, Depth, currentDepth + 1);
        }
    }
}
void CalculateDepth()
{
    memset(hasdepth,false,sizeof hasdepth);
    memset(Depth,0,sizeof Depth);//不初始化则之后dfs不会更新了(因为depth在第一遍执行时已经有值了)
    for(int i=1;i<=courseNum;i++)
    {
        int id=courses[i].id;
        if(valid[id]&&courses[i].preCoursesNum==0)
        {
            hasdepth[id]=true;
            dfs(id, Depth,1);
        }
    }
}
void DrawLineWithArrow(QGraphicsScene* scene, QPen pen, QPointF start, QPointF end) // 创建带箭头的有向边
{
    QLineF line(end, start);
    int arrowsize=10;
    double angle = std::atan2(-line.dy(), line.dx());
    QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowsize,
                                          cos(angle + M_PI / 3) * arrowsize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowsize,
                                          cos(angle + M_PI - M_PI / 3) * arrowsize);

    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;

    scene->addLine(line, pen);
    scene->addPolygon(arrowHead, pen, QBrush(pen.color()));
}
void CourseGraphWindow::drawCourseGraph()
{
    double x[MAX_COURSES];
    // 清空场景
    scene->clear();
    CalculateDepth();
    for(int i=1;i<=courseNum;i++)
    {
        int id=courses[i].id;
        if(!valid[id]) continue;
        //qDebug()<<h2.find(id)<<" "<<Depth[id];
    }
    QGraphicsEllipseItem *hn[MAX_COURSES];//维护id到点坐标之间的映射关系
    int horizontalSpacing = 160; // 两个节点之间的水平间距
    int verticalSpacing = 150;    // 不同层次之间的垂直间距
    bool hasDraw[MAX_COURSES];
    memset(hasDraw,false,sizeof hasDraw);
    qreal nodeRadius = 40; // 节点的半径
    int nowcnt[MAX_COURSES];
    memset(nowcnt,0,sizeof nowcnt);
    for(int i=1;i<=courseNum;i++)
    {
        auto&c=courses[i];
        if(!valid[c.id]) continue;

        int d=Depth[c.id];

        QGraphicsEllipseItem *node=NULL;

        if(!hasDraw[c.id])
        {
            qDebug()<<d<<" "<<c.name;
            node=new QGraphicsEllipseItem(horizontalSpacing*nowcnt[d], verticalSpacing*d, 2 * nodeRadius, 2 * nodeRadius);
            hn[c.id]=node;
            x[c.id]=nowcnt[d];
            nowcnt[d]++;
            hasDraw[c.id]=true;
            // 设置节点样式
            if(!c.isCompulsory)
            {
                QPen nodePen(QColor(123,104,238));
                QBrush nodeBrush(QColor(123,104,238)); // 设置节点的填充颜色
                node->setPen(nodePen);
                node->setBrush(nodeBrush);
            }
            else
            {
                QPen nodePen(QColor(219,112,147));
                QBrush nodeBrush(QColor(219,112,147)); // 设置节点的填充颜色
                node->setPen(nodePen);
                node->setBrush(nodeBrush);
            }

            QGraphicsTextItem *textItem1 = new QGraphicsTextItem(node);
            QFont font = textItem1->font();

            // 设置字体加粗
            font.setBold(true);

            // 将新字体应用于文本项
            textItem1->setFont(font);
            textItem1->setDefaultTextColor(Qt::black);
            textItem1->setHtml(QString("<div align='center'>%1</div>").arg(c.name));
            textItem1->setTextWidth(node->boundingRect().width()); // 设置文本宽度，以自动换行

            // 将文本位置设置为节点中心
            textItem1->setPos(node->boundingRect().center() - QPointF(textItem1->boundingRect().width() / 2, textItem1->boundingRect().height() / 2));
            scene->addItem(node);
        }
        for(int j=1;j<=c.preCoursesNum;j++)
        {
            int preid=c.preCourses[j];
            if(!valid[preid]) continue;

            if(!hasDraw[preid])
            {
                int d=Depth[preid];

                QGraphicsEllipseItem *nodefa=new QGraphicsEllipseItem(horizontalSpacing*nowcnt[d], verticalSpacing*d, 2 * nodeRadius, 2 * nodeRadius);
                hn[preid]=nodefa;
                x[preid]=nowcnt[d];
                nowcnt[d]++;
                if(!Compulsory[preid])
                {
                    QPen nodePen(QColor(123,104,238));
                    QBrush nodeBrush(QColor(123,104,238)); // 设置节点的填充颜色
                    nodefa->setPen(nodePen);
                    nodefa->setBrush(nodeBrush);
                }
                else
                {
                    QPen nodePen(QColor(219,112,147));
                    QBrush nodeBrush(QColor(219,112,147)); // 设置节点的填充颜色
                    nodefa->setPen(nodePen);
                    nodefa->setBrush(nodeBrush);
                }
                QGraphicsTextItem *textItem1 = new QGraphicsTextItem(nodefa);
                QFont font = textItem1->font();

                // 设置字体加粗
                font.setBold(true);

                // 将新字体应用于文本项
                textItem1->setFont(font);
                textItem1->setHtml(QString("<div align='center'>%1</div>").arg(h2.find(preid)));//id和courses数组不对应！
                textItem1->setTextWidth(nodefa->boundingRect().width()); // 设置文本宽度，以自动换行

                // 将文本位置设置为节点中心
                textItem1->setPos(nodefa->boundingRect().center() - QPointF(textItem1->boundingRect().width() / 2, textItem1->boundingRect().height() / 2));
                textItem1->setDefaultTextColor(Qt::black);
                scene->addItem(nodefa);
                hasDraw[preid]=true;

            }
        }
    }
    //生成所有点后连边 这样即使当前课程的先修课程还没被输入，最终图的拓扑结构也是正确的
    for(int i=1;i<=courseNum;i++)
    {
        int id=courses[i].id;
        if(!valid[id]) continue;
        QGraphicsEllipseItem *nodefa=hn[id];
        for(int j=0;j<needcnt[id];j++)
        {
            int bid=needthis[id][j];
            QGraphicsEllipseItem *node=hn[bid];
            QPen edgePen(Qt::black);
            DrawLineWithArrow(scene, edgePen, nodefa->boundingRect().center()+QPointF(0,nodeRadius) , node->boundingRect().center()-QPointF(0,nodeRadius));
        }
    }

}


