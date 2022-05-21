#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include <string>
#include <cassert>
#include <vector>
#include <memory>
#include <map>
using std::multimap;
using std::shared_ptr;
using std::string;
using std::vector;

class TokenStream;
class ValueData; // 在相对应的命名空间进行申明
// unique_ptr对于不完整类型需要在别处处理其析构函数
// 一旦定义了自己的析构函数则移动构造函数将不会被编译器自动产生
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
        typedef vector<Value>::size_type ArrayIndex;
        Value();
        Value(ValueType in_type);
        Value(ValueType in_type, double in_num);
        Value(ValueType in_type, const string &in_s);
        ~Value();

        bool isBool() const;
        bool isNull() const;
        bool isNumber() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;

        bool asBool() const;
        double asDouble() const;
        string asString() const;

        const Value &operator[](ArrayIndex index) const;  // vist array const;
        Value &operator[](ArrayIndex index);              // vist array;
        const Value &operator[](const string &key) const; // vist object const;
        Value &operator[](const string &key);             // vist object;

        ValueType getType() const
        {
            return _type;
        }

        void debug_print() const;

    private:
        void _set_type(ValueType in_type);
        ValueType _type;

        double _ret_number() const;
        string _ret_string() const;
        void _push_value(Value &&in_v);                       // for _data -> array
        void _insert_object(const string &key, Value &&in_v); // for _data -> object
        shared_ptr<ValueData> _data;
    };
    enum ParseStatus
    {
        INVALID_VALUE,                     //无效值
        EXPECT_VALUE,                      //期望值
        ERROR_KEY_WORD,                    //关键词错误
        ARRAY_INVALID_TOKEN_AFTER_BRACKET, //解析array时中括号之后遇到了无效的token
        ARRAY_INVALID_TOKEN_AFTER_COMMA,   //解析array时逗号之后遇到了无效的token
        ARRAY_INVALID_TOKEN_AFTER_VALUE,   //解析array时值之后遇到了无效的token
        OBJECT_INVALID_TOKEN_AFTER_BRACE,  //解析object时大括号之后遇到无效的token
        OBJECT_INVALID_TOKEN_AFTER_KEY,    //解析object时key之后遇到无效的token
        OBJECT_INVALID_TOKEN_AFTER_COLON,  //解析object时:之后遇到无效的token
        OBJECT_INVALID_TOKEN_AFTER_VALUE,  //解析object时值之后遇到无效的token
        OBJECT_INVAID_TOKEN_AFTER_COMMA,   //解析object时逗号之后遇到武侠的token
        OK,
    };
    void ParseStatusPrint(ParseStatus in_status);
    // 从字符串中读取。
    class Reader
    {
    public:
        static ParseStatus parse(const string &in_str, Value &in_val);

    private:
        Reader() = default;
        static ParseStatus _parse_json(TokenStream &in_tokens, Value &in_value);
        static void _parse_whitespace(TokenStream &in_tokens);
        static ParseStatus _parse_null(TokenStream &in_tokens, Value &in_value);
        static ParseStatus _parse_value(TokenStream &in_tokens, Value &in_value);
        static ParseStatus _parse_boolen(TokenStream &in_tokens, Value &in_value);
        static ParseStatus _parse_number(TokenStream &in_tokens, Value &in_value);
        static ParseStatus _parse_string(TokenStream &in_tokens, Value &in_value);
        static ParseStatus _parse_array(TokenStream &in_tokens, Value &in_value);
        static ParseStatus _parse_object(TokenStream &in_tokens, Value &in_value);
    };
}
#endif