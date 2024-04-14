#pragma once

#include <algorithm>
#include <vector>
#include <deque>
#include <string>

#include "search_server.h"


class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server) :
        search_server_(search_server) {
    }

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        auto result = search_server_.FindTopDocuments(raw_query, document_predicate);
        if (result.size() == 0)
        {
            if (requests_.size() == min_in_day_) requests_.pop_front();
            requests_.push_back(0);
        }
        else
        {
            if (requests_.size() == min_in_day_) requests_.pop_front();
            requests_.push_back(1);
        }
        return search_server_.FindTopDocuments(raw_query, document_predicate);
    }

    std::vector<Document> AddFindRequest(const std::string& raw_query)
    {
        return AddFindRequest(raw_query, DocumentStatus::ACTUAL);
    }

    int GetNoResultRequests() const;

private:
    struct QueryResult
    {
        public:
        QueryResult (int res)
        {
            res_ = res;
        }

        int GetRes();

        private:
        int res_;
    };

   const SearchServer& search_server_;
   std::deque<QueryResult> requests_;
   const static int min_in_day_ = 1440;

};

