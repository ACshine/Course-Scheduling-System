#include"HashTable.h"

HashTable::HashTable(int capacity) {
    this->capacity = capacity;
    table = new Node*[capacity]();
}

HashTable::~HashTable() {
    for (int i = 0; i < capacity; i++) {
        Node* current = table[i];
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] table;
}

int HashTable::hash(const QString& key) {
    int hash = 0;
    for (QChar c : key) {
        hash = (hash * 31 + c.unicode()) % capacity;
    }
    return hash;
}

void HashTable::insert(const QString& key, int value) {
    int index = hash(key);
    Node* newNode = new Node{key, value, nullptr};

    if (!table[index]) {
        table[index] = newNode;
    } else {
        // 处理冲突，使用链表
        Node* current = table[index];
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

int HashTable::find(const QString& key) {
    int index = hash(key);
    Node* current = table[index];
    int value;
    while (current) {
        if (current->key == key) {
            value = current->value;
            return value;
        }
        current = current->next;
    }
    return -1;
}

bool HashTable::remove(const QString& key) {
    int index = hash(key);
    Node* current = table[index];
    Node* prev = nullptr;

    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}
