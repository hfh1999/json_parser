/*只考虑处理c_orent string.*/
#include <iostream>
#include <json_parser.h>
#include <token.h>
#include <value_data.h>
using std::make_shared;

namespace Json
{
    void ParseStatusPrint(ParseStatus in_status)
    {
        switch (in_status)
        {
        case ParseStatus::INVALID_VALUE:
            printf("ParseStatus:invalid value in json string.\n");
            break;
        case ParseStatus::EXPECT_VALUE:
            printf("ParseStatus:expect a value.\n");
            break;
        case ParseStatus::ERROR_KEY_WORD:
            printf("ParseStaus: error key word,becase the tokenlize error.\n");
            break;
        case ParseStatus::ARRAY_INVALID_TOKEN_AFTER_BRACKET:
            printf("ParseStatus:array invalid token after bracket\n");
            break;
        case ParseStatus::ARRAY_INVALID_TOKEN_AFTER_COMMA:
            printf("ParseStatus:array invalid token after comma\n");
            break;
        case ParseStatus::ARRAY_INVALID_TOKEN_AFTER_VALUE:
            printf("ParseStatus:array invalid token after value\n");
            break;
        case ParseStatus::OBJECT_INVALID_TOKEN_AFTER_BRACE:
            printf("ParseStatus:object invalid token after brace.\n");
            break;
        case ParseStatus::OBJECT_INVALID_TOKEN_AFTER_KEY:
            printf("ParseStatus:object invalid token after key.\n");
            break;
        case ParseStatus::OBJECT_INVALID_TOKEN_AFTER_COLON:
            printf("ParseStatus:object invalid token after colon.\n");
            break;
        case ParseStatus::OBJECT_INVALID_TOKEN_AFTER_VALUE:
            printf("ParseStatus:object invalid token after value.\n");
            break;
        case ParseStatus::OBJECT_INVAID_TOKEN_AFTER_COMMA:
            printf("ParseStatus:object invalid token after comma.\n");
            break;
        case ParseStatus::OK:
            printf("ParseStatus:Ok.\n");
            break;
        default:
            printf("ParseStaus: Unknow error.\n");
            break;
        }
    }
    Value::Value() : _type(ValueType::INVALID_TYPE), _data(make_shared<ValueData>()) {}
    Value::Value(ValueType in_type) : _type(in_type), _data(make_shared<ValueData>()) {}
    Value::Value(ValueType in_type, double in_num) : _type(in_type), _data(make_shared<ValueData>(in_num)) {}
    Value::Value(ValueType in_type, const string &in_s) : _type(in_type), _data(make_shared<ValueData>(in_s)) {}
    Value::~Value() {}
    bool Value::isBool() const
    {
        return _type == ValueType::FALSE_TYPE ||
               _type == ValueType::TRUE_TYPE;
    }
    bool Value::isNull() const
    {
        return _type == ValueType::NULL_TYPE;
    }
    bool Value::isNumber() const
    {
        return _type == ValueType::NUMBER_TYPE;
    }
    bool Value::isString() const
    {
        return _type == ValueType::STRING_TYPE;
    }
    bool Value::isArray() const
    {
        return _type == ValueType::ARRAY_TYPE;
    }
    bool Value::isObject() const
    {
        return _type == ValueType::OBJECT_TYPE;
    }

    bool Value::asBool() const
    {
        assert(_type == ValueType::TRUE_TYPE || _type == ValueType::FALSE_TYPE);
        if (_type == ValueType::TRUE_TYPE)
            return true;
        else
            return false;
    }

    double Value::asDouble() const
    {
        assert(_type == ValueType::NUMBER_TYPE);
        return _ret_number();
    }

    string Value::asString() const
    {
        assert(_type == ValueType::STRING_TYPE);
        return _ret_string();
    }
    const Value &Value::operator[](ArrayIndex index) const
    {
        assert(_type == ValueType::ARRAY_TYPE);
        return (_data->array)[index];
    }
    Value &Value::operator[](ArrayIndex index)
    {
        return const_cast<Value &>(static_cast<const Value>(*this)[index]);
    }
    const Value &Value::operator[](const string &key) const
    {
        assert(_type == ValueType::OBJECT_TYPE);
        auto it = (_data->object).find(key);
        if (it == _data->object.end()) // 有更好的方式吗?
        {
            (_data->object).emplace(key, Value(ValueType::NULL_TYPE));
            it = (_data->object).find(key);
            return it->second;
        }
        return it->second;
    }
    Value &Value::operator[](const string &key)
    {
        return const_cast<Value &>(static_cast<const Value>(*this)[key]);
    }

