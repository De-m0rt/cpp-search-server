#pragma once
#include <iostream>
#include <vector>

struct Document {
    Document() = default;

    Document(int id, double relevance, int rating)
        : id(id)
        , relevance(relevance)
        , rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

std::ostream& operator<<(std::ostream& out, std::vector<Document> page)
{
    for(auto doc : page)
    {
        out<<"{ ";
        out<<"document_id = "<<doc.id<<", "<<"relevance = "<<doc.relevance<<", "<<"rating = "<<doc.rating;
        out<<" }";
    }
    return out;
}


