/*
 * trie.h
 *
 * Declaration of the trie class.
 * 
 * Author: Tristen Erpelding
 */

#ifndef _TRIE_H
#define _TRIE_H

#include <string>
#include <vector>

using namespace std;

class trie {

private:
    class Node {
        public:
            Node() : isTerminal(false), children{} {}
            bool isTerminal;
            Node* children[26];
    };

    Node root;

public:
    void insert(const string &s);
    bool contains(const string &s);
    bool is_prefix(const string &s);
    void extend(const string &prefix, vector<string> &result);
    void extendHelper(Node* &root, string prefix, vector<string> &result);
};

#endif
