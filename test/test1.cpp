#include <json_parser.h>
#include <iostream>
using std::string;
static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;
using Json::ValueType;

#define EXPECT_EQ_BASE(equality,expect,actual,format)\
    do {\
        test_count++;\
        if(equality)\
            test_pass++;\
        else{\
            fprintf(stderr,"%s:%d:expect" format "actual:" format "\n",__FILE__,__LINE__,expect,actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect,actual) EXPECT_EQ_BASE((expect) == (actual) , expect, actual,"%d");

void test_value(const string& in_str,ValueType expect_type)
{
    test_count++;
    Json::Value v;
    Json::Reader reader(in_str,v);
    if(reader.parse() == Json::ParseError::OK && v.getType() == expect_type)
    {
        printf("test %d Ok.\n",test_count);
    }
    else
    {
        printf("test %d Failed.\n",test_count);
    }
}

void test_value_panic(const string& in_str,ValueType expect_type)
{
    test_count++;
    Json::Value v;
    Json::Reader reader(in_str,v);
    if(reader.parse() != Json::ParseError::OK)
    {
        printf("test %d Ok.\n",test_count);
    }
    else
    {
        printf("test %d Failed: should not expect but yes.\n",test_count);
    }
}

int main(){
    test_value("true",ValueType::TRUE_TYPE);
    test_value("  true  ",ValueType::TRUE_TYPE);
    test_value("  false\r",ValueType::FALSE_TYPE);
    test_value("  null \r \n  ",ValueType::NULL_TYPE);
    test_value(" \r 123.44e+231  \r\n",ValueType::NUMBER_TYPE);
    test_value("     \"hello\"   ",ValueType::STRING_TYPE);
    test_value("     \"h\\\"el\\blo\"   ",ValueType::STRING_TYPE);
    test_value_panic("     \"h\\\"el\\\blo\"",ValueType::STRING_TYPE);
    test_value( "     \"h   \\\"el\\b\\\\lo\"   ",ValueType::STRING_TYPE);
    test_value("     [2,null]   ",ValueType::ARRAY_TYPE);
    test_value( "     [\"hello\",3,null,[1,2,3,true]]   ",ValueType::ARRAY_TYPE);
    test_value( "     [  \r\n\"hello\",3,null,[1,\r  2,3,true]]   ",ValueType::ARRAY_TYPE);
    test_value( "     [  \r\n\"hello\",3,null,[-13445.213e+1234,\r  2,3,true]]   ",ValueType::ARRAY_TYPE);
    return 0;
}