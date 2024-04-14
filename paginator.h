#pragma once
#include <vector>

#include "document.h"



template <typename Container>
auto Paginate(const Container& c, size_t page_size)
{
    std::vector<std::vector<Document>> Paginator;
    int cnt = 0;
    for (auto doc : c)
    {
        if (cnt == page_size) cnt = 0;
        if (cnt == 0) Paginator.push_back(std::vector<Document>(page_size));
        Paginator.at(Paginator.size() - 1).at(cnt) = doc;
        ++cnt;
    }

    return Paginator;
}
