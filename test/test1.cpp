#include <json_parser.h>
#include <iostream>
using std::string;
void test(const string& in_str);
int main(){
    string str1 = "true";
    string str2 = "  true";
    string str3 = "  false";
    string str4 = "  null \r \n  ";
    string str5 = "  123.44e+231  \r\n";
    string str6 = "     \"hello\"   ";
    string str7 = "     \"h\\\"el\\\blo\"   ";
    string str8 = "     \"h\\\"el\\blo\"   ";
    string str9 = "     \"h   \\\"el\\blo\"   ";
    string str10 = "     \"h   \\\"el\\b\\\\lo\"   ";
    test(str1);
    test(str2);
    test(str3);
    test(str4);
    test(str5);
    test(str6);
    test(str7);
    test(str8);
    test(str9);
    test(str10);
    return 0;
}
void test(const string& in_str)
{
    Json::Value v;
    v.debug_print();
    Json::Reader reader(in_str,v);
    reader.parse();
    v.debug_print();
    printf("=====\n");
}