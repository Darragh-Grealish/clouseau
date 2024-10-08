#include "array_list.hpp"
#include <gtest/gtest.h>

// TEST: GIVEN an empty ArrayList WHEN size is called THEN it returns 0
TEST(ArrayListTest, SizeOfEmptyList) {
  ArrayList<int> list;
  EXPECT_EQ(list.size(), 0);
}

// TEST: GIVEN an ArrayList with elements WHEN size is called THEN it returns
// the correct size
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

// TEST: GIVEN an ArrayList with elements WHEN get is called THEN it returns the
// correct element
TEST(ArrayListTest, GetElement) {
  ArrayList<int> list;
  list.add(1);
  list.add(2);
  EXPECT_EQ(list.get(1), 2);
}

// TEST: GIVEN an ArrayList with elements WHEN remove is called THEN the element
// is removed
TEST(ArrayListTest, RemoveElement) {
  ArrayList<int> list;
  list.add(1);
  list.add(2);
  list.remove(0);
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.get(0), 2);
}

// TEST: GIVEN an ArrayList WHEN remove is called on an empty list THEN it
// throws an exception
TEST(ArrayListTest, RemoveFromEmptyList) {
  ArrayList<int> list;
  EXPECT_THROW(list.remove(0), std::out_of_range);
}

// TEST: GIVEN an ArrayList with elements WHEN contains is called THEN it
// returns true if the element is present
TEST(ArrayListTest, ContainsElement) {
  ArrayList<int> list;
  list.add(1);
  list.add(2);
  EXPECT_TRUE(list.contains(1));
  EXPECT_FALSE(list.contains(3));
}

// TEST: GIVEN an ArrayList with elements WHEN contains is called for removed
// elements THEN it returns false
TEST(ArrayListTest, ContainsRemovedElement) {
  ArrayList<int> list;
  list.add(1);
  list.add(2);
  list.remove(0);
  EXPECT_FALSE(list.contains(1));
}

// TEST: GIVEN an ArrayList WHEN initialized with an initializer list THEN it
// contains the correct elements
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

// TEST: GIVEN an ArrayList WHEN add is called with an index THEN the element is
// added at the correct position
TEST(ArrayListTest, AddElementAtIndex) {
  ArrayList<int> list;
  list.add(1);
  list.add(3);
  list.add(2, 1);
  EXPECT_EQ(list.get(1), 2);
  EXPECT_EQ(list.get(2), 3);
}

// TEST: GIVEN an ArrayList WHEN adding more elements than initial capacity THEN
// it resizes correctly
TEST(ArrayListTest, ResizeOnAdd) {
  ArrayList<int> list;
  for (int i = 0; i < 15; ++i) {
    list.add(i);
  }
  EXPECT_EQ(list.size(), 15);
  EXPECT_EQ(list.get(14), 14);
}

// TEST: GIVEN an ArrayList WHEN isEmpty is called THEN it returns true if the
// list is empty
TEST(ArrayListTest, IsEmpty) {
  ArrayList<int> list;
  EXPECT_TRUE(list.isEmpty());
  list.add(1);
  EXPECT_FALSE(list.isEmpty());
}

// TEST: GIVEN an ArrayList WHEN get is called with an out-of-bounds index THEN
// it throws an exception
TEST(ArrayListTest, GetOutOfBounds) {
  ArrayList<int> list;
  list.add(1);
  EXPECT_THROW(list.get(1), std::out_of_range);
  EXPECT_THROW(list.get(-1), std::out_of_range);
}

// TEST: GIVEN an ArrayList WHEN an element is removed and then added again THEN
// it behaves as expected
TEST(ArrayListTest, RemoveAndAddAgain) {
  ArrayList<int> list;
  list.add(1);
  list.add(2);
  list.remove(0);
  list.add(3);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.get(0), 2);
  EXPECT_EQ(list.get(1), 3);
}

// TEST: GIVEN two ArrayLists WHEN one is copied THEN it behaves independently
TEST(ArrayListTest, CopyConstructor) {
  ArrayList<int> list1;
  list1.add(1);
  list1.add(2);
  ArrayList<int> list2(list1);
  list1.remove(0);
  EXPECT_EQ(list1.size(), 1);
  EXPECT_EQ(list2.size(), 2);
  EXPECT_EQ(list2.get(0), 1);
  EXPECT_EQ(list2.get(1), 2);
}

// TEST: GIVEN two ArrayLists WHEN one is moved THEN the original is in a valid
// but empty state
TEST(ArrayListTest, MoveConstructor) {
  ArrayList<int> list1;
  list1.add(1);
  list1.add(2);
  ArrayList<int> list2(std::move(list1));
  EXPECT_EQ(list2.size(), 2);
  EXPECT_EQ(list1.size(), 0);
}

// TEST: GIVEN an ArrayList with multiple elements WHEN removing elements in
// sequence THEN the list updates correctly
TEST(ArrayListTest, MultipleRemovals) {
  ArrayList<int> list;
  for (int i = 0; i < 5; ++i) {
    list.add(i);
  }
  list.remove(0);
  EXPECT_EQ(list.size(), 4);
  EXPECT_EQ(list.get(0), 1);
  list.remove(2);
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list.get(2), 4);
}
