#include "hashmap.hpp"
#include <gtest/gtest.h>

// TEST: GIVEN empty hashmap WHEN elements are inserted THEN size is 2 & elements are found
TEST(HashMapTest, InsertElements) {
    HashMap<std::string, int> map;
    EXPECT_EQ(map.size(), 0);
    map.insert("one", 1);
    map.insert("two", 2);
    EXPECT_EQ(map.size(), 2);

    auto iter1 = map.find("one");
    int v = (*iter1).second; // Dereference the iterator and access value
    EXPECT_NE(iter1, map.end()); 
    EXPECT_EQ(v, 1);

    auto iter2 = map.find("two");
    v = (*iter2).second;
    EXPECT_NE(iter2, map.end());
    EXPECT_EQ(v, 2);
}

// TEST: GIVEN hashmap of elements WHEN find("nine") is called THEN return 9
TEST(HashMapTest, FindElements) {
    HashMap<string, int> map = HashMap<string, int>();
    map.insert("two", 2);
    map.insert("three", 3);
    map.insert("seven", 7);
    map.insert("nine", 9);
    map.insert("ten", 10);

    auto iter = map.find("nine");
    int v = (*iter).second;
    EXPECT_NE(iter, map.end());
    EXPECT_EQ(v, 9);
}

// TEST: GIVEN a hashmap with 3 elements WHEN 2 are deleted THEN size is 1
TEST(HashMapTest, EraseElements) {
    HashMap<string, int> map = HashMap<string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    EXPECT_EQ(map.size(), 3);

    map.erase("one");
    map.erase("three");

    EXPECT_EQ(map.size(), 1);
}

// TEST: GIVEN an empty hashmap WHEN size() & empty() are called THEN return 0 and true respectively
TEST(HashMapTest, EmptyHashMap) {
    HashMap<string, int> map = HashMap<string, int>();
    EXPECT_EQ(map.size(), 0);
    EXPECT_TRUE(map.empty());
}
