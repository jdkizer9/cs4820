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
    //HuffmanNode(std::string &str, int numRefs);
    HuffmanNode(HuffmanNode *node1, HuffmanNode *node2);
    //HuffmanNode();
//    // Copy constructor.
//    HuffmanNode(const HuffmanNode &);
//    // Assignment.
//    HuffmanNode &operator=(const HuffmanNode &);
    
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


//HuffmanNode::HuffmanNode()
//{
//    character = '\0';
//    numberOfReferences = 0;
//    prefixCode = nullptr;
//    leftNode = nullptr;
//    rightNode = nullptr;
//}

HuffmanNode::HuffmanNode(char c) : isLeaf(true)
{
    character = c;
    numberOfReferences = 0;
    prefixCode = new std::string;
    leftNode = nullptr;
    rightNode = nullptr;
}

//HuffmanNode::HuffmanNode(std::string &str, int numRefs) : isLeaf(false), numberOfReferences(numRefs)
//{
//    character = '\0';
//    stringOfChars = str;
//    numberOfReferences = 0;
//    prefixCode = nullptr;
//    leftNode = nullptr;
//    rightNode = nullptr;
//}


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

//class mycomparison
//{
//    bool reverse;
//public:
//    mycomparison(const bool& revparam=false)
//    {reverse=revparam;}
//    bool operator() (const HuffmanNode& leftNode, const HuffmanNode&rightNode) const
//    {
//        int lhs = leftNode.getNumberOfReferences();
//        int rhs = rightNode.getNumberOfReferences();
//        
//        if (reverse) return (lhs>rhs);
//        else return (lhs<rhs);
//    }
//};

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
        //std::cout<<"Setting prefix code of Node " << node->getCharacter() << " to " << node->getPrefixCode()<<std::endl;
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
    //std::cout << "The Huffman Encoding string is: " << huffmanString << std::endl;
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
            //std::cout<< "Creating new node for " << c << std::endl;
        }
        
        int characterReferences = huffmanNode->getNumberOfReferences();
        huffmanNode->setNumberOfReferences(++characterReferences);
    }
    
    prioQueue = new std::priority_queue<HuffmanNode*,std::vector<HuffmanNode*>, CompareNode>;
    
    for (auto it = nodeMap->begin(); it != nodeMap->end(); ++it )
    {
        //char c = it->first;
        HuffmanNode *node = it->second;
        //std::cout << "Adding node " << c << ":" << node->getNumberOfReferences() << std::endl;
        //add nodes to the priority queue
        prioQueue->push(node);
        //node = prioQueue->top();
        //char prioNodeC = node->getCharacter();
        //std::cout << "Node at top  "<< prioNodeC << std::endl;
    }

    //we now have all our entries counted and added to min prio queue
    //Build our huffman tree
    while (prioQueue->size() > 2)
    {
        size_t originalSize = prioQueue->size();
        HuffmanNode *node1 = prioQueue->top();
        
        
//        std::cout<<"Node 1 is ";
//        if (node1->isLeafNode())
//            std::cout<< node1->getCharacter();
//        else
//            std::cout<<node1->getStringOfCharacters();
//        std::cout<<" with weight " <<node1->getNumberOfReferences()<<std::endl;
        
        prioQueue->pop();
        HuffmanNode *node2 = prioQueue->top();
        
        
//        std::cout<<"Node 2 is ";
//        if (node2->isLeafNode())
//            std::cout<< node2->getCharacter();
//        else
//            std::cout<<node2->getStringOfCharacters();
//        std::cout<<" with weight " <<node2->getNumberOfReferences()<<std::endl;
        
        
        prioQueue->pop();
        
        assert(prioQueue->size() == (originalSize - 2));
        
        //create a new node for these two nodes
        HuffmanNode *newNode = new HuffmanNode(node1, node2);

//        std::cout<<"New Node is ";
//        if (newNode->isLeafNode())
//            std::cout<< newNode->getCharacter();
//        else
//            std::cout<<newNode->getStringOfCharacters();
//        std::cout<<" with weight " <<newNode->getNumberOfReferences()<<std::endl;
        
        prioQueue->push(newNode);
        
        assert(prioQueue->size() == (originalSize - 1));
        
    }
    
    HuffmanNode *node1 = prioQueue->top();
    prioQueue->pop();
    HuffmanNode *node2 = prioQueue->top();
    prioQueue->pop();
    assert(prioQueue->size() == 0);
    
    
    
    rootNode = new HuffmanNode(node1, node2);
    
//    std::cout<<"Root Node is ";
//    if (rootNode->isLeafNode())
//        std::cout<< rootNode->getCharacter();
//    else
//        std::cout<<rootNode->getStringOfCharacters();
//    std::cout<<" with weight " <<rootNode->getNumberOfReferences()<<std::endl;
    
    //we now have a proper huffman tree
    //search the tree for leaves and populate prefix codes
    
    setPrefixCodes();
    
    //std::cout << std::endl;
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
    
    //std::cout<<"The encoded string for " <<stringToEncode<< " is "<<encodedString <<std::endl;
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
    
    //std::cout << N << " total strings to encode" << std::endl;
    
    auto it = stringsToEncode.begin();
    int i = 0;
    while (it != stringsToEncode.end())
    {
        i++;
        std::string encodedString;
        h.encode(*it++, encodedString);
        std::cout<<encodedString.length()<<std::endl;
        //std::cout << i << ") " << encodedString <<std::endl;
    }
    
    
    
    
    
    
    //std::cout << inputString <<" "<< inputString << std::endl;
    
    return 0;
}

