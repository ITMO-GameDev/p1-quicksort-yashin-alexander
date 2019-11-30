#include "gtest/gtest.h"
#include "dictonary.hpp"


template <typename K, typename V>
void assert_single_value_insertion(Dictonary<K, V> &dict, K key, V value)
{
    dict.put(key, value);
    EXPECT_TRUE(dict.contains(key));
    EXPECT_EQ(dict[key], value);
}


template <typename K, typename V>
void assert_single_value_removal(Dictonary<K, V> &dict, K key)
{
    dict.remove(key);
    EXPECT_FALSE(dict.contains(key));
}

TEST(DictTest, TestSingleIsnertion){
    Dictonary<std::string, std::string> dict;

    assert_single_value_insertion(dict, (std::string)"wow", (std::string)"is it working?");
    EXPECT_EQ(dict.size(), 1);
}


TEST(DictTest, TestIsnertion){
    Dictonary<std::string, int> dict;
    std::string key;

    for (int i = 0; i < 20; i++)
    {
        key = "test string number " + std::to_string(i);
        assert_single_value_insertion(dict, key, i);
    }
    EXPECT_EQ(dict.size(), 20);
}


TEST(DictTest, TestOverwrite){
    Dictonary<std::string, int> dict;
    std::string key;

    for (int i = 0; i < 20; i++)
    {
        key = "test string number " + std::to_string(i);
        assert_single_value_insertion(dict, key, i);
    }
    EXPECT_EQ(dict.size(), 20);
    for (int i = 20; i > 0; i--)
    {
        key = "another string number " + std::to_string(i);
        assert_single_value_insertion(dict, key, i);
    }
    EXPECT_EQ(dict.size(), 40);
}


TEST(DictTest, TestRemove){
    Dictonary<std::string, int> dict;
    std::string key = "wow";

    assert_single_value_insertion(dict, key, 22);
    EXPECT_EQ(dict.size(), 1);
    assert_single_value_removal(dict, key);
    EXPECT_EQ(dict.size(), 0);
}


TEST(DictTest, TestInsertionAfterRemove){
    Dictonary<std::string, int> dict;
    std::string key;

    for (int i = 0; i < 20; i++)
    {
        key = "test string number " + std::to_string(i);
        assert_single_value_insertion(dict, key, i);
    }
    EXPECT_EQ(dict.size(), 20);
    for (int i = 0; i < 20; i++)
    {
        key = "test string number " + std::to_string(i);
        assert_single_value_removal(dict, key);
    }
    EXPECT_EQ(dict.size(), 0);
}


TEST(DictTest, TestBeginIterator){
    Dictonary<int, int> dict;
    std::string key;

    dict.put(22, 44);
    dict.put(23, 2);
    dict.put(0, -1);
    dict.put(1, 2);
    dict.put(-5, 44);

    auto iter = dict.begin();
    iter.next();
    EXPECT_EQ(iter.key(), -5);
    EXPECT_EQ(iter.get(), 44);
    iter.next();
    EXPECT_EQ(iter.key(), 0);
    EXPECT_EQ(iter.get(), -1);
    iter.next();
    EXPECT_EQ(iter.key(), 1);
    EXPECT_EQ(iter.get(), 2);
    iter.next();
    EXPECT_EQ(iter.key(), 22);
    EXPECT_EQ(iter.get(), 44);
    iter.next();
    EXPECT_EQ(iter.key(), 23);
    EXPECT_EQ(iter.get(), 2);
}


TEST(DictTest, TestEndIterator){
    Dictonary<int, int> dict;
    std::string key;

    dict.put(14, 1234);
    dict.put(100, 55);
    dict.put(0, -12);
    dict.put(1, 2);
    dict.put(-35, 4);

    auto iter = dict.end();
    EXPECT_TRUE(iter.hasPrev());
    EXPECT_FALSE(iter.hasNext());
    iter.prev();
    EXPECT_EQ(iter.key(), 100);
    EXPECT_EQ(iter.get(), 55);
    EXPECT_TRUE(iter.hasPrev());
    iter.prev();
    EXPECT_EQ(iter.key(), 14);
    EXPECT_EQ(iter.get(), 1234);
    EXPECT_TRUE(iter.hasPrev());
    iter.prev();
    EXPECT_EQ(iter.key(), 1);
    EXPECT_EQ(iter.get(), 2);
    EXPECT_TRUE(iter.hasPrev());
    iter.prev();
    EXPECT_EQ(iter.key(), 0);
    EXPECT_EQ(iter.get(), -12);
    EXPECT_TRUE(iter.hasPrev());
    iter.prev();
    EXPECT_EQ(iter.key(), -35);
    EXPECT_EQ(iter.get(), 4);

    EXPECT_FALSE(iter.hasPrev());
    EXPECT_TRUE(iter.hasNext());
}


TEST(DictTest, TestBidirectionalIterator){
    Dictonary<int, int> dict;
    std::string key;

    dict.put(22, 44);
    dict.put(23, 2);
    dict.put(0, -1);
    dict.put(1, 2);
    dict.put(-5, 44);
//    std::cout << dict.pformat() << '\n';

    auto iter = dict.end();

    EXPECT_TRUE(iter.hasPrev());
    EXPECT_FALSE(iter.hasNext());
    iter.prev();
    EXPECT_EQ(iter.key(), 23);
    EXPECT_EQ(iter.get(), 2);
    iter.prev();
    EXPECT_EQ(iter.key(), 22);
    EXPECT_EQ(iter.get(), 44);
    iter.prev();
    EXPECT_EQ(iter.key(), 1);
    EXPECT_EQ(iter.get(), 2);
    iter.prev();
    EXPECT_EQ(iter.key(), 0);
    EXPECT_EQ(iter.get(), -1);
    // go forward
    iter.next();
    EXPECT_EQ(iter.key(), 1);
    EXPECT_EQ(iter.get(), 2);
    iter.next();
    EXPECT_EQ(iter.key(), 22);
    EXPECT_EQ(iter.get(), 44);
    iter.next();
    EXPECT_EQ(iter.key(), 23);
    EXPECT_EQ(iter.get(), 2);
    // go backward
    iter.prev();
    EXPECT_EQ(iter.key(), 22);
    EXPECT_EQ(iter.get(), 44);
    iter.prev();
    EXPECT_EQ(iter.key(), 1);
    EXPECT_EQ(iter.get(), 2);
    iter.prev();
    EXPECT_EQ(iter.key(), 0);
    EXPECT_EQ(iter.get(), -1);
    iter.prev();
    EXPECT_EQ(iter.key(), -5);
    EXPECT_EQ(iter.get(), 44);

    EXPECT_FALSE(iter.hasPrev());
    EXPECT_TRUE(iter.hasNext());
}


TEST(DictTest, TestIteratorSet){
    Dictonary<std::string, int> dict;
    std::string key;


    dict.put((std::string)"String", 5);
    dict.put((std::string)"Second String", 100);
    auto iter = dict.begin();

    iter.next();
    iter.set(555);
    EXPECT_EQ(dict[(std::string)"Second String"], 555);

    iter.next();
    iter.set(322);
    EXPECT_EQ(dict[(std::string)"String"], 322);
}

