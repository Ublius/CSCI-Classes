/*
 * Author: C. Painter Wakefield
 *
 * Tests for the trie project.
 */

#include <vector>
#include <string>
#include <unordered_set>
#include <fstream>
#include <algorithm> // for shuffle()
#include <random> // RNGs

#include "gtest/gtest.h"
#include "trie.h"

using namespace std;

// fixture to load in dictionary and other word sets used in tests
class TrieTest: public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        // dictionary words
        ifstream fin("dictionary.txt");
        if (!fin) {
            FAIL() << "Error opening dictionary.txt, skipping all tests.";
            return;
        }

        while (fin) {
            string s;
            fin >> s;
            if (!s.empty()) {
                dictionary.push_back(s);
            }
        }
        fin.close();

        // put them in random order
        default_random_engine rng(1234);
        shuffle(dictionary.begin(), dictionary.end(), rng);
        
        // make a set
        unordered_set<string> dictionary_set;
        for (string s: dictionary) {
            dictionary_set.insert(s);
        }

        //create some non-words to test with
        while (non_words.size() < 50000) {
            string s = dictionary[rng() % dictionary.size()];
            char c = 'a' + (rng() % 26);
            s[rng() % s.size()] = c;
            if (dictionary_set.count(s) == 0) {
                non_words.push_back(s);
            }
        }
    }

    static void TearDownTestSuite() {
        dictionary.clear();
        non_words.clear();
    }

    static vector<string> dictionary;
    static vector<string> non_words;
};

vector<string> TrieTest::dictionary = vector<string>();
vector<string> TrieTest::non_words = vector<string>();

TEST_F(TrieTest, EmptyTest) {
    trie t;
    ASSERT_FALSE(t.contains("the"));
}

TEST_F(TrieTest, InsertShort) {
    trie t;
    t.insert("a");
    ASSERT_TRUE(t.contains("a"));
}

TEST_F(TrieTest, InsertOne) {
    trie t;
    t.insert("the");
    ASSERT_TRUE(t.contains("the"));
}

TEST_F(TrieTest, InsertTwoNoSharedPrefix) {
    trie t;
    t.insert("the");
    t.insert("and");
    ASSERT_TRUE(t.contains("the"));
    ASSERT_TRUE(t.contains("and"));
}

TEST_F(TrieTest, InsertPrefixThenExtension) {
    trie t;
    t.insert("fore");
    t.insert("forecast");
    ASSERT_TRUE(t.contains("fore"));
    ASSERT_TRUE(t.contains("forecast"));
}

TEST_F(TrieTest, InsertExtensionThenPrefix) {
    trie t;
    t.insert("forecast");
    t.insert("fore");
    ASSERT_TRUE(t.contains("fore"));
    ASSERT_TRUE(t.contains("forecast"));
}

TEST_F(TrieTest, TestPrefixNotWord) {
    trie t;
    t.insert("test");
    ASSERT_TRUE(t.contains("test"));
    ASSERT_FALSE(t.contains("t"));
    ASSERT_FALSE(t.contains("te"));
    ASSERT_FALSE(t.contains("tes"));
}

TEST_F(TrieTest, TestExtensionNotWord) {
    trie t;
    t.insert("fore");
    t.insert("forecaster");
    ASSERT_TRUE(t.contains("fore"));
    ASSERT_TRUE(t.contains("forecaster"));
    ASSERT_FALSE(t.contains("forecast"));
    ASSERT_FALSE(t.contains("forecasters"));
}

TEST_F(TrieTest, Prefix) {
    trie t;
    t.insert("test");
    ASSERT_TRUE(t.contains("test"));
    ASSERT_TRUE(t.is_prefix(""));
    ASSERT_TRUE(t.is_prefix("t"));
    ASSERT_TRUE(t.is_prefix("te"));
    ASSERT_TRUE(t.is_prefix("tes"));
}

TEST_F(TrieTest, ExtendEmptyResult) {
    trie t;
    vector<string> v = { "forecast", "forecaster", "forecasters", "forecasts" };
    for (string &s: v) {
        t.insert(s);
    }

    vector<string> w;
    t.extend("arglebargle", w);
    ASSERT_TRUE(w.empty());
}

TEST_F(TrieTest, ExtendAll) {
    trie t;
    vector<string> v = { "forecast", "forecaster", "forecasters", "forecasts" };
    for (string &s: v) {
        t.insert(s);
    }

    vector<string> w;
    t.extend("", w);
    ASSERT_EQ(w, v);

    w.clear();
    t.extend("fo", w);
    ASSERT_EQ(w, v);

    w.clear();
    t.extend("forecast", w);
    ASSERT_EQ(w, v);
}

TEST_F(TrieTest, ExtendSome) {
    trie t;
    vector<string> v = { "different", "fore", "forecast", "forecaster", "forecasters", "forecasts" };
    for (string &s: v) {
        t.insert(s);
    }
    vector<string> expected = { "forecast", "forecaster", "forecasters", "forecasts" };

    vector<string> w;
    t.extend("foreca", w);
    ASSERT_EQ(w, expected);

    w.clear();
    t.extend("forecast", w);
    ASSERT_EQ(w, expected);
}

TEST_F(TrieTest, LargeTestEverything) {
    trie t;
    for (const string &s: dictionary) {
        t.insert(s);
    }
    for (const string &s: dictionary) {
        ASSERT_TRUE(t.contains(s));
    }
    for (const string &s: non_words) {
        ASSERT_FALSE(t.contains(s));
    }
    vector<string> w;

    t.extend("arglebargle", w);
    ASSERT_TRUE(w.empty());

    t.extend("ptero", w);
    vector<string> expected = { "pterodactyl", "pterodactyls", "pteropod", "pteropods", "pterosaur", "pterosaurs" };
    ASSERT_EQ(w, expected);

    w.clear();
    t.extend("snooze", w);
    vector<string> expected2 = { "snooze", "snoozed", "snoozer", "snoozers", "snoozes" };
    ASSERT_EQ(w, expected2);
}
