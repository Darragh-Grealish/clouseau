#include "hashmap.hpp"
#include <gtest/gtest.h>

// TEST: GIVEN empty hashmap WHEN elements are inserted THEN size is 2 & elements are found
TEST(HashMapTest, InsertElements) {
    HashMap<string, int> map = HashMap<string, int>();
    EXPECT_EQ(map.size(), 0);
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

// TEST: GIVEN hashmap of elements WHEN find("nine") is called THEN return 9
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

// TEST: GIVEN a hashmap with 3 elements WHEN 2 are flagged deleted THEN size is still 3
TEST(HashMapTest, EraseElements) {
    HashMap<string, int> map = HashMap<string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    EXPECT_EQ(map.size(), 3);

    map.erase("one");
    map.erase("three");

    EXPECT_EQ(map.size(), 3);
}

// TEST: GIVEN an empty hashmap WHEN size() & empty() are called THEN return 0 and true respectively
TEST(HashMapTest, EmptyHashMap) {
    HashMap<string, int> map = HashMap<string, int>();
    EXPECT_EQ(map.size(), 0);
    EXPECT_TRUE(map.empty());
}

// TEST: GIVEN 41 inserts WHEN loadfactor>0.75 THEN rehashing should occur & size is doubled from 40 to 80
TEST(HashMapTest, RehashElements) {
    HashMap<string, int> map = HashMap<string, int>();
    EXPECT_EQ(map.size(), 0);

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

    EXPECT_EQ(map.size(), 41);

    auto iter = map.find("thirty");
    auto v = (*iter).value;
    EXPECT_EQ(v, 30);

    iter = map.find("fortyone");
    v = (*iter).value;
    EXPECT_EQ(v, 41);
}
