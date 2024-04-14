#include "document.h"



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

