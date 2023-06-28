#include "remove_duplicates.h"

void RemoveDuplicates(SearchServer& search_server) {
    for (auto id : search_server.GetIdWords()) {
        std::cout << "Found duplicate document id " << id << std::endl;
        search_server.RemoveDocument(id);
    }
}