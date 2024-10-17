#include <gtest/gtest.h>

#include <functional>
#include "pqueue.h"

// Raj Garimella
// 919351073
// Bradley Manzo
// 916953788
// Programming Assignment #4
// Prof. Joel Porquet-Lupine
// May 20th, 2022

TEST(PQueue, less) {
    PQueue<int> pq;

    // tests with a less than comparator
    EXPECT_THROW(pq.Pop(), std::exception);
    EXPECT_THROW(pq.Top(), std::exception);
    pq.Push(42);
    pq.Push(23);
    pq.Push(2);
    pq.Push(34);

    EXPECT_EQ(pq.Top(), 2);
    EXPECT_EQ(pq.Size(), 4);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 23);
}

TEST(PQueue, great) {
    PQueue<int, std::greater<int>> pq;

    // tests with a greater than comparator
    EXPECT_THROW(pq.Pop(), std::exception);
    EXPECT_THROW(pq.Top(), std::exception);
    pq.Push(42);
    pq.Push(23);
    pq.Push(2);
    pq.Push(34);

    EXPECT_EQ(pq.Top(), 42);
    EXPECT_EQ(pq.Size(), 4);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 34);
}

class MyClass {
 public:
    explicit MyClass(int n) : n_(n) {}
    bool operator < (const MyClass& mc) const { return n_ < mc.n_; }
    int n() { return n_; }
 private:
    int n_;
};

TEST(PQueue, custom_class) {
    std::vector<MyClass> vec{ MyClass(42), MyClass(23),
        MyClass(2), MyClass(34) };

    // tests with a custom class comparator
    PQueue<MyClass> pq;
    EXPECT_THROW(pq.Pop(), std::exception);
    EXPECT_THROW(pq.Top(), std::exception);

    pq.Push(vec[0]);
    pq.Push(vec[1]);
    pq.Push(vec[2]);
    pq.Push(vec[3]);

    EXPECT_EQ(pq.Top().n(), vec[2].n());
    EXPECT_EQ(pq.Size(), 4);
    pq.Pop();
    EXPECT_EQ(pq.Top().n(), vec[1].n());
}



class MyClassPointerLess {
 public:
    bool operator()(MyClass* mc1, MyClass* mc2) const {
        return (*mc1 < *mc2);
    }
};

class MyClassPointerGreater {
 public:
    bool operator()(MyClass* mc1, MyClass* mc2) const {
        return !(*mc1 < *mc2);
    }
};


TEST(PQueue, CustomClassPointer) {
    std::vector<MyClass*> vec{ new MyClass(42), new MyClass(23),
                              new MyClass(2), new MyClass(34) };

    // tests with a less than comparator that works for pointers
    PQueue<MyClass*, MyClassPointerLess> pq;

    EXPECT_THROW(pq.Pop(), std::exception);
    EXPECT_THROW(pq.Top(), std::exception);

    pq.Push(vec[0]);
    pq.Push(vec[1]);
    pq.Push(vec[2]);
    pq.Push(vec[3]);

    EXPECT_EQ(pq.Top(), vec[2]);
    EXPECT_EQ(pq.Size(), 4);
    pq.Pop();
    EXPECT_EQ(pq.Top(), vec[1]);
}


TEST(PQueue, CustomClassPointerMaxQueue) {
    std::vector<MyClass*> vec{ new MyClass(5), new MyClass(15),
                              new MyClass(0), new MyClass(10),
                              new MyClass(25), new MyClass(-100) };

    // tests with a greater than comparator that works for pointers
    PQueue<MyClass*, MyClassPointerGreater> pq;
    pq.Push(vec[0]);
    pq.Push(vec[1]);
    pq.Push(vec[2]);
    pq.Push(vec[3]);
    pq.Push(vec[4]);
    pq.Push(vec[5]);

    EXPECT_EQ(pq.Top(), vec[4]);
    EXPECT_EQ(pq.Size(), 6);
    pq.Pop();
    EXPECT_EQ(pq.Top(), vec[1]);
    EXPECT_EQ(pq.Size(), 5);
}

TEST(PQueue, push_pop_all) {
    PQueue<int, std::greater<int>> pq;

    // tests a long series of consecutive pushes and pops

    pq.Push(42);
    pq.Push(23);
    pq.Push(2);
    pq.Push(34);
    pq.Push(41);
    pq.Push(5);
    pq.Push(7);
    pq.Push(78);
    pq.Push(90);
    pq.Push(30);
    pq.Push(67);
    pq.Push(25);

    EXPECT_EQ(pq.Top(), 90);
    EXPECT_EQ(pq.Size(), 12);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 78);
    EXPECT_EQ(pq.Size(), 11);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 67);
    EXPECT_EQ(pq.Size(), 10);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 42);
    EXPECT_EQ(pq.Size(), 9);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 41);
    EXPECT_EQ(pq.Size(), 8);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 34);
    EXPECT_EQ(pq.Size(), 7);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 30);
    EXPECT_EQ(pq.Size(), 6);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 25);
    EXPECT_EQ(pq.Size(), 5);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 23);
    EXPECT_EQ(pq.Size(), 4);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 7);
    EXPECT_EQ(pq.Size(), 3);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 5);
    EXPECT_EQ(pq.Size(), 2);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 2);
    EXPECT_EQ(pq.Size(), 1);
    pq.Pop();
    EXPECT_EQ(pq.Size(), 0);
    EXPECT_THROW(pq.Pop(), std::exception);
    EXPECT_THROW(pq.Top(), std::exception);
}

TEST(PQueue, push_pop_push_pop) {
    PQueue<int, std::greater<int>> pq;

    // tests a long series of interleaven pushes and pops

    pq.Push(42);
    pq.Push(23);
    pq.Push(2);
    pq.Push(34);
    EXPECT_EQ(pq.Top(), 42);
    EXPECT_EQ(pq.Size(), 4);
    pq.Pop();
    pq.Pop();
    EXPECT_EQ(pq.Top(), 23);
    pq.Push(50);
    pq.Push(5);
    pq.Push(7);
    pq.Push(1000);
    pq.Push(6);


    EXPECT_EQ(pq.Top(), 1000);
    EXPECT_EQ(pq.Size(), 7);
    pq.Pop();
    pq.Pop();
    pq.Pop();
    EXPECT_EQ(pq.Top(), 7);
    EXPECT_EQ(pq.Size(), 4);
}


int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
