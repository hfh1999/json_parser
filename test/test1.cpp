#include <json_parser.h>
#include <iostream>
int main(){
    Json::Value v;
    Json::Value v2;
    Json::Reader reader("  trueh  ",v);
    Json::Reader reader2("  \n null \r \n  ",v2);
    reader.parse();
    reader2.parse();
    if(v.isBool()) 
    {
        printf("is bool.\n");
    }
    else
    {
        printf("not bool.\n");
    }
    if(v2.isNull()) 
    {
        printf("is bool.\n");
    }
    else
    {
        printf("not bool.\n");
    }
    v.debug_print();
    v2.debug_print();
    return 0;
}