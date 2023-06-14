#include "document.h"

Document::Document(int id, double relevance, int rating)
            : id(id)
            , relevance(relevance)
            , rating(rating) {
    }

ostream &operator<<(ostream &os, const Document &document) {
    os << "{ "
       << "document_id = "s << document.id << ", "
       << "relevance = "s << document.relevance << ", "
       << "rating = "s << document.rating << " }";
    return os;
}