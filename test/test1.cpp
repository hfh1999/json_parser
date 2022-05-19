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

    printf("********************\n");

    /*test false*/
    test_value("false", ValueType::FALSE_TYPE);
    test_value("  false  ", ValueType::FALSE_TYPE);
    test_value(" \r  \n\rfalse\r\n  ", ValueType::FALSE_TYPE);
    test_value_panic("ft", ValueType::FALSE_TYPE);
    test_value_panic("   ft   ", ValueType::FALSE_TYPE);
    test_value_panic("  \r \n  ft \r \n  ", ValueType::FALSE_TYPE);

    printf("********************\n");

    /*test null*/
    test_value("null", ValueType::NULL_TYPE);
    test_value("  null   ", ValueType::NULL_TYPE);
    test_value("  null \r \n  ", ValueType::NULL_TYPE);
    test_value_panic("nul", ValueType::NULL_TYPE);
    test_value_panic("     nul   ", ValueType::NULL_TYPE);
    test_value_panic("  \r\n   nul\n   \r", ValueType::NULL_TYPE);

    printf("********************\n");

    /*test number*/
    test_value_panic("-", ValueType::NUMBER_TYPE);
    test_value_panic("-e+2313", ValueType::NUMBER_TYPE);
    test_value_panic("-e-2313", ValueType::NUMBER_TYPE);
    test_value_panic("-.2313", ValueType::NUMBER_TYPE);
    test_value("1", ValueType::NUMBER_TYPE);
    test_value("2", ValueType::NUMBER_TYPE);
    test_value("3", ValueType::NUMBER_TYPE);
    test_value("4", ValueType::NUMBER_TYPE);
    test_value("5", ValueType::NUMBER_TYPE);
    test_value("6", ValueType::NUMBER_TYPE);
    test_value("7", ValueType::NUMBER_TYPE);
    test_value("8", ValueType::NUMBER_TYPE);
    test_value("9", ValueType::NUMBER_TYPE);
    test_value("0", ValueType::NUMBER_TYPE);
    test_value_panic("0.", ValueType::NUMBER_TYPE);
    test_value_panic("123.", ValueType::NUMBER_TYPE);
    test_value("0.123", ValueType::NUMBER_TYPE);
    test_value("  0.123e+0123  ", ValueType::NUMBER_TYPE);
    test_value("  0.123e-0123 ", ValueType::NUMBER_TYPE);
    test_value("  0.123e+123 ", ValueType::NUMBER_TYPE);
    test_value("  0.123e-123", ValueType::NUMBER_TYPE);
    test_value("  -0.123e+0123  ", ValueType::NUMBER_TYPE);
    test_value("  -0.123e-0123 ", ValueType::NUMBER_TYPE);
    test_value("  -0.123e+123 ", ValueType::NUMBER_TYPE);
    test_value("  -0.123e-123", ValueType::NUMBER_TYPE);
    test_value_panic("01", ValueType::NUMBER_TYPE);
    test_value_panic("014556", ValueType::NUMBER_TYPE);
    test_value_panic("  014556  ", ValueType::NUMBER_TYPE);
    test_value_panic("  014556.  ", ValueType::NUMBER_TYPE);
    test_value_panic("\r  \n  \r  014556\r  \n  ", ValueType::NUMBER_TYPE);
    test_value_panic("\r  \n  \r  014556\r  \n  ", ValueType::NUMBER_TYPE);
    test_value("1", ValueType::NUMBER_TYPE);
    test_value("123", ValueType::NUMBER_TYPE);
    test_value("-123", ValueType::NUMBER_TYPE);
    test_value("123.34", ValueType::NUMBER_TYPE);
    test_value("-123.34", ValueType::NUMBER_TYPE);
    test_value("123.44e+0", ValueType::NUMBER_TYPE);
    test_value("123.44e-0", ValueType::NUMBER_TYPE);
    test_value("123.44E+0", ValueType::NUMBER_TYPE);
    test_value("123.44E-0", ValueType::NUMBER_TYPE);
    test_value("123.44e+231", ValueType::NUMBER_TYPE);
    test_value("123.44e-231", ValueType::NUMBER_TYPE);
    test_value("123.44E+231", ValueType::NUMBER_TYPE);
    test_value("123.44E-231", ValueType::NUMBER_TYPE);
    test_value("-123.44e-231", ValueType::NUMBER_TYPE);
    test_value("-123.44e+231", ValueType::NUMBER_TYPE);
    test_value("-123.44E-231", ValueType::NUMBER_TYPE);
    test_value("-123.44E+231", ValueType::NUMBER_TYPE);
    test_value("232e+12345", ValueType::NUMBER_TYPE);
    test_value("23211413e-12345", ValueType::NUMBER_TYPE);
    test_value("-23211413e+12345", ValueType::NUMBER_TYPE);
    test_value("-23211413e-12345", ValueType::NUMBER_TYPE);
    test_value(" \r 123.44e+231  \r\n", ValueType::NUMBER_TYPE);
    test_value_panic("+123123.56e234", ValueType::NUMBER_TYPE);
    test_value("232E+12345", ValueType::NUMBER_TYPE);
    test_value("23211413E-12345", ValueType::NUMBER_TYPE);
    test_value("-23211413E+12345", ValueType::NUMBER_TYPE);
    test_value("-23211413E-12345", ValueType::NUMBER_TYPE);
    test_value(" \r 123.44E+231  \r\n", ValueType::NUMBER_TYPE);
    test_value_panic("+123123.56E234", ValueType::NUMBER_TYPE);
    test_value_panic("+1231234", ValueType::NUMBER_TYPE);
    test_value_panic("+1231234.", ValueType::NUMBER_TYPE);
    test_value_panic("-1231234.", ValueType::NUMBER_TYPE);
    test_value_panic("1231234.", ValueType::NUMBER_TYPE);
    test_value_panic("-123123.56e+", ValueType::NUMBER_TYPE);
    test_value_panic("-123123.56e-", ValueType::NUMBER_TYPE);
    test_value_panic("-123123.56E+", ValueType::NUMBER_TYPE);
    test_value_panic("-123123.56E-", ValueType::NUMBER_TYPE);
    test_value("-123123.56e1231", ValueType::NUMBER_TYPE);
    test_value("123123.56e1231", ValueType::NUMBER_TYPE);
    test_value("-123123.56E1231", ValueType::NUMBER_TYPE);
    test_value("123123.56E1231", ValueType::NUMBER_TYPE);
    test_value_panic("-123123.56ee-", ValueType::NUMBER_TYPE);
    test_value_panic("-123123.56Ee-", ValueType::NUMBER_TYPE);
    test_value_panic("-123123.56eE-", ValueType::NUMBER_TYPE);

    printf("********************\n");

    /*test string*/
    test_value("     \"hello\"   ", ValueType::STRING_TYPE);
    test_value("\"\"", ValueType::STRING_TYPE);
    test_value("     \"你好 这里是utf8\"   ", ValueType::STRING_TYPE);
    test_value("     \"h\\\"el\\blo\"   ", ValueType::STRING_TYPE);
    test_value_panic("     \"h\\\"el\\\blo\"", ValueType::STRING_TYPE);
    test_value("     \"h   \\\"el\\b\\\\lo\"   ", ValueType::STRING_TYPE);

    printf("********************\n");

    /*test array*/
    test_value("     [2,null]   ", ValueType::ARRAY_TYPE);
    test_value("     []   ", ValueType::ARRAY_TYPE);
    test_value_panic("     [   ", ValueType::ARRAY_TYPE);
    test_value_panic("     [2,4   ", ValueType::ARRAY_TYPE);
    test_value("     [\"hello\",3.33,null,[1,2,3,true]]   ", ValueType::ARRAY_TYPE);
    test_value("     [  \r\n\"hello\",3,null,[1,\r  2,3,true]]   ", ValueType::ARRAY_TYPE);
    test_value("     [  \r\n\"hello\",3,null,[-13445.213e+1234,\r  2,3,true]]   ", ValueType::ARRAY_TYPE);

    printf("********************\n");

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

    printf("********************\n");

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