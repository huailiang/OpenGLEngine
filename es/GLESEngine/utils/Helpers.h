//
//  Helpers.h
//  GLESEngine
//
//  Created by 彭怀亮 on 6/1/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#ifndef Helpers_h
#define Helpers_h


#include <iostream>
#include <string>
#include <algorithm>

using namespace std;


class StringManipulator
{
public:
    static void SplitFileName(const string& str, string &folder, string &file)
    {
        size_t found;
        found=str.find_last_of("/\\");
#ifdef DEBUG
        cerr << "Splitting: " << str << endl;
        cerr << " folder: " << str.substr(0,found) << endl;
        cerr << " file: " << str.substr(found+1) << endl;
#endif
        folder=str.substr(0,found);
        file=str.substr(found+1) ;
    }
    
    static void GetExtensitonType(const string& str, string &extension)
    {
        size_t found=str.find_last_of(".");
        extension = str.substr(found+1);
        
    }
    static void GetBaseName(const string& str, string &base)
    {
        size_t found=str.find_last_of(".");
        base = str.substr(0, found);
        
    }
    
    static void ToLowerCase(string& str)
    {
        std::transform(str.begin(),str.end(),str.begin(),::tolower);
    }
    static void ToUpperCase(string& str)
    {
        std::transform(str.begin(),str.end(),str.begin(),::toupper);
    }
    static int IsEqual(const string &str1, const string &str2)
    {
        return str1.compare(str2);
    }
    static void AddCharArrayToString(string &str,const char* array)
    {
        int i=0;
        while(array[i]!='\0')
        {
            str+=array[i];
            i++;
        }
    }
};

// Macro which returns a random value between -1 and 1
#define RAND_MINUS_1_TO_1() ((random() / (float32)0x3fffffff )-1.0f)

// MAcro which returns a random number between 0 and 1
#define RAND_0_TO_1() ((random() / (float32)0x7fffffff ))

#endif /* Helpers_h */
