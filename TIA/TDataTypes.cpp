#pragma once

#include "TDataTypes.h"
#include <iostream>


namespace TDataTypes
{


    TPixelCoordinate::TPixelCoordinate()
    {
        // Default constructor
        i = 0;
        j = 0;
    };

    TPixelCoordinate::TPixelCoordinate(int i, int j) 
    {
        this->i = i;
        this->j = j;
    };
    TPixelCoordinate::TPixelCoordinate(std::pair<int, int> p) 
    {
        this->i = p.first;
        this->j = p.second;
    };



};
