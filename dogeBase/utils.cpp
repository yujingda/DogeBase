//
//  utils.cpp
//  dbm
//
//  Created by 周坤 on 2018/11/5.
//  Copyright © 2018年 周坤. All rights reserved.
//

#include <vector>
#include "dbHead.h"
using namespace std;

//字符串分割
void splitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

