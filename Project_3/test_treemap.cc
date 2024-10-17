#include <gtest/gtest.h>
#include <string>

#include "treemap.h"

TEST(Treemap, Empty) {
  Treemap<int, int> map;

  /* Should be fully empty */
  EXPECT_EQ(map.Empty(), true);
  EXPECT_EQ(map.Size(), 0);
  EXPECT_THROW(map.Get(42), std::exception);
}

TEST(Treemap, OneKey) {
  Treemap<int, char> map;

  /* Test some insertion */
  map.Insert(23, 'A');
  map.Insert(42, 'B');
  EXPECT_EQ(map.Empty(), false);
  EXPECT_EQ(map.Size(), 2);
  EXPECT_EQ(map.MinKey(), 23);
  EXPECT_EQ(map.MaxKey(), 42);
  EXPECT_EQ(map.Get(23), 'A');
  // Insert throws exception if key already exists
  EXPECT_THROW(map.Insert(23, 'A'), std::exception);
}

TEST(Treemap, ContainsKey) {
  Treemap<int, int> map;

  /* Checks Contains Key Function */
  map.Insert(23, 'A');
  map.Insert(42, 'B');
  map.Insert(5, 'C');
  map.Insert(8, 'D');
  EXPECT_EQ(map.Empty(), false);
  EXPECT_EQ(map.Size(), 4);
  EXPECT_EQ(map.MinKey(), 5);
  EXPECT_EQ(map.MaxKey(), 42);
  EXPECT_EQ(map.Get(23), 'A');
  EXPECT_EQ(map.ContainsKey(8), true);
  EXPECT_EQ(map.ContainsKey(37), false);
}

TEST(Treemap, ContainsValue) {
  Treemap<int, char> map;

  /* Checks Contains Value Function */
  map.Insert(23, 'A');
  map.Insert(42, 'B');
  map.Insert(5, 'C');
  map.Insert(8, 'D');
  EXPECT_EQ(map.Empty(), false);
  EXPECT_EQ(map.Size(), 4);
  EXPECT_EQ(map.MinKey(), 5);
  EXPECT_EQ(map.MaxKey(), 42);
  EXPECT_EQ(map.Get(23), 'A');
  EXPECT_EQ(map.ContainsValue('C'), true);
  EXPECT_EQ(map.ContainsValue('E'), false);
}

TEST(Treemap, GetFloor) {
  Treemap<int, int> map;

  /* Checks GetFloor Function */
  map.Insert(23, 'A');
  map.Insert(42, 'B');
  map.Insert(5, 'C');
  map.Insert(8, 'D');
  EXPECT_EQ(map.Empty(), false);
  EXPECT_EQ(map.Size(), 4);
  EXPECT_EQ(map.MinKey(), 5);
  EXPECT_EQ(map.MaxKey(), 42);
  EXPECT_EQ(map.FloorKey(8), 8);
  EXPECT_EQ(map.FloorKey(7), 5);
  // FloorKey throws exception if no floor exists for key
  EXPECT_THROW(map.FloorKey(4), std::exception);
  map.Remove(23);
  map.Remove(42);
  map.Remove(5);
  map.Remove(8);
  EXPECT_EQ(map.Size(), 0);
  EXPECT_EQ(map.Empty(), true);
  // FloorKey throws exception if tree is empty
  EXPECT_THROW(map.CeilKey(43), std::exception);
}
// Throws exception if tree is empty or no ceil exists for key
TEST(Treemap, GetCeiling) {
  Treemap<int, int> map;

  /* Checks GetCeil Function */
  map.Insert(23, 'A');
  map.Insert(42, 'B');
  map.Insert(5, 'C');
  map.Insert(8, 'D');
  EXPECT_EQ(map.Empty(), false);
  EXPECT_EQ(map.Size(), 4);
  EXPECT_EQ(map.MinKey(), 5);
  EXPECT_EQ(map.MaxKey(), 42);
  EXPECT_EQ(map.CeilKey(23), 23);
  EXPECT_EQ(map.CeilKey(28), 42);
  // CeilKey throws exception if no ceiling exists for key
  EXPECT_THROW(map.CeilKey(43), std::exception);
  map.Remove(23);
  map.Remove(42);
  map.Remove(5);
  map.Remove(8);
  EXPECT_EQ(map.Size(), 0);
  EXPECT_EQ(map.Empty(), true);
  // CeilKey throws exception if tree is empty
  EXPECT_THROW(map.CeilKey(43), std::exception);
}
TEST(Treemap, MegaTree) {
  Treemap<int, int> map;

  /* Tests a mega tree */
  map.Insert(23, 'A');
  map.Insert(42, 'B');
  map.Insert(5, 'C');
  map.Insert(8, 'D');
  map.Insert(2, 'E');
  map.Insert(59, 'F');
  map.Insert(73, 'G');
  map.Insert(67, 'H');
  EXPECT_EQ(map.Empty(), false);
  EXPECT_EQ(map.Size(), 8);
  EXPECT_EQ(map.MinKey(), 2);
  EXPECT_EQ(map.MaxKey(), 73);
  EXPECT_EQ(map.FloorKey(8), 8);
  EXPECT_EQ(map.FloorKey(7), 5);
  EXPECT_EQ(map.CeilKey(23), 23);
  EXPECT_EQ(map.CeilKey(43), 59);
  EXPECT_THROW(map.FloorKey(1), std::exception);
  EXPECT_THROW(map.CeilKey(74), std::exception);
}

