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

#define COMPARISON CompareNode

//output for CompareNode
//abracadabra
//4
//abracadabra
//abba
//baba
//radar
//Creating new node for dc
//Creating new node for rdc
//Creating new node for brdc
//Creating new node for abrdc
//Setting prefix code for a to 0
//Setting prefix code for b to 10
//Setting prefix code for r to 110
//Setting prefix code for d to 1110
//Setting prefix code for c to 1111
//01011001111011100101100
//23
//010100
//6
//100100
//6
//110011100110
//12

//output for CompareNode2
//abracadabra
//4
//abracadabra
//abba
//baba
//radar
//Creating new node for cd
//Creating new node for cdb
//Creating new node for rcdb
//Creating new node for arcdb
//Setting prefix code for a to 0
//Setting prefix code for r to 10
//Setting prefix code for c to 1100
//Setting prefix code for d to 1101
//Setting prefix code for b to 111
//01111001100011010111100
//23
//01111110
//8
//11101110
//8
//1001101010
//10

//output for CompareNode3
//abracadabra
//4
//abracadabra
//abba
//baba
//radar
//Creating new node for cd
//Creating new node for cdb
//Creating new node for rcdb
//Creating new node for arcdb
//Setting prefix code for a to 0
//Setting prefix code for r to 10
//Setting prefix code for c to 1100
//Setting prefix code for d to 1101
//Setting prefix code for b to 111
//01111001100011010111100
//23
//01111110
//8
//11101110
//8
//1001101010
//10

//output for CompareNode4
//abracadabra
//4
//abracadabra
//abba
//baba
//radar
//Creating new node for dc
//Creating new node for rb
//Creating new node for dcrb
//Creating new node for adcrb
//Setting prefix code for a to 0
//Setting prefix code for d to 100
//Setting prefix code for c to 101
//Setting prefix code for r to 110
//Setting prefix code for b to 111
//01111100101010001111100
//23
//01111110
//8
//11101110
//8
//11001000110
//11

//output for CompareNode5
//abracadabra
//4
//abracadabra
//abba
//baba
//radar
//Creating new node for dc
//Creating new node for dcb
//Creating new node for rdcb
//Creating new node for ardcb
//Setting prefix code for a to 0
//Setting prefix code for r to 10
//Setting prefix code for d to 1100
//Setting prefix code for c to 1101
//Setting prefix code for b to 111
//01111001101011000111100
//23
//01111110
//8
//11101110
//8
//1001100010
//10

//As can be seen in the output above, the tie breaking strategy does affect the prefix code values assigned to a particular character.
//However, the tie breaking strategy does not affect the compression of the huffman string which is used to generate the tree.
//It does affect other strings encoded by the huffman tree.
//CompareNode does not take ties into account and produces the steepest tree. This results in very good compression for the most used characters in the huffman string,
//but results in poor compression for the lesser used characters, which probably results in more variance in encoded string length for all inputs.
//The opposite is true of CompareNode4, where depth is taken into account. This results in a more even compression.


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
    stringOfChars = std::string(&character, 1);
    numberOfReferences = 0;
    prefixCode = new std::string;
    leftNode = nullptr;
    rightNode = nullptr;
}


HuffmanNode::HuffmanNode(HuffmanNode *node1, HuffmanNode *node2) : isLeaf(false)
{
    
    character = '\0';
    
    stringOfChars = node1->getStringOfCharacters();
    stringOfChars.append(node2->getStringOfCharacters());
    
    //std::cout<< "Creating new node for " << stringOfChars <<std::endl;
    
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
    //std::cout<< "Setting prefix code for " << character << " to " <<*prefixCode<<std::endl;
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
        return leftNode->getNumberOfReferences() > rightNode->getNumberOfReferences();
    }
};

struct CompareNode2 {
    bool operator() (HuffmanNode* leftNode, HuffmanNode* rightNode) const
    {
        
        if (leftNode->getNumberOfReferences() == rightNode->getNumberOfReferences())
            return (leftNode->getStringOfCharacters()).compare(rightNode->getStringOfCharacters());
        
        return leftNode->getNumberOfReferences() > rightNode->getNumberOfReferences();
    }
};

struct CompareNode3 {
    bool operator() (HuffmanNode* leftNode, HuffmanNode* rightNode) const
    {
        
        if (leftNode->getNumberOfReferences() == rightNode->getNumberOfReferences())
            return (rightNode->getStringOfCharacters()).compare(leftNode->getStringOfCharacters());
        
        return leftNode->getNumberOfReferences() > rightNode->getNumberOfReferences();
    }
};

struct CompareNode4 {
    bool operator() (HuffmanNode* leftNode, HuffmanNode* rightNode) const
    {
        
        if (leftNode->getNumberOfReferences() == rightNode->getNumberOfReferences())
            return leftNode->getStringOfCharacters().length() > rightNode->getStringOfCharacters().length();
        
        return leftNode->getNumberOfReferences() > rightNode->getNumberOfReferences();
    }
};

struct CompareNode5 {
    bool operator() (HuffmanNode* leftNode, HuffmanNode* rightNode) const
    {
        
        if (leftNode->getNumberOfReferences() == rightNode->getNumberOfReferences())
            return leftNode->getStringOfCharacters().length() < rightNode->getStringOfCharacters().length();
        
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
    std::priority_queue<HuffmanNode*,std::vector<HuffmanNode*>, COMPARISON> *prioQueue;
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
    
    //this should run in O(n), where n is number of chars in huffmanString
    //each loop below is constant time
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
    
    prioQueue = new std::priority_queue<HuffmanNode*,std::vector<HuffmanNode*>, COMPARISON>;
    
    
    //nodeMap contain d entries, where d is number of distinct characters in huffmanString
    //the following runs in O(d log d)
    for (auto it = nodeMap->begin(); it != nodeMap->end(); ++it )
    {
        HuffmanNode *node = it->second;
        //add nodes to the priority queue
        prioQueue->push(node);
    }

    //we now have all our entries counted and added to min prio queue
    //Build our huffman tree
    //the following runs in O(d log d)
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

    //the following runs in O(d log d)
    setPrefixCodes();
}

Huffman::~Huffman()
{
    ::operator delete(nodeMap);
    ::operator delete(rootNode);
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
        //std::cout<<encodedString<<std::endl;
        std::cout<<encodedString.length()<<std::endl;
    }
    
    return 0;
}

