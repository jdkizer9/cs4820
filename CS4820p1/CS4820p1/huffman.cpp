//
//  input.cpp
//  CS4820p1
//
//  Created by James Kizer on 6/5/14.
//  Copyright (c) 2014 JimmyTime Software. All rights reserved.
//

#include <iostream>
#include <queue>
#include <unordered_map>
#include <assert.h>

class HuffmanNode
{
public:
    
    HuffmanNode(char c);
    HuffmanNode(HuffmanNode *node1, HuffmanNode *node2);
    
    ~HuffmanNode();
    
    int getNumberOfReferences();
    void setNumberOfReferences(int);
    
    char getCharacter ();
    void setCharacter(char c);
    
    std::string &getStringOfCharacters();
    bool isLeafNode();
    
    std::string &getPrefixCode();
    void setPrefixCode(std::string &code);
    
    HuffmanNode *getLeftNode();
    HuffmanNode *getRightNode();

private:
    char character;
    std::string stringOfChars;
    int numberOfReferences;
    
    HuffmanNode *leftNode;
    HuffmanNode *rightNode;
    
    std::string *prefixCode;
    
    bool isLeaf;
};

HuffmanNode::HuffmanNode(char c) : isLeaf(true)
{
    character = c;
    numberOfReferences = 0;
    prefixCode = new std::string;
    leftNode = nullptr;
    rightNode = nullptr;
}


HuffmanNode::HuffmanNode(HuffmanNode *node1, HuffmanNode *node2) : isLeaf(false)
{
    
    character = '\0';
    
    std::string newNodeStringOfChars;
    if(node1->isLeafNode())
    {
        char c = node1->getCharacter();
        newNodeStringOfChars.append(&c, 1);
    }
    else
    {
        newNodeStringOfChars.append(node1->getStringOfCharacters());
    }
    
    if(node2->isLeafNode())
    {
        char c = node2->getCharacter();
        newNodeStringOfChars.append(&c, 1);
    }
    else
    {
        newNodeStringOfChars.append(node2->getStringOfCharacters());
    }
    stringOfChars = newNodeStringOfChars;
    
    numberOfReferences = node1->getNumberOfReferences() + node2->getNumberOfReferences();
    
    leftNode = node1;
    rightNode = node2;
}



HuffmanNode::~HuffmanNode()
{
    ::operator delete(prefixCode);
    
    //recursively delete tree
    if(leftNode)
        ::operator delete(leftNode);
    if(rightNode)
        ::operator delete(rightNode);
}

int HuffmanNode::getNumberOfReferences()
{
    return numberOfReferences;
}

void HuffmanNode::setNumberOfReferences(int numRefs)
{
    numberOfReferences = numRefs;
}

char HuffmanNode::getCharacter()
{
    return character;
}

void HuffmanNode::setCharacter(char c)
{
    character = c;
}

bool HuffmanNode::isLeafNode()
{
    return isLeaf;
}

std::string &HuffmanNode::getStringOfCharacters()
{
    return stringOfChars;
}

std::string &HuffmanNode::getPrefixCode()
{
    return *prefixCode;
}

void HuffmanNode::setPrefixCode(std::string &code)
{
    *prefixCode = code;
}

HuffmanNode *HuffmanNode::getLeftNode()
{
    return leftNode;
}

HuffmanNode *HuffmanNode::getRightNode()
{
    return rightNode;
}

struct CompareNode {
    bool operator() (HuffmanNode* leftNode, HuffmanNode* rightNode) const
    {
        // return "true" if "p1" is ordered before "p2", for example:
        return leftNode->getNumberOfReferences() > rightNode->getNumberOfReferences();
    }
};

class Huffman
{
public:
    Huffman(std::string &);
    ~Huffman();
    void encode(std::string &, std::string &);
    std::string &prefixCodeForChar(char c);
    
private:
    void setPrefixCode(HuffmanNode*, std::string &);
    void setPrefixCodes();
    std::unordered_map<char, HuffmanNode*> *nodeMap;
    std::priority_queue<HuffmanNode*,std::vector<HuffmanNode*>, CompareNode> *prioQueue;
    HuffmanNode *rootNode;
};

