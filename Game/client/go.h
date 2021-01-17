/**
 * @file go.h
 * @brief 
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include <iostream>

typedef struct SmartInput{
    bool J;
    bool L1;
    bool R1;
    bool R2;
    bool R3;
    bool R4;
    bool R5;
    bool R6;
    bool Q;
} SmartInput;

extern SmartInput GoSInput;
