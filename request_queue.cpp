
#include <algorithm>
#include <vector>
#include <deque>
#include <string>

#include "search_server.h"
#include "request_queue.h"


    int RequestQueue::GetNoResultRequests() const
    {
        return std::count_if(requests_.begin(),requests_.end(),[](auto item){return item.GetRes() == 0;});

    }

    int RequestQueue::QueryResult::GetRes()
    {
        return res_;
    }