void Huffman::setPrefixCode(HuffmanNode *node, std::string &code)
{
    if(node->isLeafNode())
    {
        node->setPrefixCode(code);
        return;
    }
    std::string *newCode = new std::string(code);
    (*newCode)+="0";
    assert(node->getLeftNode() != nullptr);
    setPrefixCode(node->getLeftNode(), (*newCode));
    (*newCode) = code;
    (*newCode)+="1";
    assert(node->getRightNode() != nullptr);
    setPrefixCode(node->getRightNode(), (*newCode));
    
    ::operator delete(newCode);
}

void Huffman::setPrefixCodes()
{
    assert(rootNode != nullptr);
    std::string rootString;
    setPrefixCode(rootNode, rootString);
}

Huffman::Huffman(std::string &huffmanString)
{
    nodeMap = new std::unordered_map<char, HuffmanNode*>;
    
    //iterate throught the characters in the huffmanString
    for(std::string::iterator it = huffmanString.begin(); it != huffmanString.end(); ++it)
    {
        char c = *it;
        HuffmanNode* huffmanNode = (*nodeMap)[c];
        if (huffmanNode == nullptr)
        {
            //if there is no node in the map for this character, instantiate one
            huffmanNode = new HuffmanNode(c);
            (*nodeMap)[c] = huffmanNode;
        }
        
        int characterReferences = huffmanNode->getNumberOfReferences();
        huffmanNode->setNumberOfReferences(++characterReferences);
    }
    
    prioQueue = new std::priority_queue<HuffmanNode*,std::vector<HuffmanNode*>, CompareNode>;
    
    for (auto it = nodeMap->begin(); it != nodeMap->end(); ++it )
    {
        HuffmanNode *node = it->second;
        //add nodes to the priority queue
        prioQueue->push(node);
    }

    //we now have all our entries counted and added to min prio queue
    //Build our huffman tree
    while (prioQueue->size() > 2)
    {
        size_t originalSize = prioQueue->size();
        HuffmanNode *node1 = prioQueue->top();
        prioQueue->pop();
        HuffmanNode *node2 = prioQueue->top();
        
        prioQueue->pop();
        
        assert(prioQueue->size() == (originalSize - 2));
        
        //create a new node for these two nodes
        HuffmanNode *newNode = new HuffmanNode(node1, node2);
        
        prioQueue->push(newNode);
        
        assert(prioQueue->size() == (originalSize - 1));
        
    }
    
    HuffmanNode *node1 = prioQueue->top();
    prioQueue->pop();
    HuffmanNode *node2 = prioQueue->top();
    prioQueue->pop();
    assert(prioQueue->size() == 0);
    
    rootNode = new HuffmanNode(node1, node2);
    
    //we now have a proper huffman tree
    //search the tree for leaves and populate prefix codes

    setPrefixCodes();
}

Huffman::~Huffman()
{
    ::operator delete(nodeMap);
}

std::string &Huffman::prefixCodeForChar(char c)
{
    HuffmanNode* huffmanNode = (*nodeMap)[c];
    return huffmanNode->getPrefixCode();
}

void Huffman::encode(std::string &stringToEncode, std::string &encodedString)
{
    //for each letter in stringToEncode, do a lookup in nodeMap (prefixCodeForChar)
    encodedString = std::string();
    for(std::string::iterator it = stringToEncode.begin(); it != stringToEncode.end(); ++it)
    {
        char c = *it;
        encodedString.append(prefixCodeForChar(c));
    }
}


int main(int argc, const char * argv[])
{
    std::string huffmanEncodingString;
    int N;
    std::string stringToEncode;
    std::deque<std::string> stringsToEncode;
    
    std::cin >> huffmanEncodingString;
    std::cin >> N;
    
    for (int i=0; i<N; i++)
    {
        std::string stringToEncode;
        std::cin >> stringToEncode;
        stringsToEncode.push_back (stringToEncode);
    }
    
    Huffman h(huffmanEncodingString);
    
    auto it = stringsToEncode.begin();
    int i = 0;
    while (it != stringsToEncode.end())
    {
        i++;
        std::string encodedString;
        h.encode(*it++, encodedString);
        std::cout<<encodedString.length()<<std::endl;
    }
    
    return 0;
}

