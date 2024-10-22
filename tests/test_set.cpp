#include "set.hpp"
#include <gtest/gtest.h>

// TEST: GIVEN an empty Set WHEN checking if it's empty THEN it should return true.
TEST(SetTest, DefaultConstructor_IsEmpty) {
    Set<int> set;
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0);
}

// TEST: GIVEN a Set with initial capacity WHEN checking its capacity THEN it should match the provided capacity.
TEST(SetTest, Constructor_WithCapacity) {
    Set<int> set(5);
    EXPECT_EQ(set.capacity(), 5);
}

// TEST: GIVEN a Set with initializer list WHEN checking its size THEN it should match the number of unique elements.
TEST(SetTest, InitializerListConstructor) {
    Set<int> set = {1, 2, 3, 2};  
    EXPECT_EQ(set.size(), 3);  
    EXPECT_FALSE(set.empty());
}

// TEST: GIVEN a Set WHEN adding unique elements THEN size should increase.
TEST(SetTest, Insert_IncreasesSize) {
    Set<int> set;
    set.insert(10);
    set.insert(20);
    set.insert(10);  
    EXPECT_EQ(set.size(), 2);
    EXPECT_TRUE(set.contains(10));
    EXPECT_TRUE(set.contains(20));
}

// TEST: GIVEN a Set WHEN inserting elements THEN it should prevent duplicates.
TEST(SetTest, Insert_PreventsDuplicates) {
    Set<int> set;
    set.insert(1);
    set.insert(1); 
    EXPECT_EQ(set.size(), 1);  
    EXPECT_TRUE(set.contains(1));
}

// TEST: GIVEN a Set WHEN erasing an element THEN the size should decrease and element should not be found.
TEST(SetTest, Erase_RemovesElement) {
    Set<int> set = {1, 2, 3};
    set.erase(2);
    EXPECT_EQ(set.size(), 2);
    EXPECT_FALSE(set.contains(2));
    EXPECT_TRUE(set.contains(1));
    EXPECT_TRUE(set.contains(3));
}

// TEST: GIVEN a Set WHEN using the copy constructor THEN the new set should have the same elements.
TEST(SetTest, CopyConstructor_CopiesElements) {
    Set<int> set = {1, 2, 3};
    Set<int> copiedSet = set;  
    EXPECT_EQ(copiedSet.size(), 3);
    EXPECT_TRUE(copiedSet.contains(1));
    EXPECT_TRUE(copiedSet.contains(2));
    EXPECT_TRUE(copiedSet.contains(3));
}

// TEST: GIVEN a Set WHEN using the move constructor THEN ownership should be transferred.
TEST(SetTest, MoveConstructor_TransfersOwnership) {
    Set<int> set = {1, 2, 3};
    Set<int> movedSet = std::move(set);  
    EXPECT_EQ(movedSet.size(), 3);
    EXPECT_TRUE(set.empty()); 
}

// TEST: GIVEN a Set WHEN clearing it THEN it should be empty.
TEST(SetTest, Clear_EmptiesTheSet) {
    Set<int> set = {1, 2, 3};
    set.clear();
    EXPECT_EQ(set.size(), 0);
    EXPECT_TRUE(set.empty());
}

// TEST: GIVEN a Set WHEN inserting a range of elements THEN they should be inserted correctly without duplicates.
TEST(SetTest, InsertRange_InsertsCorrectly) {
    Set<int> set1 = {1, 2, 3};
    Set<int> set2;
    set2.insert(set1.begin(), set1.end()); 
    EXPECT_EQ(set2.size(), 3);
    EXPECT_TRUE(set2.contains(1));
    EXPECT_TRUE(set2.contains(2));
    EXPECT_TRUE(set2.contains(3));
}

// TEST: GIVEN a Set WHEN computing intersection THEN the correct common elements should be returned.
TEST(SetTest, Intersect_ReturnsCommonElements) {
    Set<int> set1 = {1, 2, 3};
    Set<int> set2 = {2, 3, 4};
    Set<int> intersection = set1.intersect(set2);
    EXPECT_EQ(intersection.size(), 2);
    EXPECT_TRUE(intersection.contains(2));
    EXPECT_TRUE(intersection.contains(3));
}

// TEST: GIVEN a Set WHEN erasing elements THEN size should decrease and duplicates are not allowed.
TEST(SetTest, Erase_And_InsertPreventsDuplicates) {
    Set<int> set = {1, 2, 3};
    set.erase(2);
    set.insert(2);
    EXPECT_EQ(set.size(), 3);
    EXPECT_TRUE(set.contains(1));
    EXPECT_TRUE(set.contains(2));
    EXPECT_TRUE(set.contains(3));
}

// TEST: GIVEN a Set WHEN using the assignment operator THEN elements should be copied correctly.
TEST(SetTest, AssignmentOperator_CopiesElements) {
    Set<int> set = {1, 2, 3};
    Set<int> assignedSet;
    assignedSet = set;  
    EXPECT_EQ(assignedSet.size(), 3);
    EXPECT_TRUE(assignedSet.contains(1));
    EXPECT_TRUE(assignedSet.contains(2));
    EXPECT_TRUE(assignedSet.contains(3));
}

// TEST: GIVEN a Set WHEN using move assignment THEN ownership should be transferred.
TEST(SetTest, MoveAssignmentOperator_TransfersOwnership) {
    Set<int> set = {1, 2, 3};
    Set<int> movedSet;
    movedSet = std::move(set); 
    EXPECT_EQ(movedSet.size(), 3);
    EXPECT_TRUE(set.empty()); 
}

// TEST: GIVEN a Set WHEN resizing beyond capacity THEN the capacity should grow to accommodate new elements.
TEST(SetTest, Resize_HandlesLargerCapacity) {
    Set<int> set;
    for (int i = 0; i < 100; ++i) {
        set.insert(i);
    }
    EXPECT_EQ(set.size(), 100);
    EXPECT_GE(set.capacity(), 100);
}
