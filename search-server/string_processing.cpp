#include "string_processing.h"

std::vector<std::string_view> SplitIntoWords(std::string_view text) {
    std::vector<std::string_view> words;
    size_t space = 0;
    while(space != std::string_view::npos) {
        size_t prev = space;
        space = text.find(' ', prev);
        if (space != std::string_view::npos) {
            words.push_back(text.substr(prev, space - prev));
            ++space;
        }
        else {
            words.push_back(text.substr(prev, space));
        }
    }
    return words;
}

