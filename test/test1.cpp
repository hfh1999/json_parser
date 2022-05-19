#include <json_parser.h>
#include <iostream>
using std::string;
static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;
using Json::ValueType;

#define EXPECT_EQ_BASE(equality, expect, actual, format)                                                      \
    do                                                                                                        \
    {                                                                                                         \
        test_count++;                                                                                         \
        if (equality)                                                                                         \
            test_pass++;                                                                                      \
        else                                                                                                  \
        {                                                                                                     \
            fprintf(stderr, "%s:%d:expect" format "actual:" format "\n", __FILE__, __LINE__, expect, actual); \
            main_ret = 1;                                                                                     \
        }                                                                                                     \
    } while (0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d");

void test_value(const string &in_str, ValueType expect_type)
{
    test_count++;
    Json::Value v;
    Json::Reader reader(in_str, v);
    if (reader.parse() == Json::ParseStatus::OK && v.getType() == expect_type)
    {
        printf("test %d Ok.\n", test_count);
        test_pass += 1;
    }
    else
    {
        printf("test %d Failed.\n", test_count);
    }
}

void test_value_panic(const string &in_str, ValueType expect_type)
{
    test_count++;
    Json::Value v;
    Json::Reader reader(in_str, v);
    if (reader.parse() != Json::ParseStatus::OK)
    {
        printf("test %d Ok.\n", test_count);
        test_pass += 1;
    }
    else
    {
        printf("test %d Failed: should not expect but yes.\n", test_count);
    }
}

int main()
{
    /*test true*/
    test_value("true", ValueType::TRUE_TYPE);
    test_value("  true  ", ValueType::TRUE_TYPE);
    test_value(" \r\n true\r \n\r  ", ValueType::TRUE_TYPE);
    test_value_panic("tru", ValueType::TRUE_TYPE);
    test_value_panic("  tru  ", ValueType::TRUE_TYPE);
    test_value_panic("  \rtru\n  ", ValueType::TRUE_TYPE);

    /*test false*/
    test_value("false", ValueType::FALSE_TYPE);
    test_value("  false  ", ValueType::FALSE_TYPE);
    test_value(" \r  \n\rfalse\r\n  ", ValueType::FALSE_TYPE);
    test_value_panic("ft", ValueType::FALSE_TYPE);
    test_value_panic("   ft   ", ValueType::FALSE_TYPE);
    test_value_panic("  \r \n  ft \r \n  ", ValueType::FALSE_TYPE);

    /*test null*/
    test_value("null", ValueType::NULL_TYPE);
    test_value("  null   ", ValueType::NULL_TYPE);
    test_value("  null \r \n  ", ValueType::NULL_TYPE);
    test_value_panic("nul", ValueType::NULL_TYPE);
    test_value_panic("     nul   ", ValueType::NULL_TYPE);
    test_value_panic("  \r\n   nul\n   \r", ValueType::NULL_TYPE);

    /*test number*/
    //test_value("0", ValueType::NUMBER_TYPE);
    //test_value_panic("01", ValueType::NUMBER_TYPE);
    //test_value_panic("014556", ValueType::NUMBER_TYPE);
    //test_value("0.123", ValueType::NUMBER_TYPE);
    test_value("123", ValueType::NUMBER_TYPE);
    test_value("-123", ValueType::NUMBER_TYPE);
    test_value("123.34", ValueType::NUMBER_TYPE);
    test_value("-123.34", ValueType::NUMBER_TYPE);
    test_value("123.44e+0", ValueType::NUMBER_TYPE);
    test_value("123.44e-0", ValueType::NUMBER_TYPE);
    test_value("123.44e+231", ValueType::NUMBER_TYPE);
    test_value("123.44e-231", ValueType::NUMBER_TYPE);
    test_value("-123.44e-231", ValueType::NUMBER_TYPE);
    test_value("-123.44e+231", ValueType::NUMBER_TYPE);
    test_value(" \r 123.44e+231  \r\n", ValueType::NUMBER_TYPE);

    /*test string*/
    test_value("     \"hello\"   ", ValueType::STRING_TYPE);
    test_value("\"\"", ValueType::STRING_TYPE);
    test_value("     \"你好 这里是utf8\"   ", ValueType::STRING_TYPE);
    test_value("     \"h\\\"el\\blo\"   ", ValueType::STRING_TYPE);
    test_value_panic("     \"h\\\"el\\\blo\"", ValueType::STRING_TYPE);
    test_value("     \"h   \\\"el\\b\\\\lo\"   ", ValueType::STRING_TYPE);

    /*test array*/
    test_value("     [2,null]   ", ValueType::ARRAY_TYPE);
    test_value("     []   ", ValueType::ARRAY_TYPE);
    test_value_panic("     [   ", ValueType::ARRAY_TYPE);
    test_value_panic("     [2,4   ", ValueType::ARRAY_TYPE);
    test_value("     [\"hello\",3,null,[1,2,3,true]]   ", ValueType::ARRAY_TYPE);
    test_value("     [  \r\n\"hello\",3,null,[1,\r  2,3,true]]   ", ValueType::ARRAY_TYPE);
    test_value("     [  \r\n\"hello\",3,null,[-13445.213e+1234,\r  2,3,true]]   ", ValueType::ARRAY_TYPE);

    /*test object*/
    test_value("{}", ValueType::OBJECT_TYPE);
    test_value("{\"hello\":[]}", ValueType::OBJECT_TYPE);
    test_value("{\"hello\":[],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);
    test_value_panic("{", ValueType::OBJECT_TYPE);
    test_value_panic("{hhhh", ValueType::OBJECT_TYPE);
    test_value_panic("{\"one\":3,3:3}", ValueType::OBJECT_TYPE);
    test_value("{\"hello\":[1,2,3],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);
    test_value("{\"hello\":[1,2,3,{}],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);
    test_value("{\"hello\":[1,2,3,{\"one\":0,\"two\":\"this is hello\",\"third\":[1,2,3]}],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);
    test_value("{\"hello\":[1,2,\"v1\",3,{\"one\":0,\"two\":\"this is hello\",\"third\":[1,2,true,3]}],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);

    if (test_count == test_pass)
    {
        printf("============\n");
        printf("ALL is Pass.\n");
        printf("============\n");
    }
    else
    {
        printf("============\n");
        printf("%d/%d Pass.\n", test_pass, test_count);
        printf("============\n");
    }
    return 0;
}