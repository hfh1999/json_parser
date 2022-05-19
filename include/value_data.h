#ifndef VALUE_DATA_H
#define VALUE_DATA_H
#include <string>
#include <vector>
#include <json_parser.h>
#include <map>
using std::multimap;
using std::string;
using std::vector;

using Json::Value; // 依赖性再考虑一下
struct ValueData
{
    double n; // 存储数字
    string s; // 存储字符串
    vector<Value> array;
    multimap<string, Value> object;

    ValueData() {}
    ValueData(double in_num) : n(in_num) {}
    ValueData(const string &in_s) : s(in_s) {}
};
#endif