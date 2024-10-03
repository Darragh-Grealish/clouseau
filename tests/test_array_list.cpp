#include "array_list.hpp"
#include <gtest/gtest.h>

TEST(ArrayListTest, AddElements) {
    ArrayList<int> list = ArrayList<int>();
    list.add(1);
    list.add(2);
    list.add(3);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 3);
}

TEST(ArrayListTest, GetElements) {
    ArrayList<int> list = ArrayList<int>();
    list.add(1);
    list.add(2);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
}

TEST(ArrayListTest, RemoveElements) {
    ArrayList<int> list = *new ArrayList<int>();
    list.add(1);
    list.add(2);
    list.remove(0);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), 2);
}