    void Value::debug_print() const
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
    double Value::_ret_number() const
    {
        return _data->n;
    }
    string Value::_ret_string() const
    {
        return _data->s;
    }
    void Value::_push_value(Value &&in_v)
    {
        (_data->array).push_back(std::move(in_v));
    }
    void Value::_insert_object(const string &key, Value &&in_v)
    {
        (_data->object).emplace(key, std::move(in_v));
    }
    void Value::_set_type(ValueType in_type)
    {
        _type = in_type;
    }

    ParseStatus Reader::parse(const string &in_str, Value &in_val)
    {
        TokenStream tokens(in_str);
        if (tokens.getStatus() != TokensStatus::TOKENS_GOOD)
        {
            switch (tokens.getStatus())
            {
            case TokensStatus::EXPECT_KEY_ERROR:
                // printf("parse token error: expect key.\n");
                // break;
            case TokensStatus::NUMBER_FORMAT_ERROR:
                // printf("parse token error: number format error.\n");
                // break;
            case TokensStatus::STRING_BAD:
                // printf("parse token error: string bad.\n");
                // break;
            case TokensStatus::KEY_WORD_TRUE_ERROR:
            case TokensStatus::KEY_WORD_FALSE_ERROR:
            case TokensStatus::KEY_WORD_NULL_ERROR:
                break;
            default:
                printf("Unknow parse token error.\n");
                assert(false);
            }
            return ParseStatus::ERROR_KEY_WORD;
        }
        // printf("*****************\n");
        // tokens.debug_print();
        // printf("*****************\n");
        return _parse_json(tokens, in_val);
    }
    ParseStatus Reader::_parse_json(TokenStream &in_tokens, Value &in_value)
    {
        _parse_whitespace(in_tokens);
        return _parse_value(in_tokens, in_value);
    }

    ParseStatus Reader::_parse_value(TokenStream &in_tokens, Value &in_value)
    {
        switch (in_tokens.getToken().type)
        {
        case TokenType::NULL_TOKEN:
            return _parse_null(in_tokens, in_value);
        case TokenType::TRUE_TOKEN:
        case TokenType::FALSE_TOKEN:
            return _parse_boolen(in_tokens, in_value);
        case TokenType::NUMBER_TOKEN:
            return _parse_number(in_tokens, in_value);
        case TokenType::STRING_TOKEN:
            return _parse_string(in_tokens, in_value);
        case TokenType::LEFT_ARRAY_TOKEN:
            return _parse_array(in_tokens, in_value);
        case TokenType::LEFT_OBJECT_TOKEN:
            return _parse_object(in_tokens, in_value);
        default:
            return ParseStatus::INVALID_VALUE;
        }
    }

    void Reader::_parse_whitespace(TokenStream &in_tokens)
    {
        while (in_tokens.getToken().type == TokenType::WHITE_TOKEN)
        {
            in_tokens.next();
        }
    }

    ParseStatus Reader::_parse_null(TokenStream &in_tokens, Value &in_value)
    {
        in_value = Value(ValueType::NULL_TYPE);
        in_tokens.next();
        return ParseStatus::OK;
    }

    ParseStatus Reader::_parse_boolen(TokenStream &in_tokens, Value &in_value)
    {
        if (in_tokens.getToken().type == TokenType::TRUE_TOKEN)
        {
            in_value = Value(ValueType::TRUE_TYPE);
        }
        else
        {
            in_value = Value(ValueType::FALSE_TYPE);
        }
        in_tokens.next();
        return ParseStatus::OK;
    }

    ParseStatus Reader::_parse_number(TokenStream &in_tokens, Value &in_value)
    {
        auto token = in_tokens.getToken();
        /*将number token转换为double.*/
        double number = 0;
        //*todo*
        number = 0;

        in_value = Value(ValueType::NUMBER_TYPE, number);
        in_tokens.next();
        return ParseStatus::OK;
    }

    ParseStatus Reader::_parse_string(TokenStream &in_tokens, Value &in_value)
    {
        auto token = in_tokens.getToken();

        in_value = Value(ValueType::STRING_TYPE, token.str_data);
        in_tokens.next();
        return ParseStatus::OK;
    }

