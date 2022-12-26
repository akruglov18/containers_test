// Copyright 2022 Kruglov Aleksei

#include <tbb/tbb.h>
#include <iostream>
#include <vector>
#include <queue>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>
#include <concurrent_vector.h>
#include <chrono>
#include "containers.h"

void testQueue();
void testVector();
void testHashMap();

int main(int argc, char** argv) {
    /*std::cout << "-----------------------\nTest queue:\n";
    testQueue();
    std::cout << "-----------------------\nTest vector:\n";
    testVector();*/
    std::cout << "-----------------------\nTest hash_map:\n";
    testHashMap();
    return 0;
}

void testQueue() {
    const std::vector<int> sizes = { (int) 1e6, (int)1e7, (int)1e8, (int) 4e8 };
    for (auto size : sizes) {
        std::cout << "size: " << size << "\n";
        {
            MyQueue<int> mq;
            auto start = tbb::tick_count::now();
            for (int i = 0; i < size; i++) {
                mq.push(i);
                mq.pop();
            }
            auto finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " ";
        }
        {
            tbb::concurrent_queue<int> cq;
            auto start = tbb::tick_count::now();
            for (int i = 0; i < size; i++) {
                cq.push(i);
                int c;
                while (!cq.try_pop(c)) {

                }
            }
            auto finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " ";
        }
        {
            std::queue<int> mq;
            auto start = tbb::tick_count::now();
            for (int i = 0; i < size; i++) {
                mq.push(i);
                mq.pop();
            }
            auto finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << "\n";
        }
    }
}

void testVector() {
    const std::vector<int> sizes = { (int)1e6, (int)1e7, (int)1e8, (int)4e8 };
    for (auto size : sizes) {
        std::cout << "size: " << size << "\n";
        {
            MyVector<int> vec;
            auto start = tbb::tick_count::now();
            for (int i = 0; i < size; i++) {
                vec.push_back(i);
            }
            auto finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " ";
            start = tbb::tick_count::now();
            long long sum = 0;
            for (int i = 0; i < size; i++) {
                sum += vec[i];
            }
            finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " " << sum << "\n";
        }
        {
            tbb::concurrent_vector<int> vec;
            auto start = tbb::tick_count::now();
            for (int i = 0; i < size; i++) {
                vec.push_back(i);
            }
            auto finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " ";
            start = tbb::tick_count::now();
            long long sum = 0;
            for (int i = 0; i < size; i++) {
                sum += vec[i];
            }
            finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " " << sum << "\n";
        }
        {
            std::vector<int> vec;
            auto start = tbb::tick_count::now();
            for (int i = 0; i < size; i++) {
                vec.push_back(i);
            }
            auto finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " ";
            start = tbb::tick_count::now();
            long long sum = 0;
            for (int i = 0; i < size; i++) {
                sum += vec[i];
            }
            finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " " << sum << "\n";
        }
    }
}

void testHashMap() {
    const std::vector<int> sizes = { (int)1e6, (int)5e6, (int)1e7, (int)2e7 };
    for (auto size : sizes) {
        std::cout << "size: " << size << "\n";
        {
            MyHashMap<int, int, std::hash<int>> m;
            auto start = tbb::tick_count::now();
            for (int i = 0; i < size; i++) {
                m.insert({ i, i });
            }
            auto finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << "\n";
            start = tbb::tick_count::now();
            long long sum = 0;
            for (int i = 0; i < size; i++) {
                sum += (m.find(i))->second;
            }
            finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " " << sum << "\n";
        }
        {
            tbb::concurrent_hash_map<int, int, MyHashCompare> m;
            auto start = tbb::tick_count::now();
            for (int i = 0; i < size; i++) {
                tbb::concurrent_hash_map<int, int, MyHashCompare>::accessor access;
                m.insert(access, { i, i });
            }
            auto finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << "\n";
            start = tbb::tick_count::now();
            long long sum = 0;
            for (int i = 0; i < size; i++) {
                tbb::concurrent_hash_map<int, int, MyHashCompare>::const_accessor access;
                m.find(access, i);
                sum += access->second;
            }
            finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " " << sum << "\n";
        }
        {
            std::unordered_map<int, int, std::hash<int>> m;
            auto start = tbb::tick_count::now();
            for (int i = 0; i < size; i++) {
                m.insert({ i, i });
            }
            auto finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << "\n";
            start = tbb::tick_count::now();
            long long sum = 0;
            for (int i = 0; i < size; i++) {
                sum += (m.find(i))->second;
            }
            finish = tbb::tick_count::now() - start;
            std::cout << finish.seconds() << " " << sum << "\n";
        }
    }
}
