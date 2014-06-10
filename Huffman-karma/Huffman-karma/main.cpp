//
//  main.cpp
//  Huffman-karma
//
//  Created by James Kizer on 6/9/14.
//  Copyright (c) 2014 JimmyTime Software. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include "assert.h"

typedef struct{
    
    char *ptr;
    size_t len;
    
} myBasicString;

#define MIN_PREFIX_CODE_LEN 1
#define MAX_PREFIX_CODE_LEN 25

size_t MIN(size_t x, size_t y)
{
    return (x<y)?x:y;
}

bool charIsInPrefixCodeMap(char c, std::unordered_map<char, myBasicString> *prefixCodeMap)
{
    auto iter = prefixCodeMap->find(c);
    
    if (iter == prefixCodeMap->end())
        return false;
    else
        return true;
}

void addCharAndPrefixCodeToPrefixCodeMap(char c, myBasicString prefixCode, std::unordered_map<char, myBasicString> *prefixCodeMap)
{
    auto iter = prefixCodeMap->find(c);
    
    assert(iter == prefixCodeMap->end());
    if (iter == prefixCodeMap->end())
        (*prefixCodeMap)[c] = prefixCode;
}

void removeCharFromPrefixCodeMap(char c, std::unordered_map<char, myBasicString> *prefixCodeMap)
{
    auto iter = prefixCodeMap->find(c);
    assert(iter != prefixCodeMap->end());
    prefixCodeMap->erase(c);
}


//this function runs order(m) where m is distinct characters in the inpt string
bool validCharAndPrefixCodeForPrefixCodeMap(char c, myBasicString prefixCode, std::unordered_map<char, myBasicString> *prefixCodeMap)
{
    
    myBasicString mapPrefixCode;
    
    
    assert(!charIsInPrefixCodeMap(c, prefixCodeMap));
    
    //c does not exist, check that prefixCode is valid for existing prefixCodes in the map
    //i.e., prefixCode is not a substring of any existing prefix codes
    //nor are any existing prefix codes a substring of prefixCode
    
    //iterate over all values in prefixCodeMap
    for ( auto it = prefixCodeMap->begin(); it != prefixCodeMap->end(); ++it )
    {
        //get value in the map
        mapPrefixCode = it->second;
        
        //for the min number of bytes between the two prefixCodes, compare strings
        size_t compareLength = MIN(prefixCode.len, mapPrefixCode.len);
        if (strncmp(prefixCode.ptr, mapPrefixCode.ptr, compareLength) == 0)
            return false;
    }
    return true;
}


