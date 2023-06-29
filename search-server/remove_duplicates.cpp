#include "remove_duplicates.h"

void RemoveDuplicates(SearchServer& search_server) {
    std::set<int> duplicates;
    auto id_words = search_server.GetIdWords();
    for (auto it = id_words.begin(); it != id_words.end(); ++it) {
        auto jt = it;
        for (++jt; jt != id_words.end(); ++jt) {
            if (it->second == jt->second) {
                duplicates.insert(jt->first);
            }
        }
    }
    for (const auto document_id : duplicates) {
        std::cout << "Found duplicate document id " << document_id << std::endl;
        search_server.RemoveDocument(document_id);
    }
}