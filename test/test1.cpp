#include <json_parser.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <unistd.h>
using std::fstream;
using std::ifstream;
using std::istreambuf_iterator;
using std::streambuf;
using std::string;
static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;
using Json::ValueType;

//#define EXPECT_EQ_BASE(equality, expect, actual, format)                                                      \
//    do                                                                                                        \
//    {                                                                                                         \
//        test_count++;                                                                                         \
//        if (equality)                                                                                         \
//            test_pass++;                                                                                      \
//        else                                                                                                  \
//        {                                                                                                     \
//            fprintf(stderr, "%s:%d:expect" format "actual:" format "\n", __FILE__, __LINE__, expect, actual); \
//            main_ret = 1;                                                                                     \
//        }                                                                                                     \
//    } while (0)
//
//#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d");

void test_value_type(const string &in_str, ValueType expect_type)
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

void test_value_type_invalid(const string &in_str, ValueType expect_type)
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
    test_value_type("true", ValueType::TRUE_TYPE);
    test_value_type("  true  ", ValueType::TRUE_TYPE);
    test_value_type(" \r\n true\r \n\r  ", ValueType::TRUE_TYPE);
    test_value_type_invalid("tru", ValueType::TRUE_TYPE);
    test_value_type_invalid("  tru  ", ValueType::TRUE_TYPE);
    test_value_type_invalid("  \rtru\n  ", ValueType::TRUE_TYPE);

    printf("********************\n");

    /*test false*/
    test_value_type("false", ValueType::FALSE_TYPE);
    test_value_type("  false  ", ValueType::FALSE_TYPE);
    test_value_type(" \r  \n\rfalse\r\n  ", ValueType::FALSE_TYPE);
    test_value_type_invalid("ft", ValueType::FALSE_TYPE);
    test_value_type_invalid("   ft   ", ValueType::FALSE_TYPE);
    test_value_type_invalid("  \r \n  ft \r \n  ", ValueType::FALSE_TYPE);

    printf("********************\n");

    /*test null*/
    test_value_type("null", ValueType::NULL_TYPE);
    test_value_type("  null   ", ValueType::NULL_TYPE);
    test_value_type("  null \r \n  ", ValueType::NULL_TYPE);
    test_value_type_invalid("nul", ValueType::NULL_TYPE);
    test_value_type_invalid("     nul   ", ValueType::NULL_TYPE);
    test_value_type_invalid("  \r\n   nul\n   \r", ValueType::NULL_TYPE);

    printf("********************\n");

    /*test number*/
    test_value_type_invalid("-", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-e+2313", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-e-2313", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-.2313", ValueType::NUMBER_TYPE);
    test_value_type("1", ValueType::NUMBER_TYPE);
    test_value_type("2", ValueType::NUMBER_TYPE);
    test_value_type("3", ValueType::NUMBER_TYPE);
    test_value_type("4", ValueType::NUMBER_TYPE);
    test_value_type("5", ValueType::NUMBER_TYPE);
    test_value_type("6", ValueType::NUMBER_TYPE);
    test_value_type("7", ValueType::NUMBER_TYPE);
    test_value_type("8", ValueType::NUMBER_TYPE);
    test_value_type("9", ValueType::NUMBER_TYPE);
    test_value_type("0", ValueType::NUMBER_TYPE);
    test_value_type_invalid("0.", ValueType::NUMBER_TYPE);
    test_value_type_invalid("123.", ValueType::NUMBER_TYPE);
    test_value_type("0.123", ValueType::NUMBER_TYPE);
    test_value_type("  0.123e+0123  ", ValueType::NUMBER_TYPE);
    test_value_type("  0.123e-0123 ", ValueType::NUMBER_TYPE);
    test_value_type("  0.123e+123 ", ValueType::NUMBER_TYPE);
    test_value_type("  0.123e-123", ValueType::NUMBER_TYPE);
    test_value_type("  -0.123e+0123  ", ValueType::NUMBER_TYPE);
    test_value_type("  -0.123e-0123 ", ValueType::NUMBER_TYPE);
    test_value_type("  -0.123e+123 ", ValueType::NUMBER_TYPE);
    test_value_type("  -0.123e-123", ValueType::NUMBER_TYPE);
    test_value_type_invalid("01", ValueType::NUMBER_TYPE);
    test_value_type_invalid("014556", ValueType::NUMBER_TYPE);
    test_value_type_invalid("  014556  ", ValueType::NUMBER_TYPE);
    test_value_type_invalid("  014556.  ", ValueType::NUMBER_TYPE);
    test_value_type_invalid("\r  \n  \r  014556\r  \n  ", ValueType::NUMBER_TYPE);
    test_value_type_invalid("\r  \n  \r  014556\r  \n  ", ValueType::NUMBER_TYPE);
    test_value_type("1", ValueType::NUMBER_TYPE);
    test_value_type("123", ValueType::NUMBER_TYPE);
    test_value_type("-123", ValueType::NUMBER_TYPE);
    test_value_type("123.34", ValueType::NUMBER_TYPE);
    test_value_type("-123.34", ValueType::NUMBER_TYPE);
    test_value_type("123.44e+0", ValueType::NUMBER_TYPE);
    test_value_type("123.44e-0", ValueType::NUMBER_TYPE);
    test_value_type("123.44E+0", ValueType::NUMBER_TYPE);
    test_value_type("123.44E-0", ValueType::NUMBER_TYPE);
    test_value_type("123.44e+231", ValueType::NUMBER_TYPE);
    test_value_type("123.44e-231", ValueType::NUMBER_TYPE);
    test_value_type("123.44E+231", ValueType::NUMBER_TYPE);
    test_value_type("123.44E-231", ValueType::NUMBER_TYPE);
    test_value_type("-123.44e-231", ValueType::NUMBER_TYPE);
    test_value_type("-123.44e+231", ValueType::NUMBER_TYPE);
    test_value_type("-123.44E-231", ValueType::NUMBER_TYPE);
    test_value_type("-123.44E+231", ValueType::NUMBER_TYPE);
    test_value_type("232e+12345", ValueType::NUMBER_TYPE);
    test_value_type("23211413e-12345", ValueType::NUMBER_TYPE);
    test_value_type("-23211413e+12345", ValueType::NUMBER_TYPE);
    test_value_type("-23211413e-12345", ValueType::NUMBER_TYPE);
    test_value_type(" \r 123.44e+231  \r\n", ValueType::NUMBER_TYPE);
    test_value_type_invalid("+123123.56e234", ValueType::NUMBER_TYPE);
    test_value_type("232E+12345", ValueType::NUMBER_TYPE);
    test_value_type("23211413E-12345", ValueType::NUMBER_TYPE);
    test_value_type("-23211413E+12345", ValueType::NUMBER_TYPE);
    test_value_type("-23211413E-12345", ValueType::NUMBER_TYPE);
    test_value_type(" \r 123.44E+231  \r\n", ValueType::NUMBER_TYPE);
    test_value_type_invalid("+123123.56E234", ValueType::NUMBER_TYPE);
    test_value_type_invalid("+1231234", ValueType::NUMBER_TYPE);
    test_value_type_invalid("+1231234.", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-1231234.", ValueType::NUMBER_TYPE);
    test_value_type_invalid("1231234.", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-123123.56e+", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-123123.56e-", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-123123.56E+", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-123123.56E-", ValueType::NUMBER_TYPE);
    test_value_type("-123123.56e1231", ValueType::NUMBER_TYPE);
    test_value_type("123123.56e1231", ValueType::NUMBER_TYPE);
    test_value_type("-123123.56E1231", ValueType::NUMBER_TYPE);
    test_value_type("123123.56E1231", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-123123.56ee-", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-123123.56Ee-", ValueType::NUMBER_TYPE);
    test_value_type_invalid("-123123.56eE-", ValueType::NUMBER_TYPE);

    printf("********************\n");

    /*test string*/
    test_value_type("     \"hello\"   ", ValueType::STRING_TYPE);
    test_value_type("\"\"", ValueType::STRING_TYPE);
    test_value_type("     \"你好 这里是utf8\"   ", ValueType::STRING_TYPE);
    test_value_type("     \"h\\\"el\\blo\"   ", ValueType::STRING_TYPE);
    test_value_type_invalid("     \"h\\\"el\\\blo\"", ValueType::STRING_TYPE);
    test_value_type("     \"h   \\\"el\\b\\\\lo\"   ", ValueType::STRING_TYPE);
    test_value_type("\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"", ValueType::STRING_TYPE);

    printf("********************\n");

    /*test array*/
    test_value_type("     [2,null]   ", ValueType::ARRAY_TYPE);
    test_value_type("     []   ", ValueType::ARRAY_TYPE);
    test_value_type_invalid("     [   ", ValueType::ARRAY_TYPE);
    test_value_type_invalid("     [2,4   ", ValueType::ARRAY_TYPE);
    test_value_type("     [\"hello\",3.33,null,[1,2,3,true]]   ", ValueType::ARRAY_TYPE);
    test_value_type("     [  \r\n\"hello\",3,null,[1,\r  2,3,true]]   ", ValueType::ARRAY_TYPE);
    test_value_type("     [  \r\n\"hello\",3,null,[-13445.213e+1234,\r  2,3,true]]   ", ValueType::ARRAY_TYPE);

    printf("********************\n");

    /*test object*/
    test_value_type("{}", ValueType::OBJECT_TYPE);
    test_value_type("{\"hello\":[]}", ValueType::OBJECT_TYPE);
    test_value_type("{\"hello\":\"world\"}", ValueType::OBJECT_TYPE);
    test_value_type("{\"hello\":[],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);
    test_value_type_invalid("{", ValueType::OBJECT_TYPE);
    test_value_type_invalid("{hhhh", ValueType::OBJECT_TYPE);
    test_value_type_invalid("{\"one\":3,3:3}", ValueType::OBJECT_TYPE);
    test_value_type("{\"hello\":[1,2,3],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);
    test_value_type("{\"hello\":[1,2,3,{}],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);
    test_value_type("{\"hello\"  :\r\n[1,2,3,{}],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);
    test_value_type("{\"hello\"  :\r\n\t[1,2,3,{}] , \"world\": \r\n4556.333e-21 \r\n}", ValueType::OBJECT_TYPE);
    test_value_type("{\"hello\"  :\r\n\t[1,2,3,{} \r\n] , \"world\": \r\n4556.333e-21 \r\n}", ValueType::OBJECT_TYPE);
    test_value_type("{\"hello\"  :\r\n\t[1,2,3,{\r\n   } \r\n] , \"world\": \r\n4556.333e-21 \r\n}", ValueType::OBJECT_TYPE);
    test_value_type("{\"hello\":[1,2,3,{\"one\":0,\"two\":\"this is hello\",\"third\":[1,2,3]}],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);
    test_value_type("{  \r\n \"hello\":[1,2,\"v1\",3,{\"one\":0,\"two\":\"this is hello\",\"third\":[1,2,true,3]}],\"world\":4556.333e-21}", ValueType::OBJECT_TYPE);

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

    Json::Value v;
    ifstream in_f("test/test.json");
    if(in_f)
    {
        istreambuf_iterator<char>beg(in_f),end;
        string str(beg,end);
        std::cout << str <<std::endl;
        Json::Reader reader(str,v);
        auto result = reader.parse();
        if (result != Json::ParseStatus::OK)
        {
            Json::ParseStatusPrint(result);
            return 0;
        }
        v.debug_print();
        if (v.isObject())
        {
            std::cout << v["hello"].asString() << std::endl;
            //v["one"].debug_print();
            std::cout << v["one"].asString() << std::endl;
            std::cout << v["second"].asDouble() << std::endl;
            std::cout << v["third"][0].asDouble() <<std::endl;
            std::cout << v["third"][1].asBool() <<std::endl;
            std::cout << v["third"][2].asString() <<std::endl;
        }
    }
    else
    {
        printf("open file failed.\n");
    }


    return 0;
}