bool HuffmanDecodeRec(myBasicString plainText, myBasicString bitString, std::unordered_map<char, myBasicString> *prefixCodeMap)
{
    
//    if ( (plainText.len == 0) && (bitString.len == 0))
//        return true;
    
    char firstChar = plainText.ptr[0];
    
    
    
    //if next plainText char is already in the map, check to see if first m bits in bit string match
    //if so iterate and recurse
    //else, return false
    if (charIsInPrefixCodeMap(firstChar, prefixCodeMap))
    {
        
        myBasicString mapPrefixCode = prefixCodeMap->at(firstChar);
        if( strncmp(mapPrefixCode.ptr, bitString.ptr, mapPrefixCode.len) == 0)
        {
            myBasicString remainderPlainText;
            remainderPlainText.ptr = plainText.ptr + 1;
            remainderPlainText.len = plainText.len - 1;
            
            myBasicString remainderPrefixCode;
            remainderPrefixCode.ptr = bitString.ptr + mapPrefixCode.len;
            remainderPrefixCode.len = bitString.len - mapPrefixCode.len;
            
            if ((remainderPlainText.len == 0) && (remainderPrefixCode.len == 0))
                return true;
            
            if(HuffmanDecodeRec(remainderPlainText, remainderPrefixCode, prefixCodeMap) == true)
                return true;
        }
        else
            return false;
    }
    else
    {
        if(plainText.len == 1)
            return validCharAndPrefixCodeForPrefixCodeMap( firstChar, bitString, prefixCodeMap);
        
        for(int i=MIN_PREFIX_CODE_LEN; i<MAX_PREFIX_CODE_LEN && i<bitString.len; i++)
        {
            
            myBasicString firstPrefixCode;
            
            firstPrefixCode.ptr = bitString.ptr;
            firstPrefixCode.len = i;
            
            
            if(!validCharAndPrefixCodeForPrefixCodeMap(firstChar, firstPrefixCode, prefixCodeMap))
                //this prefix code length is not valid, iterate
                continue;
            
            
            //addCharAndPrefixCodeToPrefixCodeMap checks to see if this exists in the map before we add it
            addCharAndPrefixCodeToPrefixCodeMap(firstChar, firstPrefixCode, prefixCodeMap);
            
            myBasicString remainderPlainText;
            remainderPlainText.ptr = plainText.ptr + 1;
            remainderPlainText.len = plainText.len - 1;
            
            myBasicString remainderPrefixCode;
            remainderPrefixCode.ptr = bitString.ptr + i;
            remainderPrefixCode.len = bitString.len - i;
            
            if ((remainderPlainText.len == 0) && (remainderPrefixCode.len == 0))
                return true;
            
            if(HuffmanDecodeRec(remainderPlainText, remainderPrefixCode, prefixCodeMap) == true)
                return true;
            
            removeCharFromPrefixCodeMap(firstChar, prefixCodeMap);
        }
        
        return false;
    }
    return false;
}

bool HuffmanDecode(std::string &plainText, std::string &bitString)
{
    std::unordered_map<char, myBasicString> *prefixCodeMap;
    
    //get c pointers to string data
    myBasicString plainTextBasicString;
    plainTextBasicString.ptr = (char *)plainText.data();
    plainTextBasicString.len = plainText.length();
    
    myBasicString bitStringBasicString;
    bitStringBasicString.ptr = (char *)bitString.data();
    bitStringBasicString.len = bitString.length();
    
    if (plainText.length() == 1)
        return true;
    
    for(int i=MIN_PREFIX_CODE_LEN; i<MAX_PREFIX_CODE_LEN && i<bitStringBasicString.len; i++)
    {
        
        prefixCodeMap = new std::unordered_map<char, myBasicString>;
        
        //get first char
        char firstChar = plainTextBasicString.ptr[0];
        myBasicString firstPrefixCode;
        
        firstPrefixCode.ptr = bitStringBasicString.ptr;
        firstPrefixCode.len = i;
        
        addCharAndPrefixCodeToPrefixCodeMap(firstChar, firstPrefixCode, prefixCodeMap);
        
        myBasicString remainderPlainText;
        remainderPlainText.ptr = plainTextBasicString.ptr + 1;
        remainderPlainText.len = plainTextBasicString.len - 1;
        
        myBasicString remainderPrefixCode;
        remainderPrefixCode.ptr = bitStringBasicString.ptr + i;
        remainderPrefixCode.len = bitStringBasicString.len - i;
        
        if (HuffmanDecodeRec(remainderPlainText, remainderPrefixCode, prefixCodeMap) == true)
        {
            ::operator delete(prefixCodeMap);
            return true;
        }
        
        ::operator delete(prefixCodeMap);
    }
    
    return false;
}

int main(int argc, const char * argv[])
{

    int N;
    
    std::istream &input = std::cin;
    std::ostream &output = std::cout;
    std::string plainText;
    std::string bitString;
    
    
    input >> N;
    
    for (int i=0; i<N; i++)
    {
        input >> plainText;
        input >> bitString;
        
        if (HuffmanDecode(plainText, bitString))
            output << "Y" <<std::endl;
        else
            output << "N" <<std::endl;
    }
    
    
    return 0;
}

