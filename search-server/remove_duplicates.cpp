#include "remove_duplicates.h"

void RemoveDuplicates(SearchServer& search_server) {
<<<<<<< HEAD
    std::set<std::set<std::string>> words;
    for (auto document_id = search_server.begin(); document_id != search_server.end(); ) {
        std::set<string> strings;
        for (const auto& [word, freq] : search_server.GetWordFrequencies(*document_id)) {
            strings.insert(word);
        }
        if (words.find(strings) != words.end()) {
            std::cout << "Found duplicate document id " << *document_id << std::endl;
            search_server.RemoveDocument(*document_id);
        }
        else {
            words.insert(strings);
            ++document_id;
        }
=======
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
>>>>>>> aaa61b8b86cd71e43fb7dad29b09670d2a2f0785
    }
}