#include "array_list.hpp"
#include <gtest/gtest.h>

// TEST: GIVEN an empty ArrayList WHEN size is called THEN it returns 0
TEST(ArrayListTest, SizeOfEmptyList) {
    ArrayList<int> list;
    EXPECT_EQ(list.size(), 0);
}

// TEST: GIVEN an ArrayList with elements WHEN size is called THEN it returns the correct size
TEST(ArrayListTest, SizeOfNonEmptyList) {
    ArrayList<int> list;
    list.add(1);
    list.add(2);
    EXPECT_EQ(list.size(), 2);
}

// TEST: GIVEN an ArrayList WHEN add is called THEN the element is added
TEST(ArrayListTest, AddElement) {
    ArrayList<int> list;
    list.add(1);
    EXPECT_EQ(list.get(0), 1);
}

// TEST: GIVEN an ArrayList with elements WHEN get is called THEN it returns the correct element
TEST(ArrayListTest, GetElement) {
    ArrayList<int> list;
    list.add(1);
    list.add(2);
    EXPECT_EQ(list.get(1), 2);
}

// TEST: GIVEN an ArrayList with elements WHEN remove is called THEN the element is removed
TEST(ArrayListTest, RemoveElement) {
    ArrayList<int> list;
    list.add(1);
    list.add(2);
    list.remove(0);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), 2);
}

// TEST: GIVEN an ArrayList with elements WHEN contains is called THEN it returns true if the element is present
TEST(ArrayListTest, ContainsElement) {
    ArrayList<int> list;
    list.add(1);
    list.add(2);
    EXPECT_TRUE(list.contains(1));
    EXPECT_FALSE(list.contains(3));
}

// TEST: GIVEN an ArrayList WHEN initialized with an initializer list THEN it contains the correct elements
TEST(ArrayListTest, InitializerList) {
    ArrayList<int> list = {1, 2, 3};
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 3);
}

// TEST: GIVEN an ArrayList WHEN iterated THEN it iterates over all elements
TEST(ArrayListTest, Iterator) {
    ArrayList<int> list = {1, 2, 3};
    int sum = 0;
    for (int value : list) {
        sum += value;
    }
    EXPECT_EQ(sum, 6);
}

// TEST: GIVEN an ArrayList WHEN add is called with an index THEN the element is added at the correct position
TEST(ArrayListTest, AddElementAtIndex) {
    ArrayList<int> list;
    list.add(1);
    list.add(3);
    list.add(2, 1);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 3);
}

// TEST: GIVEN an ArrayList WHEN isEmpty is called THEN it returns true if the list is empty
TEST(ArrayListTest, IsEmpty) {
    ArrayList<int> list;
    EXPECT_TRUE(list.isEmpty());
    list.add(1);
    EXPECT_FALSE(list.isEmpty());
}

