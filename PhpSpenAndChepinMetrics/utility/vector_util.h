#ifndef VECTOR_UTIL_H
#define VECTOR_UTIL_H

#include <vector>

// cuts vector into two [0;pos) and [pos;vec.size())

template<typename T>
std::pair<std::vector<T>, std::vector<T>> vector_cut(std::vector<T> vec, size_t pos)
{
    std::vector<T> first;
    std::vector<T> second;

    for (size_t it = 0; it < vec.size(); ++it)
    {
        if (it < pos)
        {
            first.push_back(vec[it]);
        }
        else
        {
            second.push_back(vec[it]);
        }
    }

    return {first, second};
}

#endif // VECTOR_UTIL_H
