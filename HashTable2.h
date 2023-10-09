#ifndef HASHTABLE2_H
#define HASHTABLE2_H

#include <QString>

class HashTable2 {
public:
    HashTable2(int size);
    ~HashTable2();

    void insert(int key, const QString& value);
    bool remove(int key);
    bool contains(int key);
    QString find(int key);

private:
    struct Node {
        int key;
        QString value;
        Node* next;
    };

    Node** table;
    int tableSize;

    int hash(int key);
};

#endif // HASHTABLE2_H
