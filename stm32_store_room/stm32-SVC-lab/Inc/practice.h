#pragma once
#include <stdint.h>

int32_t Add(int32_t a, int32_t b){
    return a+b;
}

int32_t Sub(int32_t a, int32_t b){
    return a-b;
}

int32_t Mul(int32_t a, int32_t b){
    return a*b;
}

int32_t Div(int32_t a, int32_t b){
    if(b==0){
        return -1;
    }
    return a/b;
}