#include "sllist.hpp"
#include "dllist.hpp"
#include "pool.hpp"
#include <cstdio>

struct Test
{
    Test() 
        : value("") {
    }

    Test(const char *_value)
        : value(_value) {
    }

    // First item for fun
    DLListNode list_node1;
    
    const char *value;
    
    // Anywhere in the struct!
    DLListNode list_node2;
};

int main() {
    // Just to make nicer code
    typedef DLList<Test, &Test::list_node1> TestList1;
    typedef DLList<Test, &Test::list_node2> TestList2;
    
    Pool<Test> testPool(2);

    printf("Pool test 1\n");
    for (int i = 0; i < 5; ++i) {
        Test *x = testPool.alloc();
        //printf("%p\n", x);
        testPool.free(x);
        x = testPool.alloc();
        //printf("%p\n", x);
    }

    testPool.reset();

    printf("Pool test 2\n");
    Test *holder[5];
    for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < 5; ++i) {
            holder[i] = testPool.alloc();
            printf("%p\n", holder[i]);
        }
        for (int i = 0; i < 5; ++i) {
            testPool.free(holder[i]);
        }
    }

    printf("SLList Tests\n");
    Test *test[] = {
        new Test("test 0"),
        new Test("test 1"),
        new Test("test 2"),
        new Test("test 3")
    };

    TestList1 list1;
    TestList2 list2;

    list1.push_back(test[0]);
    list1.push_back(test[2]);
    list1.push_back(test[1]);
    list1.push_back(test[3]);

    list2.push_back(test[3]);
    list2.push_back(test[2]);
    list2.push_back(test[1]);
    list2.push_back(test[0]);

    printf("%s\n", list1.front()->value);
    printf("%s\n", list1.back()->value);

    /*
    printf("List 1 - 1st\n");
    for (TestList1::FastIterator i = list1.fbegin(); i != list1.fend(); ++i) {
        printf("%s %p %p\n", i->value, &i->list_node1, (Test*)i, (void*)i);
    }
    */

    printf("List 1 - 2nd\n");
    for (TestList1::Iterator i = list1.begin(); i != list1.end(); ++i) {
        printf("%s %p %p\n", i->value, &i->list_node1, (Test*)i);
        list1.erase(i);
    }

    printf("List 1 - 3rd\n");
    for (TestList1::Iterator i = list1.begin(); i != list1.end(); ++i) {
        printf("%s %p %p\n", i->value, &i->list_node1, (Test*)i);
    }
    printf("%d\n", list2.empty());

    /*
    printf("List 2 - 1st\n");
    for (TestList2::FastIterator i = list2.fbegin(); i != list2.fend(); ++i) {
        printf("%s %p %p\n", i->value, &i->list_node1, (Test*)i);
    }
    */

    list2.remove(test[1]);

    printf("List 2 - 2nd\n");
    for (TestList2::Iterator i = list2.begin(); i != list2.end(); ++i) {
        printf("%s %p %p\n", i->value, &i->list_node1, (Test*)i);
    }

    list2.clear();

    printf("List 2 - 3rd\n");
    for (TestList2::Iterator i = list2.begin(); i != list2.end(); ++i) {
        printf("%s %p %p\n", i->value, &i->list_node1, (Test*)i);
    }
    printf("%d\n", list2.empty());

    system("PAUSE");
    return 0;
}
