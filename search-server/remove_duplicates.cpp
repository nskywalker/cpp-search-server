#include "remove_duplicates.h"

void RemoveDuplicates(SearchServer& search_server) {
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
    }
}