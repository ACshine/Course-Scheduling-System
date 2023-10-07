#include "Course.h"

Course::Course(int id, QString name, double credits, int preCoursesNum, int* preCourses, bool isCompulsory) {
    this->id = id;
    this->name = name;
    this->credits = credits;
    this->preCoursesNum = preCoursesNum;
    this->preCourses = new int[preCoursesNum];
    for (int i = 0; i < preCoursesNum; i++) {
        this->preCourses[i] = preCourses[i];
    }
    this->isCompulsory = isCompulsory;
}

Course::Course() {
    id = 0;
    name = "";
    credits = 0.0;
    preCoursesNum = 0;
    preCourses = nullptr;
    isCompulsory = false;
}

Course::~Course() {
    delete[] preCourses;
}

// 其他方法的实现可以根据需要添加到这里
