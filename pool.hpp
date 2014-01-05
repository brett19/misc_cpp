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
        unsigned char * item_states;
        int available;
    };
    typedef SLList<Group, &Group::listptr> GroupList;

    int size;
    GroupList groups;

    Group * allocNewGroup() {
        Group *group = new Group();
        group->available = size;
        group->items = (T*)new char[sizeof(T) * size];
        group->item_states = new unsigned char[size >> 3];
        memset(group->item_states, 0, size >> 3);
        groups.push_front(group);
        return group;
    }

    void freeGroup(Group *group) {
        delete[] (char*)group->items;
        delete[] group->item_states;
        delete group;
    }

    T * allocFromGroup(Group * group) {
        // Bit Packing!
        int idx = -1;
        for (int i = 0; i < (size >> 3); ++i) {
            if (group->item_states[i] != 0xFF) {
                // Manual loop unroll for performance
                if (!(group->item_states[i] & (1<<0))) { idx = (i<<3) + 0; break; }
                if (!(group->item_states[i] & (1<<1))) { idx = (i<<3) + 1; break; }
                if (!(group->item_states[i] & (1<<2))) { idx = (i<<3) + 2; break; }
                if (!(group->item_states[i] & (1<<3))) { idx = (i<<3) + 3; break; }
                if (!(group->item_states[i] & (1<<4))) { idx = (i<<3) + 4; break; }
                if (!(group->item_states[i] & (1<<5))) { idx = (i<<3) + 5; break; }
                if (!(group->item_states[i] & (1<<6))) { idx = (i<<3) + 6; break; }
                if (!(group->item_states[i] & (1<<7))) { idx = (i<<3) + 7; break; }
            }
        }

        if (idx >= 0) {
            group->item_states[idx>>3] |= 1 << (idx&7);
            group->available--;
            T * item = &group->items[idx];
            new (item) T();
            return item;
        } else {
            return NULL;
        }
    }

    void freeFromGroup(Group * group, T * ptr) {
        ptr->~T();

        int idx = ptr - group->items;
        group->item_states[idx >> 3] &= ~(1 << (idx&7));
        group->available++;
    }

public:
    Pool(int _size)
        : size(_size) {
        // Align allocations to 8
        if ((size&3) > 0) {
            size += 8-(size&3);
        }
        
        // Alloc first group
        allocNewGroup();
    }

    T * alloc() {
        for (typename GroupList::Iterator i = groups.begin(); i != groups.end(); ++i) {
            if (i->available > 0) {
                return allocFromGroup(i);
            }
        }
        
        Group *newgroup = allocNewGroup();
        return allocFromGroup(newgroup);
    }

    void free(T * ptr) {
        for (typename GroupList::Iterator i = groups.begin(); i != groups.end(); ++i) {
            if (ptr >= i->items && ptr < i->items + sizeof(T) * size) {
                freeFromGroup(i, ptr);
                return;
            }
        }
    }

    void reset() {
        for(typename GroupList::Iterator i = groups.begin(); i != groups.end(); ++i) {
            freeGroup(i);
        }
        groups.clear();
        allocNewGroup();
    }

};

#endif // POOL_HPP
