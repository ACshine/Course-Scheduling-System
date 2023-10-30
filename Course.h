#ifndef COURSE_H
#define COURSE_H
#include <QSharedPointer>
#include<QString>
class Course {
public:
    int id;
    QString name;
    double credits;
    int preCoursesNum;
    int* preCourses;
    bool isCompulsory;//是否为必修课程
    Course(int id,QString name, double credits, int preCoursesNum,int* preCourses,bool isCompulsory);
    Course();
    ~Course();
};
#endif // COURSE_H
