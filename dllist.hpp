#ifndef DLLIST_HPP
#define DLLIST_HPP

#include <cstddef>
#include <cstdlib>

struct DLListNode {
    DLListNode() 
        : prev(NULL), next(NULL) {
    }

    DLListNode *prev;
    DLListNode *next;
};

template <typename T, DLListNode T::*M>
class DLList {
public:
    class Iterator {
        friend class DLList<T, M>;
    private:
        Iterator()
            : prev(NULL), cur(NULL), next(NULL) {
        }

        Iterator(DLListNode *node)
            : prev(node ? node->prev : NULL), cur(node), next(node ? node->next : NULL) {
        }

        DLListNode *prev;
        DLListNode *cur;
        DLListNode *next;

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

        Iterator & operator--() {
            next = cur;
            cur = prev;
            if (cur) {
                prev = cur->prev;
            } else {
                prev = NULL;
            }
            return *this;
        }

        Iterator & operator++(int) {
            Iterator i = *this;
            ++(*this);
            return i;
        }

        Iterator & operator--(int) {
            Iterator i = *this;
            --(*this);
            return i;
        }
    };
    
    // Helpers, static so they are compile-time calculated
    static DLListNode * toNode(T *item) {
        return (DLListNode*)((char*)item + ((size_t)(&(((T*)0)->*M))));
    }
    static T * fromNode(DLListNode *node) {
        return (T*)((char*)node - ((size_t)(&(((T*)0)->*M))));
    }

    DLList()
        : first(NULL), last(NULL) {
    }

    ~DLList() {

    }

    Iterator begin() const {
        return Iterator(first);
    }

    Iterator end() const {
        return Iterator();
    }

    Iterator rbegin() const {
        return Iterator(last);
    }

    Iterator rend() const {
        return Iterator();
    }

    Iterator find(T *item) const {
        DLListNode *node = toNode(item);
        return Iterator(node);
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
        } else {
            iter.next->prev = iter.prev;
        }
    }

    void clear() {
        first = NULL;
        last = NULL;
    }

    void push_back(T *item) {
        DLListNode *node = toNode(item);
        node->prev = last;
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
        DLListNode *node = toNode(item);
        node->prev = NULL;
        node->next = first;
        if (first) {
            first->prev = node;
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

        DLListNode *node = first;
        first = first->next;
        if (first) {
            first->prev = NULL;
        } else {
            last = NULL;
        }
        node->next = NULL;
        node->prev = NULL;
        return fromNode(node);
    }

    T * pop_back() {
        if (last == NULL) {
            return NULL;
        }

        DLListNode *node = last;
        last = last->prev;
        if (last) {
            last->next = NULL;
        } else {
            first = NULL;
        }
        node->next = NULL;
        node->prev = NULL;
        return fromNode(node);
    }

private:
    DLListNode *first;
    DLListNode *last;
};

#endif // DLLIST_HPP
