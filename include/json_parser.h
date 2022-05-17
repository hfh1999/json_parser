#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include <string>
#include <value_data.h>
#include <memory>
using std::string;
using std::unique_ptr;

class TokenStream;
namespace Json
{
    // 表示解析过后的数据
    enum ValueType
    {
        NULL_TYPE,
        TRUE_TYPE,
        FALSE_TYPE,
        NUMBER_TYPE,
    };
    class Value
    {
    public:
        Value() = default;
        Value(ValueType in_type) : _type(in_type),_data() {}
        Value(ValueType in_type, double in_num):_type(in_type),_data(new ValueData(in_num)){}
        bool isBool()
        {
            return _type == ValueType::FALSE_TYPE ||
                   _type == ValueType::TRUE_TYPE;
        }
        bool isNull()
        {
            return _type == ValueType::NULL_TYPE;
        }
        bool isNumber()
        {
            return _type == ValueType::NUMBER_TYPE;
        }
        void debug_print()
        {
            switch(_type)
            {
                case ValueType::TRUE_TYPE:
                case ValueType::FALSE_TYPE:
                    printf("boolen type.\n");
                    break;
                case ValueType::NULL_TYPE:
                    printf("null type.\n");
                    break;
                case ValueType::NUMBER_TYPE:
                    printf("NUMBER type.\n");
                    break;
                default:
                    printf("Unknow type.\n");
            }
        }

    private:
        double ret_number(){
            return _data->n;
        }
        ValueType _type;
        unique_ptr<ValueData> _data;
    };
    enum ParseError
    {
        INVALID_VALUE,
        EXPECT_VALUE,
        ERROR_KEY_WORD,
        OK,
    };
    // 从字符串中读取。
    class Reader
    {
    public:
        Reader(const string &in_str, Value &in_val);
        ParseError parse();

    private:
        ParseError _parse_json(TokenStream &in_tokens, Value &in_value);
        void _parse_whitespace(TokenStream &in_tokens);
        ParseError _parse_null(TokenStream &in_tokens, Value &in_value);
        ParseError _parse_value(TokenStream &in_tokens, Value &in_value);
        ParseError _parse_boolen(TokenStream &in_tokens, Value &in_value);
        ParseError _parse_number(TokenStream &in_tokens, Value &in_value);

        Value& _parsed_result;
        string _raw_str;
    };
}
#endif