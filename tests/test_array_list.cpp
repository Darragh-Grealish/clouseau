#include "array_list.hpp"
#include <gtest/gtest.h>


// TEST: GIVEN an empty ArrayList WHEN checking if it's empty THEN it should return true.
TEST(ArrayListTest, DefaultConstructor_IsEmpty) {
    ArrayList<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

// TEST: GIVEN an ArrayList with initial capacity WHEN checking its capacity THEN it should match the provided capacity.
TEST(ArrayListTest, Constructor_WithCapacity) {
    ArrayList<int> list(5);
    EXPECT_EQ(list.capacity(), 5);
}

// TEST: GIVEN an ArrayList with initializer list WHEN checking its size THEN it should match the number of elements in the list.
TEST(ArrayListTest, InitializerListConstructor) {
    ArrayList<int> list = {1, 2, 3};
    EXPECT_EQ(list.size(), 3);
    EXPECT_FALSE(list.empty());
}

// TEST: GIVEN an ArrayList WHEN adding elements THEN size should increase and values should be accessible.
TEST(ArrayListTest, PushBack_IncreasesSize) {
    ArrayList<int> list;
    list.push_back(10);
    list.push_back(20);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list[0], 10);
    EXPECT_EQ(list[1], 20);
}

// TEST: GIVEN an ArrayList WHEN pushing back more elements than the initial capacity THEN it should resize appropriately.
TEST(ArrayListTest, PushBack_ResizesArray) {
    ArrayList<int> list(2); // Capacity 2
    list.push_back(1);
    list.push_back(2);
    list.push_back(3); // Should trigger resize
    EXPECT_EQ(list.size(), 3);
    EXPECT_GE(list.capacity(), 3); // Ensure the capacity has grown
}

// TEST: GIVEN an ArrayList WHEN using at() to access an element THEN it should throw for invalid indices.
TEST(ArrayListTest, At_ThrowsExceptionOnOutOfBounds) {
    ArrayList<int> list = {1, 2, 3};
    EXPECT_NO_THROW(list.at(0));
    EXPECT_THROW(list.at(10), std::out_of_range);
}

// TEST: GIVEN an ArrayList WHEN using the copy constructor THEN the new list should have the same elements.
TEST(ArrayListTest, CopyConstructor_CopiesElements) {
    ArrayList<int> list = {1, 2, 3};
    ArrayList<int> copiedList = list; // Copy constructor
    EXPECT_EQ(copiedList.size(), 3);
    EXPECT_EQ(copiedList[0], 1);
    EXPECT_EQ(copiedList[1], 2);
    EXPECT_EQ(copiedList[2], 3);
}

// TEST: GIVEN an ArrayList WHEN using the move constructor THEN ownership should be transferred.
TEST(ArrayListTest, MoveConstructor_TransfersOwnership) {
    ArrayList<int> list = {1, 2, 3};
    ArrayList<int> movedList = std::move(list); // Move constructor
    EXPECT_EQ(movedList.size(), 3);
    EXPECT_TRUE(list.empty()); // Original list should be empty after move
}

// TEST: GIVEN an ArrayList WHEN clearing it THEN it should be empty.
TEST(ArrayListTest, Clear_EmptiesTheList) {
    ArrayList<int> list = {1, 2, 3};
    list.clear();
    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());
}

// TEST: GIVEN an ArrayList WHEN popping elements THEN size should decrease.
TEST(ArrayListTest, PopBack_DecreasesSize) {
    ArrayList<int> list = {1, 2, 3};
    list.pop_back();
    EXPECT_EQ(list.size(), 2);
    list.pop_back();
    EXPECT_EQ(list.size(), 1);
    list.pop_back();
    EXPECT_TRUE(list.empty());
}

// TEST: GIVEN an ArrayList WHEN iterating with begin() and end() THEN it should iterate over all elements.
TEST(ArrayListTest, Iterator_IteratesOverElements) {
    ArrayList<int> list = {1, 2, 3};
    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
}

// TEST: GIVEN an ArrayList WHEN using the assignment operator THEN elements should be copied correctly.
TEST(ArrayListTest, AssignmentOperator_CopiesElements) {
    ArrayList<int> list = {1, 2, 3};
    ArrayList<int> assignedList;
    assignedList = list; // Copy assignment
    EXPECT_EQ(assignedList.size(), 3);
    EXPECT_EQ(assignedList[0], 1);
    EXPECT_EQ(assignedList[1], 2);
    EXPECT_EQ(assignedList[2], 3);
}

// TEST: GIVEN an ArrayList WHEN using move assignment THEN ownership should be transferred.
TEST(ArrayListTest, MoveAssignmentOperator_TransfersOwnership) {
    ArrayList<int> list = {1, 2, 3};
    ArrayList<int> movedList;
    movedList = std::move(list); // Move assignment
    EXPECT_EQ(movedList.size(), 3);
    EXPECT_TRUE(list.empty()); // Original list should be empty after move
}

// TEST: GIVEN an ArrayList WHEN resizing beyond capacity THEN the capacity should grow to accommodate new elements.
TEST(ArrayListTest, Resize_HandlesLargerCapacity) {
    ArrayList<int> list;
    for (int i = 0; i < 100; ++i) {
        list.push_back(i);
    }
    EXPECT_EQ(list.size(), 100);
    EXPECT_GE(list.capacity(), 100);
}


