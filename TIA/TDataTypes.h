#pragma once
#include <iostream>

namespace TDataTypes
{


    /*
    * TPixelCoordinate
    *This class is intended to hold pixel coordinates for CV operations.
    * Uses matrix conventions of i,j to denote row and column of pixels.
    *   j ------------------------------>
    * i
    * |
    * |
    * |
    * |
    * V


    */
    class TPixelCoordinate {
    public:
        TPixelCoordinate();
        TPixelCoordinate(int i, int j);
        TPixelCoordinate(std::pair<int, int> p);

        TPixelCoordinate operator+ (const TPixelCoordinate &other) {
            TPixelCoordinate pc;
            pc.i = this->i + other.i;
            pc.j = this->j + other.j;
            return pc;
        }

    private:
        int i;
        int j;

    };

    using TPC = TPixelCoordinate;

    // Denotes what type of storage a TContainerItem contains.
    enum gridType {
        ARRAY = 0, // A contiguous block/grid of item slots.
        DYNAMIC = 1 // Could have disconnected slots of varying sizes and positions.
    };

  
}