TEST(Treemap, GetRemoveTest) {
  Treemap<int, char> map;

  /* Tests Get and Remove Functions */
  map.Insert(23, 'A');
  map.Insert(42, 'B');
  // Get throws exception if key doesn't exist
  EXPECT_EQ(map.Get(23), 'A');
  EXPECT_THROW(map.Get(24), std::exception);
  map.Remove(23);
  map.Remove(42);
  EXPECT_EQ(map.Empty(), true);
  // Remove throws exception if key doesn't exist
  EXPECT_THROW(map.Remove(23), std::exception);
  // Get throws exception if tree is empty
  EXPECT_THROW(map.Get(23), std::exception);
}
TEST(Treemap, MaxMinEmptyTest) {
  Treemap<int, char> map;

  /* Checks Max and Min Error Handling */
  EXPECT_EQ(map.Empty(), true);
  EXPECT_THROW(map.MaxKey(), std::exception);
  EXPECT_THROW(map.MinKey(), std::exception);
}
TEST(Treemap, Donor20Test) {
    Treemap<int, std::string> map;

    /* Tests a mega tree */
    map.Insert(18000, "Shima Masatoshi");
    map.Insert(19000, "Sergei Alekseyevich Lebedev");
    map.Insert(17000, "Joseph Marie Jacquard");
    map.Insert(9000, "Konrad Zuse");
    map.Insert(11000, "Douglas Engelbart");
    map.Insert(20000, "Stephen Cook");
    map.Insert(5000, "Wesley A.Clark");
    map.Insert(2000, "Donald Knuth");
    map.Insert(16000, "Allen Newell");
    map.Insert(14000, "Peter Rozsa");
    map.Insert(6000, "Kurt Godel");
    map.Insert(8000, "Marvin Minsky");
    map.Insert(4000, "Nakamatsu Yoshiro");
    map.Insert(1000, "Charles Babbage");
    map.Insert(3000, "Frances E.Allen");
    map.Insert(12000, "Sasaki Tadashi");
    map.Insert(13000, "John Mauchly");
    map.Insert(15000, "Barbara Grosz");
    map.Insert(7000, "Saul Rosen");
    map.Insert(10000, "Edsger Dijkstra");
    EXPECT_EQ(map.CeilKey(999), 1000);
    EXPECT_EQ(map.CeilKey(1001), 2000);
    EXPECT_EQ(map.CeilKey(2001), 3000);
    EXPECT_EQ(map.CeilKey(3001), 4000);
    EXPECT_EQ(map.FloorKey(1001), 1000);
    EXPECT_EQ(map.FloorKey(2001), 2000);
    EXPECT_EQ(map.FloorKey(3001), 3000);
    map.Remove(20000);
    EXPECT_EQ(map.Size(), 19);
    map.Remove(6000);
    EXPECT_EQ(map.Size(), 18);
    map.Remove(3000);
    EXPECT_EQ(map.Size(), 17);
    map.Remove(7000);
    EXPECT_EQ(map.Size(), 16);
}
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