    bool __valid_tokens_for_value(TokenType in_type)
    {
        if (in_type == TokenType::FALSE_TOKEN ||
            in_type == TokenType::TRUE_TOKEN ||
            in_type == TokenType::NULL_TOKEN ||
            in_type == TokenType::NUMBER_TOKEN ||
            in_type == TokenType::STRING_TOKEN ||
            in_type == TokenType::LEFT_ARRAY_TOKEN ||
            in_type == TokenType::LEFT_OBJECT_TOKEN)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    ParseStatus Reader::_parse_array(TokenStream &in_tokens, Value &in_value)
    {
        in_value._set_type(ValueType::ARRAY_TYPE);
        bool flag = false; // 状态机，由于状态少直接用bool表示

        /*init status [*/
        in_tokens.next();

        _parse_whitespace(in_tokens);
        if (in_tokens.getToken().type == TokenType::RIGHT_ARRAY_TOKEN)
        {
            in_tokens.next();
            return ParseStatus::OK;
        }
        if (__valid_tokens_for_value(in_tokens.getToken().type))
        {
            flag = true;
        }
        else
        {
            // error:[之后遇到了无效token
            // printf("invalid token after [\n");
            // in_tokens.getToken().debug_print();
            in_value._set_type(ValueType::INVALID_TYPE);
            return ParseStatus::ARRAY_INVALID_TOKEN_AFTER_BRACKET;
        }

        while (true)
        {
            if (flag == false) // ','
            {
                /*do nothing else here*/
                in_tokens.next();

                _parse_whitespace(in_tokens); // 确保空白字符不影响解析

                if (__valid_tokens_for_value(in_tokens.getToken().type))
                {
                    flag = true;
                }
                else
                {
                    // error:逗号之后遇到了无效token
                    // printf("invalid token after ,\n");
                    // in_tokens.getToken().debug_print();
                    in_value._set_type(ValueType::INVALID_TYPE);
                    return ParseStatus::ARRAY_INVALID_TOKEN_AFTER_COMMA;
                }
            }
            else // value状态
            {
                Value tmp_value;
                ParseStatus ret_errorno = _parse_value(in_tokens, tmp_value);
                if (ret_errorno != ParseStatus::OK)
                    return ret_errorno;
                in_value._push_value(std::move(tmp_value)); // value没有实现copy函数，因其效率太低

                _parse_whitespace(in_tokens); // 确保空白字符不影响解析

                if (in_tokens.getToken().type == TokenType::COMMA_TOKEN)
                {
                    flag = false;
                }
                else if (in_tokens.getToken().type == TokenType::RIGHT_ARRAY_TOKEN)
                {
                    // 结束array解析
                    in_tokens.next();
                    return ParseStatus::OK;
                }
                else
                {
                    // error:value之后遇到了除',' and ']'的token
                    // printf("invalid token after value\n");
                    // in_tokens.getToken().debug_print();
                    in_value._set_type(ValueType::INVALID_TYPE);
                    return ParseStatus::ARRAY_INVALID_TOKEN_AFTER_VALUE;
                }
            }
        }
    }
    ParseStatus Reader::_parse_object(TokenStream &in_tokens, Value &in_value)
    {
        in_value._set_type(ValueType::OBJECT_TYPE);
        enum
        {
            LEFT_BRACE_STATUS,
            RIGHT_BRACE_STATUS,
            MEMBER_STATUS,
            COMMA_STATUS,
        } status;

        status = LEFT_BRACE_STATUS;

        /*deal {*/
        in_tokens.next();

        _parse_whitespace(in_tokens);

        // in_tokens.getToken().debug_print();
        if (in_tokens.getToken().type == TokenType::RIGHT_OBJECT_TOKEN)
        {
            in_tokens.next();
            return ParseStatus::OK;
        }
        else if (in_tokens.getToken().type == TokenType::STRING_TOKEN)
        {
            status = MEMBER_STATUS;
        }
        else
        {
            // error
            // printf("error 1\n");
            return ParseStatus::OBJECT_INVALID_TOKEN_AFTER_BRACE;
        }

        while (status == MEMBER_STATUS)
        {
            /*deal key here.*/
            string key = in_tokens.getToken().str_data; // copy here.
            // in_tokens.getToken().debug_print();
            in_tokens.next();

            _parse_whitespace(in_tokens);

            if (in_tokens.getToken().type != TokenType::COLON_TOKEN)
            {
                // error.
                // in_tokens.getToken().debug_print();
                // printf("error 2\n");
                return ParseStatus::OBJECT_INVALID_TOKEN_AFTER_KEY;
            }

            /*deal : here*/
            in_tokens.next();
            _parse_whitespace(in_tokens);
            if (!__valid_tokens_for_value(in_tokens.getToken().type))
            {
                // error.
                // printf("error 3\n");
                return ParseStatus::OBJECT_INVALID_TOKEN_AFTER_COLON;
            }

            /*deal value here.*/
            Value tmp_value;
            ParseStatus is_ok = _parse_value(in_tokens, tmp_value);
            if (is_ok != ParseStatus::OK)
            {
                return is_ok;
            }
            in_value._insert_object(key, std::move(tmp_value));

            _parse_whitespace(in_tokens);

            if (in_tokens.getToken().type == TokenType::COMMA_TOKEN)
            {
                status = COMMA_STATUS;
            }
            else if (in_tokens.getToken().type == TokenType::RIGHT_OBJECT_TOKEN)
            {
                status = RIGHT_BRACE_STATUS; // end.
                break;
            }
            else
            {
                // error
                // printf("error 4\n");
                // in_tokens.getToken().debug_print();
                return ParseStatus::OBJECT_INVALID_TOKEN_AFTER_VALUE;
            }

            /*deal ,*/
            // in_tokens.getToken().debug_print();
            in_tokens.next();

            _parse_whitespace(in_tokens);

            if (in_tokens.getToken().type == TokenType::STRING_TOKEN)
            {
                status = MEMBER_STATUS;
            }
            else
            {
                // error
                // printf("error 5\n");
                // in_tokens.getToken().debug_print();
                return ParseStatus::INVALID_VALUE;
            }
        }

        /*deal }*/
        assert(status == RIGHT_BRACE_STATUS);
        in_tokens.next();
        return ParseStatus::OK;
    }
    string Writer::WriteToString(const Value &in_v)
    {
        if(in_v.getType() == ValueType::INVALID_TYPE)
            return string();
        string strbuf;
        _stringify_value(in_v,strbuf);
        return strbuf;
    }
    void Writer::_stringify_value(const Value &in_v, string &in_s){
        switch (in_v.getType())
        {
        case ValueType::TRUE_TYPE:
            in_s.append("true");
            break;
        case ValueType::FALSE_TYPE:
            in_s.append("false");
            break;
        case ValueType::NULL_TYPE:
            in_s.append("null");
            break;
        case ValueType::NUMBER_TYPE:
            _stringify_number(in_v, in_s);
            break;
        case ValueType::STRING_TYPE:
            _stringify_string(in_v,in_s);
            break;
        case ValueType::ARRAY_TYPE:
            _stringify_array(in_v,in_s);
            break;
        case ValueType::OBJECT_TYPE:
            _stringify_object(in_v,in_s);
            break;
        default:
            assert(true);
        }
    }
    void  Writer::_stringify_number(const Value& in_v,string& in_s)
    {
        /*处理字符数字输出*/
        in_s.push_back('0');
    }
    void Writer::_stringify_string(const Value &in_v, string &in_s)
    {
        /*to do 处理转义符*/
        in_s.append(in_v._data->s);
    }
    void Writer::_stringify_array(const Value &in_v, string &in_s)
    {
        in_s.push_back('[');
        if((in_v._data->array).size() == 1)
        {
            _stringify_value(in_v,in_s);
        }
        else
        {
            for(auto& item:in_v._data->array)
            {
                _stringify_value(in_v,in_s);
                in_s.push_back(',');
            }
        }
        in_s.push_back(']');
    }
    void Writer::_stringify_object(const Value &in_v, string &in_s)
    {
        in_s.push_back('{');
        if((in_v._data->object).size() == 1)
        {
            in_s.append((*((in_v._data->object).begin())).first);
            in_s.append(":");
            _stringify_value(in_v,in_s);
        }
        else
        {
            for(auto& pair_item:in_v._data->object)
            {
                in_s.append(pair_item.first);
                in_s.append(":");
                _stringify_value(in_v,in_s);
                in_s.push_back(',');
            }
        }
        in_s.push_back('}');

    }
}