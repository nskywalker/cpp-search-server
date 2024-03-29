#pragma once
#include <iostream>

using namespace std;

struct Document {
    Document() = default;
    Document(int id, double relevance, int rating);
    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

ostream &operator<<(ostream &os, const Document &document);