#include <gtest/gtest.h>
#include <iostream>
#include "deque.h"

// Bradley Manzo
// 916953788
// ECS 36C Prof. Porquet
// Spring 2022

TEST(Deque, Empty) {
	Deque<int> dq;

	/* Should be fully empty */
	EXPECT_EQ(dq.Empty(), true);
	EXPECT_EQ(dq.Size(), 0);
	EXPECT_THROW(dq.PopFront(), std::exception);
}

TEST(Deque, SingleInsertionBack) {
	Deque<int> dq;

	/* Single insertion from back */
	dq.PushBack(23);
	EXPECT_EQ(dq.Empty(), false);
	EXPECT_EQ(dq.Size(), 1);
	EXPECT_EQ(dq[0], 23);
}

TEST(Deque, DoubleInsertionBack) {
	Deque<int> dq;

	/* Double insertion from back */
	dq.PushBack(23);
	dq.PushBack(42);
	EXPECT_EQ(dq.Empty(), false);
	EXPECT_EQ(dq.Size(), 2);
	EXPECT_EQ(dq[0], 23);
	EXPECT_EQ(dq[1], 42);
	EXPECT_EQ(dq.Back(), 42);
}

TEST(Deque, DoubleInsertionFront) {
	Deque<int> dq;

	/* Double insertion from front */
	dq.PushFront(23);
	dq.PushFront(5);
	EXPECT_EQ(dq.Empty(), false);
	EXPECT_EQ(dq.Size(), 2);
	EXPECT_EQ(dq[0], 5);
	EXPECT_EQ(dq[1], 23);
}

TEST(Deque, DoubleInsertionBackFront) {
	Deque<int> dq;

	/* Insertion from back and front */
	/* Tests edge case where front > back */
	dq.PushBack(23);
	dq.PushFront(42);
	EXPECT_EQ(dq.Empty(), false);
	EXPECT_EQ(dq.Size(), 2);
	EXPECT_EQ(dq[0], 42);
	EXPECT_EQ(dq[1], 23);
	EXPECT_EQ(dq.Back(), 23);
}

TEST(Deque, DoubleResize) {
	Deque<int> dq;

	/* Tests multiple pops and pushes */
	/* from both front and back */
	dq.PushBack(1);
	dq.PushBack(2);
	dq.PushBack(3);
	dq.PushBack(4);
	dq.PushBack(5);
	dq.PushBack(6);
	dq.PushBack(7);
	dq.PopFront();
	dq.PopFront();
	dq.PopBack();
	dq.PopBack();
	EXPECT_EQ(dq[0], 3);
	EXPECT_EQ(dq[1], 4);
	EXPECT_EQ(dq[2], 5);
	EXPECT_EQ(dq.Size(), 3);
}

TEST(Deque, ClearTest) {
	Deque<int> dq;

	/* Tests the clear function */
	dq.PushBack(1);
	dq.PushBack(2);
	dq.PushBack(3);
	dq.PushBack(4);
	dq.PopFront();
	dq.Clear();
	EXPECT_EQ(dq.Size(), 0);
}

TEST(Deque, ContiguousResize) {
	Deque<int> dq;

	/* Tests resize for all back > front */
	dq.PushBack(1);
	dq.PushBack(2);
	dq.PushBack(3);
	dq.PushBack(4);
	dq.PushBack(5);
	EXPECT_EQ(dq.Size(), 5);
	EXPECT_EQ(dq[0], 1);
	EXPECT_EQ(dq[1], 2);
	EXPECT_EQ(dq[2], 3);
	EXPECT_EQ(dq[3], 4);
	EXPECT_EQ(dq[4], 5);
}

TEST(Deque, NonContiguousResize) {
	Deque<int> dq;

	/* Tests resize for when front > back */
	dq.PushBack(1);
	dq.PushBack(2);
	dq.PushBack(3);
	dq.PushFront(4);
	dq.PushFront(5);
	EXPECT_EQ(dq[0], 5);
	EXPECT_EQ(dq[1], 4);
	EXPECT_EQ(dq[2], 1);
	EXPECT_EQ(dq[3], 2);
	EXPECT_EQ(dq[4], 3);
}

TEST(Deque, FrontBackTest) {
	Deque<int> dq;

	/* Tests Front() and Back() */
	dq.PushBack(1);
	dq.PushBack(2);
	dq.PushBack(3);
	dq.PushFront(4);
	dq.PushFront(5);
	EXPECT_EQ(dq[0], 5);
	EXPECT_EQ(dq[1], 4);
	EXPECT_EQ(dq[2], 1);
	EXPECT_EQ(dq[3], 2);
	EXPECT_EQ(dq[4], 3);
	EXPECT_EQ(dq.Front(), 5);
	EXPECT_EQ(dq.Back(), 3);
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

