#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <QString>
class HashTable {
public:
    HashTable(int capacity);
    ~HashTable();

    void insert(const QString& key, int value);
    int  find(const QString& key);
    bool remove(const QString& key);

private:
    struct Node {
        QString key;
        int value;
        Node* next;
    };

    Node** table;
    int capacity;

    int hash(const QString& key);
};

#endif // HASHTABLE_H
