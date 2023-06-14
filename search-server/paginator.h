#pragma once
#include <iostream>
#include <vector>

using namespace std;

template <typename Iterator>
class IteratorRange {
    Iterator start;
    Iterator finish;
    int page_size;
public:
    explicit IteratorRange(Iterator begin_, Iterator end_, int size) : start(begin_), finish(end_), page_size(size) {}
    const Iterator begin() const {
        return start;
    }
    const Iterator end() const {
        return finish;
    }
    int size() const {
        return page_size;
    }
};

template<typename Iterator>
ostream &operator<<(ostream &os, const IteratorRange<Iterator> &result) {
    auto p = result.begin();
    for (int i = 0; i < result.size(); ++i) {
        os << *p;
        advance(p, 1);
    }
    return os;
}

template <typename Iterator>
class Paginator {
    vector<IteratorRange<Iterator>> result;
public:
    Paginator(Iterator begin_, Iterator end_, int page_size_) {
        int dist = distance(begin_, end_);
        int mode = dist / page_size_;
        for (int i = 0; i < dist; i += page_size_) {
            Iterator curEnd = begin_;
            if (i == mode * page_size_) {
                page_size_ = page_size_ - (dist % page_size_);
            }
            advance(curEnd, page_size_ - 1);
            result.push_back(IteratorRange<Iterator>{begin_, curEnd, page_size_});
            begin_ = curEnd;
            advance(begin_, 1);
        }

    }
    auto begin() const {
        return result.begin();
    }
    auto end() const {
        return result.end();
    }
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}