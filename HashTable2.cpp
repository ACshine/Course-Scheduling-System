#include "HashTable2.h"

HashTable2::HashTable2(int size) {
    tableSize = size;
    table = new Node*[tableSize];
    for (int i = 0; i < tableSize; ++i) {
        table[i] = nullptr;
    }
}

HashTable2::~HashTable2() {
    for (int i = 0; i < tableSize; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] table;
}

int HashTable2::hash(int key) {
    // 简单的散列函数示例，实际中可能需要更复杂的函数
    return key % tableSize;
}

void HashTable2::insert(int key, const QString& value) {
    int index = hash(key);
    Node* newNode = new Node{key, value, nullptr};
    if (table[index] == nullptr) {
        table[index] = newNode;
    } else {
        Node* current = table[index];
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

bool HashTable2::remove(int key) {
    int index = hash(key);
    Node* current = table[index];
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}

bool HashTable2::contains(int key) {
    int index = hash(key);
    Node* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }

    return false;
}

QString HashTable2::find(int key) {
    int index = hash(key);
    Node* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    return QString(); // 返回空QString表示未找到
}
