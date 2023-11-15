/*
 * trie.cpp
 *
 * Method definitions for the trie class.
 *
 * Author: Tristen Erpelding
 */

#include "trie.h"
#include <stack>

void trie::insert(const string &s)
{
    Node* tempNode = &root;

    for(char c : s)
    {
        int index = c - 'a';
        if(!tempNode->children[index])
            tempNode->children[index] = new Node;
        tempNode = tempNode->children[index];
    }

    tempNode->isTerminal = true;
}

bool trie::contains(const string &s)
{
    Node* tempNode = &root;

    for(char c : s) 
    {
        int index = c - 'a';
        if(!tempNode->children[index])
            return false;
        tempNode = tempNode->children[index];
    }

    if(tempNode->isTerminal)
        return true;
    return false;
}

bool trie::is_prefix(const string &s)
{
    Node* tempNode = &root;

    for(char c : s)
    {
        int index = c - 'a';
        if(!tempNode->children[index])
            return false;
        tempNode = tempNode->children[index];
    }

    return true;
}

void trie::extend(const string &prefix, vector<string> &result)
{
    Node* tempNode = &root;

    for(char c : prefix)
    {
        int index = c - 'a';
        if(!tempNode->children[index])
            return;
        tempNode = tempNode->children[index];
    }

    if(tempNode->isTerminal)
        result.push_back(prefix);

    extendHelper(tempNode, prefix, result);
}

void trie::extendHelper(Node* &root, string prefix, vector<string> &result)
{
    if(root == nullptr)
        return;
    
    for(unsigned int i=0; i < 26; i++) 
    {
        if (root->children[i]!=nullptr && root->children[i]->isTerminal)
            result.push_back(prefix+char('a' + i));
        // append the char to the prefix and continue recursively
        extendHelper(root->children[i], prefix + char('a' + i), result);
    }
}

