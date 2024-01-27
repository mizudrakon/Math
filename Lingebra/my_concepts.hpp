#pragma once

#include <concepts>

template <typename T>
concept Arithmetic = requires(T a, T b){
    a + b;
    a - b;
    a * b;
    a % b;
    a == b;
    a < b;
};

template <typename T>
concept Iterator = requires(T a, T b){
    //need to be comparable and point to comparable values
    a + 1;
    a - b;
    a == b;
    a < b;
    *a;
    *a < *b;
    *a == *b;
};

template <class C>
concept Container = requires(C lst){
    lst.begin();
    lst.end();
    lst[0];
};