#ifndef GRAPHLMANAGER_H
#define GRAPHLMANAGER_H
#include "Graphl.h"
class GraphlManager {
public:
    static Graphl& getInstance();
private:
    GraphlManager() = default;
    static Graphl instance;
};
#endif // GRAPHLMANAGER_H
