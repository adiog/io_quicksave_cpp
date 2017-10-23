// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once


namespace qs {
namespace util {

class Integer
{
public:
    static int safe_bounds(int input, int min, int max)
    {
        if (input < min)
        {
            return min;
        }
        else if (input > max)
        {
            return max;
        }
        else
        {
            return input;
        }
    }
};
}
}
