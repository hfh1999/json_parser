#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include <string>
#include <value_data.h>
#include <cassert>
#include <memory>
#include <map>
using std::multimap;
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
        STRING_TYPE,
        ARRAY_TYPE,
        OBJECT_TYPE,
        INVALID_TYPE,
    };
    class Value
    {
        friend class Reader;

    public:
        Value() : _type(ValueType::INVALID_TYPE), _data() {}
        Value(ValueType in_type) : _type(in_type), _data() {}
        Value(ValueType in_type, double in_num) : _type(in_type), _data(new ValueData(in_num)) {}
        Value(ValueType in_type, const string &in_s) : _type(in_type), _data(new ValueData(in_s)) {}
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
        bool isString()
        {
            return _type == ValueType::STRING_TYPE;
        }
        bool isArray()
        {
            return _type == ValueType::ARRAY_TYPE;
        }
        bool isObject()
        {
            return _type == ValueType::OBJECT_TYPE;
        }

        ValueType getType()
        {
            return _type;
        }

        void debug_print()
        {
            switch (_type)
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
            case ValueType::STRING_TYPE:
                printf("STRING type.\n");
                break;
            case ValueType::INVALID_TYPE:
                printf("Invalid type.\n");
                break;
            case ValueType::ARRAY_TYPE:
                printf("Array type.\n");
                break;
            case ValueType::OBJECT_TYPE:
                printf("Object type.\n");
                break;
            default:
                printf("Unknow type.\n");
            }
        }

    private:
        double ret_number()
        {
            return _data->n;
        }
        void push_value(Value &&in_v)
        {
            _array.push_back(std::move(in_v)); // 必须显式的指定使用right value版本的push_back.
        }
        void insert_object(const string &key, Value &&in_v)
        {
            _object.emplace(key, std::move(in_v));
        }
        void set_type(ValueType in_type)
        {
            _type = in_type;
        }
        ValueType _type;
        vector<Value> _array;
        multimap<string, Value> _object; // 用unorderd_map时需要用指针
                                         //根据标准键值可以重复
        unique_ptr<ValueData> _data;
    };
    enum ParseStatus
    {
        INVALID_VALUE,                     // 无效值
        EXPECT_VALUE,                      // 期望值
        ERROR_KEY_WORD,                    // 关键词错误
        ARRAY_INVALID_TOKEN_AFTER_BRACKET, //解析array时中括号之后遇到了无效的token
        ARRAY_INVALID_TOKEN_AFTER_COMMA,   //解析array时逗号之后遇到了无效的token
        ARRAY_INVALID_TOKEN_AFTER_VALUE,   //解析array时值之后遇到了无效的token
        OBJECT_INVALID_TOKEN_AFTER_BRACE,  //解析object时大括号之后遇到无效的token
        OBJECT_INVALID_TOKEN_AFTER_KEY,    //解析object时key之后遇到无效的token
        OBJECT_INVALID_TOKEN_AFTER_COLON,  // 解析object时:之后遇到无效的token
        OBJECT_INVALID_TOKEN_AFTER_VALUE,  // 解析object时值之后遇到无效的token
        OBJECT_INVAID_TOKEN_AFTER_COMMA,   // 解析object时逗号之后遇到武侠的token
        OK,
    };
    // 从字符串中读取。
    class Reader
    {
    public:
        Reader(const string &in_str, Value &in_val);
        ParseStatus parse();

    private:
        ParseStatus _parse_json(TokenStream &in_tokens, Value &in_value);
        void _parse_whitespace(TokenStream &in_tokens);
        ParseStatus _parse_null(TokenStream &in_tokens, Value &in_value);
        ParseStatus _parse_value(TokenStream &in_tokens, Value &in_value);
        ParseStatus _parse_boolen(TokenStream &in_tokens, Value &in_value);
        ParseStatus _parse_number(TokenStream &in_tokens, Value &in_value);
        ParseStatus _parse_string(TokenStream &in_tokens, Value &in_value);
        ParseStatus _parse_array(TokenStream &in_tokens, Value &in_value);
        ParseStatus _parse_object(TokenStream &in_tokens, Value &in_value);

        Value &_parsed_result;
        string _raw_str;
    };
}
#endif