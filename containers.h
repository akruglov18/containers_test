// Copyright 2022 Kruglov Aleksei

#pragma once
#include <mutex>
#include <queue>
#include <vector>
#include <unordered_map>

template<typename T>
class MyQueue : private std::queue<T> {
private:
    std::mutex access;
public:
    void push(const T& x) {
        std::lock_guard<std::mutex> l(access);
        std::queue<T>::push(x);
    }
    void pop() {
        std::lock_guard<std::mutex> l(access);
        std::queue<T>::pop();
    }
};

template<typename T>
class MyVector : private std::vector<T> {
private:
    std::mutex access;
public:
    void push_back(const T& x) {
        std::lock_guard<std::mutex> l(access);
        std::vector<T>::push_back(x);
    }
    T& operator[](int i) {
        std::lock_guard<std::mutex> l(access);
        return std::vector<T>::operator[](i);
    }
    void clear() {
        std::vector<T>::clear();
    }
    void shrink_to_fit() {
        std::vector<T>::shrink_to_fit();
    }
};

struct MyHashCompare {
    static size_t hash(int x) {
        return std::hash<int>::_Do_hash(x);
    }
    static bool equal(int x, int y) {
        return x == y;
    }
};

template<typename K, typename T, typename H>
class MyHashMap : private std::unordered_map<K, T, H> {
private:
    std::mutex access;
public:
    void insert(const std::pair<K, T>& x) {
        std::lock_guard<std::mutex> l(access);
        std::unordered_map<K, T>::insert(x);
    }
    std::unordered_map<K, T, H>::iterator find(const K& x) {
        std::lock_guard<std::mutex> l(access);
        return std::unordered_map<K, T>::find(x);
    }
};
