#ifndef POOL_HPP
#define POOL_HPP

#include <cstring>
#include <new>
#include "sllist.hpp"

template<typename T>
class Pool
{
private:
    struct Group {
        SLListNode listptr;

        T * items;
        char * item_states;
        int available;
    };
    typedef SLList<Group, &Group::listptr> GroupList;

    int size;
    GroupList groups;

    Group * allocNewGroup() {
        Group *group = new Group();
        group->available = size;
        group->items = (T*)new char[sizeof(T) * size];
        group->item_states = new char[size];
        memset(group->item_states, 0, sizeof(char)*size);
        groups.push_front(group);
        return group;
    }

    void freeGroup(Group *group) {
        delete[] (char*)group->items;
        delete[] group->item_states;
        delete group;
    }

    T * allocFromGroup(Group * group) {
        for (int i = 0; i < size; ++i) {
            if (group->item_states[i] == 0) {
                group->item_states[i] = 1;
                group->available--;
                T * item = &group->items[i];
                new (item) T();
                printf("Alloc %p(%d) from %p at %d\n", item, item - group->items, group, i);
                return item;
            }
        }
        return NULL;
    }

    void freeFromGroup(Group * group, T * ptr) {
        ptr->~T();

        int idx = ptr - group->items;
        printf("Dealloc %p(%d) from %p at %d\n", ptr, ptr - group->items, group, idx);
        group->item_states[idx] = 0;
        group->available++;
    }

public:
    Pool(int _size)
        : size(_size) {
        allocNewGroup();
    }

    T * alloc() {
        for(GroupList::Iterator i = groups.begin(); i != groups.end(); ++i) {
            if (i->available > 0) {
                return allocFromGroup(i);
            }
        }
        
        Group *newgroup = allocNewGroup();
        return allocFromGroup(newgroup);
    }

    void free(T * ptr) {
        for (GroupList::Iterator i = groups.begin(); i != groups.end(); ++i) {
            if (ptr >= i->items && ptr < i->items + sizeof(T) * size) {
                freeFromGroup(i, ptr);
                return;
            }
        }
    }

    void reset() {
        for(GroupList::Iterator i = groups.begin(); i != groups.end(); ++i) {
            freeGroup(i);
        }
        groups.clear();
        allocNewGroup();
    }

};

#endif // POOL_HPP
