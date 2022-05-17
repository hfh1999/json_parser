#ifndef VALUE_DATA_H
#define VALUE_DATA_H
#include <string>
using std::string;
struct ValueData
{
    double n; // 存储数字
    string s; // 存储字符串
    ValueData(double in_num):n(in_num){}
    ValueData(const string& in_s):s(in_s){}
};
#endif