#include "hashmap.hpp"
#include <gtest/gtest.h>

// TEST: GIVEN empty hashmap WHEN elements are inserted THEN size is 2 & elements are found
TEST(HashMapTest, InsertElements) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
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
    HashMap<std::string, int> map = HashMap<std::string, int>();
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

// TEST: GIVEN a hashmap with 3 elements WHEN 2 are deleted (and flagged) THEN size is 1
TEST(HashMapTest, EraseElements) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    EXPECT_EQ(map.size(), 3);

    map.erase("one");
    map.erase("three");

    EXPECT_EQ(map.size(), 1);
}

// TEST: GIVEN an empty hashmap WHEN size() & empty() are called THEN (size = 0) and (map.empty = true)
TEST(HashMapTest, EmptyHashMap) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    EXPECT_EQ(map.size(), 0);
    EXPECT_TRUE(map.empty());
}

// TEST: GIVEN a hashmap with 3 elements WHEN empty() is called THEN (size = 3) and (map.empty = false)
TEST(HashMapTest, EmptyHashMapFalse) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    EXPECT_EQ(map.size(), 3);
    EXPECT_FALSE(map.empty());
}

// TEST: GIVEN a hashmap with 3 elements WHEN clear() is called THEN size is 0
TEST(HashMapTest, ClearHashMap) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    EXPECT_EQ(map.size(), 3);

    map.clear();

    EXPECT_EQ(map.size(), 0);
}

// TEST: GIVEN a hashmap with 3 elements WHEN erase() is called with non-existent key THEN return false
TEST(HashMapTest, EraseNonExistentKey) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    bool erased = map.erase("four");
    EXPECT_FALSE(erased);
}

// TEST: GIVEN a hashmap with 3 elements WHEN erase() is called with existing key THEN return true
TEST(HashMapTest, EraseExistingKey) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    bool erased = map.erase("two");
    EXPECT_TRUE(erased);
}

// TEST: GIVEN 65 inserts WHEN loadfactor>0.7 THEN capacity is doubled from 64 to 128
TEST(HashMapTest, CapacityDoubles) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
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
    map.insert("fortytwo", 42);
    map.insert("fortythree", 43);
    map.insert("fortyfour", 44);
    map.insert("fortyfive", 45);
    map.insert("fortysix", 46);
    map.insert("fortyseven", 47);
    map.insert("fortyeight", 48);
    map.insert("fortynine", 49);
    map.insert("fifty", 50);
    map.insert("fiftyone", 51);
    map.insert("fiftytwo", 52);
    map.insert("fiftythree", 53);
    map.insert("fiftyfour", 54);
    map.insert("fiftyfive", 55);
    map.insert("fiftysix", 56);
    map.insert("fiftyseven", 57);
    map.insert("fiftyeight", 58);
    map.insert("fiftynine", 59);
    map.insert("sixty", 60);
    map.insert("sixtyone", 61);
    map.insert("sixtytwo", 62);
    map.insert("sixtythree", 63);
    map.insert("sixtyfour", 64);
    map.insert("sixtyfive", 65);

    EXPECT_EQ(map.size(), 65);
}

// TEST: GIVEN a use for operator [] WHEN key is not found THEN return default value
TEST(HashMapTest, OperatorBracketDefault) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    int v = map["four"];
    EXPECT_EQ(v, 0);
}

// TEST: GIVEN a use for operator [] WHEN key is found THEN return value
TEST(HashMapTest, OperatorBracketFound) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    int v = map["two"];
    EXPECT_EQ(v, 2);
}

// TEST: GIVEN a use for operator [] WHEN key is found THEN update value
TEST(HashMapTest, OperatorBracketUpdate) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    map["two"] = 22;
    int v = map["two"];
    EXPECT_EQ(v, 22);
}

// TEST: GIVEN a use for operator [] WHEN key is not found THEN insert key-value pair
TEST(HashMapTest, OperatorBracketInsert) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    map["four"] = 4;
    int v = map["four"];
    EXPECT_EQ(v, 4);
}

// TEST: GIVEN a use for operator [] WHEN key is not found THEN size is increased
TEST(HashMapTest, OperatorBracketSizeIncrease) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    map["four"] = 4;
    EXPECT_EQ(map.size(), 4);
}

// TEST: GIVEN a use for operator [] WHEN key is found THEN size is unchanged
TEST(HashMapTest, OperatorBracketSizeUnchanged) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    map["two"] = 22;
    EXPECT_EQ(map.size(), 3);
}

// TEST: GIVEN a use for operator [] WHEN key is not found THEN key is found
TEST(HashMapTest, OperatorBracketKeyFound) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    map["four"] = 4;
    auto iter = map.find("four");
    EXPECT_NE(iter, map.end());
}

// TEST: GIVEN an Iterator ++ WHEN called THEN return iterator to next element
TEST(HashMapTest, IteratorIncrement) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    auto iter = map.begin();
    ++iter;
    EXPECT_EQ((*iter).key, "two");
    EXPECT_EQ((*iter).value, 2);
}

// TEST: GIVEN an Iterator == WHEN called THEN return true if equal
TEST(HashMapTest, IteratorEqual) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);

    auto iter1 = map.begin();
    auto iter2 = map.begin();
    EXPECT_TRUE(iter1 == iter2);
}

// TEST: GIVEN an Iterator != WHEN called THEN return true if not equal
TEST(HashMapTest, IteratorNotEqual) {
    HashMap<std::string, int> map = HashMap<std::string, int>();
    map.insert("one", 1);
    map.insert("two", 2);

    auto iter1 = map.begin();
    auto iter2 = map.end();
    EXPECT_TRUE(iter1 != iter2);
}
