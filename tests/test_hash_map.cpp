#include "hashmap.hpp"
#include <gtest/gtest.h>

TEST(HashMapTest, InsertElements) {
    HashMap<string, int> map = HashMap<string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    EXPECT_EQ(map.size(), 2);

    auto iter = map.find("one");
    int v = (*iter).value; // Dereference the iterator and access value
    EXPECT_NE(iter, map.end()); 
    EXPECT_EQ(v, 1);

    iter = map.find("two");
    v = (*iter).value;
    EXPECT_NE(iter, map.end());
    EXPECT_EQ(v, 2);
}

TEST(HashMapTest, FindElements) {
    HashMap<string, int> map = HashMap<string, int>();
    map.insert("two", 2);
    map.insert("three", 3);
    map.insert("seven", 7);
    map.insert("nine", 9);
    map.insert("ten", 10);

    auto iter = map.find("nine");
    int v = (*iter).value;
    EXPECT_NE(iter, map.end());
    EXPECT_EQ(v, 9);
}

TEST(HashMapTest, EraseElements) {
    HashMap<string, int> map = HashMap<string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    map.erase("one");
    map.erase("three");

    EXPECT_EQ(map.size(), 3); // Lazy deletion, size is not decremented
}

TEST(HashMapTest, EmptyHashMap) {
    HashMap<string, int> map = HashMap<string, int>();
    EXPECT_EQ(map.size(), 0);
    EXPECT_TRUE(map.empty());
}

TEST(HashMapTest, RehashElements) {
    HashMap<string, int> map = HashMap<string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);
    map.insert("four", 4);
    map.insert("five", 5);
    map.insert("six", 6);
    map.insert("seven", 7);
    map.insert("eight", 8);
    map.insert("nine", 9);
    map.insert("ten", 10);
    map.insert("eleven", 11);
    map.insert("twelve", 12);
    map.insert("thirteen", 13);
    map.insert("fourteen", 14);
    map.insert("fifteen", 15);
    map.insert("sixteen", 16);
    map.insert("seventeen", 17);
    map.insert("eighteen", 18);
    map.insert("nineteen", 19);
    map.insert("twenty", 20);
    map.insert("twentyone", 21);
    map.insert("twentytwo", 22);
    map.insert("twentythree", 23);
    map.insert("twentyfour", 24);
    map.insert("twentyfive", 25);
    map.insert("twentysix", 26);
    map.insert("twentyseven", 27);
    map.insert("twentyeight", 28);
    map.insert("twentynine", 29);
    map.insert("thirty", 30);
    map.insert("thirtyone", 31);
    map.insert("thirtytwo", 32);
    map.insert("thirtythree", 33);
    map.insert("thirtyfour", 34);
    map.insert("thirtyfive", 35);
    map.insert("thirtysix", 36);
    map.insert("thirtyseven", 37);
    map.insert("thirtyeight", 38);
    map.insert("thirtynine", 39);
    map.insert("forty", 40);
    map.insert("fortyone", 41);
    map.insert("fortytwo", 42);
    map.insert("fortythree", 43);
    map.insert("fortyfour", 44);
    map.insert("fortyfive", 45);
    map.insert("fortysix", 46);
    map.insert("fortyseven", 47);
    map.insert("fortyeight", 48);
    map.insert("fortynine", 49);
    map.insert("fifty", 50);

    EXPECT_EQ(map.size(), 50); // Capacity is 40, load factor is 0.75
}
