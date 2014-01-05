#ifndef SLLIST_HPP
#define SLLIST_HPP

#include <cstddef>
#include <cstdlib>

struct SLListNode {
    SLListNode() 
        : next(NULL) {
    }

    SLListNode *next;
};

template <typename T, SLListNode T::*M>
class SLList {
public:
    class FastIterator {
        friend class SLList<T, M>;
    private:
        FastIterator()
            : cur(NULL) {
        }

        FastIterator(SLListNode *start)
            : cur(start) {
        }

        SLListNode *cur;

    public:
        T & operator*() const {
            return *fromNode(cur);
        }

        T * operator->() const {
            return fromNode(cur);
        }

        operator T*() {
            return fromNode(cur);
        }

        bool operator==(const FastIterator &other) const {
            return cur == other.cur;
        }

        bool operator!=(const FastIterator &other) const {
            return cur != other.cur;
        }

        FastIterator & operator++() {
            cur = cur->next;
            return *this;
        }

        FastIterator & operator++(int) {
            FastIterator i = *this;
            ++(*this);
            return i;
        }

    };

    class Iterator {
        friend class SLList<T, M>;
    private:
        Iterator()
            : prev(NULL), cur(NULL), next(NULL) {
        }

        Iterator(SLListNode *first_node)
            : prev(NULL), cur(first_node), next(first_node ? first_node->next : NULL) {
        }

        SLListNode *prev;
        SLListNode *cur;
        SLListNode *next;

    public:
        T & operator*() const {
            return *fromNode(cur);
        }

        T * operator->() const {
            return fromNode(cur);
        }

        operator T*() {
            return fromNode(cur);
        }

        bool operator==(const Iterator &other) const {
            return cur == other.cur;
        }

        bool operator!=(const Iterator &other) const {
            return cur != other.cur;
        }

        Iterator & operator++() {
            prev = cur;
            cur = next;
            if (cur) {
                next = cur->next;
            } else {
                next = NULL;
            }
            return *this;
        }

        Iterator & operator++(int) {
            Iterator i = *this;
            ++(*this);
            return i;
        }
    };
    
    // Helpers, static so they are compile-time calculated
    static SLListNode * toNode(T *item) {
        return (SLListNode*)((char*)item + ((size_t)(&(((T*)0)->*M))));
    }
    static T * fromNode(SLListNode *node) {
        return (T*)((char*)node - ((size_t)(&(((T*)0)->*M))));
    }

    SLList()
        : first(NULL), last(NULL) {
    }

    ~SLList() {

    }

    Iterator begin() const {
        return Iterator(first);
    }

    Iterator end() const {
        return Iterator();
    }

    FastIterator fbegin() const {
        return FastIterator(first);
    }

    FastIterator fend() const {
        return FastIterator();
    }

    FastIterator ffind(T * item) const {
        SLListNode *node = toNode(item);
        return FastIterator(node);
    }

    T * front() const {
        if (!first) {
            return NULL;
        }
        return fromNode(first);
    }

    T * back() const {
        if (!last) {
            return NULL;
        }
        return fromNode(last);
    }

    bool empty() const {
        return first == NULL;
    }

    void remove(T * item) {
        for (Iterator it = begin(); it != end(); ++it) {
            if ((T*)it == item) {
                erase(it);
                return;
            }
        }
        return;
    }

    void erase(Iterator iter) {
        if (iter.cur == first) {
            first = iter.next;
        } else {
            iter.prev->next = iter.next;
        }

        if (iter.cur == last) {
            last = iter.prev;
        }
    }

    void clear() {
        first = NULL;
        last = NULL;
    }

    void push_back(T *item) {
        SLListNode *node = toNode(item);
        node->next = NULL;
        if (last) {
            last->next = node;
            last = node;
        } else {
            first = node;
            last = node;
        }
    }

    void push_front(T *item) {
        SLListNode *node = toNode(item);
        node->next = first;
        if (first) {
            node->next = first;
            first = node;
        } else {
            first = node;
            last = node;
        }
    }

    T * pop_front() {
        if (first == NULL) {
            return NULL;
        }

        SLListNode *node = first;
        first = first->next;
        if (!first) {
            last = NULL;
        }
        node->next = NULL;
        return fromNode(node);
    }

private:
    SLListNode *first;
    SLListNode *last;
};

#endif // DLLIST_HPP
