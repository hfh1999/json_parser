/*只考虑处理c_orent string.*/
#include <iostream>
#include <json_parser.h>
#include <token.h>

namespace Json
{
    Reader::Reader(const string &in_str, Value &in_val) : _raw_str(in_str), _parsed_result(in_val)
    {
    }

    ParseStatus Reader::parse()
    {
        TokenStream tokens(_raw_str);
        if (tokens.getStatus() != TokensStatus::TOKENS_GOOD)
        {
            switch (tokens.getStatus())
            {
            case TokensStatus::EXPECT_KEY_ERROR:
                //printf("parse token error: expect key.\n");
                //break;
            case TokensStatus::NUMBER_FORMAT_ERROR:
                //printf("parse token error: number format error.\n");
                //break;
            case TokensStatus::STRING_BAD:
                //printf("parse token error: string bad.\n");
                //break;
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
        return _parse_json(tokens, _parsed_result);
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
        in_value.set_type(ValueType::ARRAY_TYPE);
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
            //printf("invalid token after [\n");
            //in_tokens.getToken().debug_print();
            in_value.set_type(ValueType::INVALID_TYPE);
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
                    //printf("invalid token after ,\n");
                    //in_tokens.getToken().debug_print();
                    in_value.set_type(ValueType::INVALID_TYPE);
                    return ParseStatus::ARRAY_INVALID_TOKEN_AFTER_COMMA;
                }
            }
            else // value状态
            {
                Value tmp_value;
                ParseStatus ret_errorno = _parse_value(in_tokens, tmp_value);
                if (ret_errorno != ParseStatus::OK)
                    return ret_errorno;
                in_value.push_value(std::move(tmp_value)); // value没有实现copy函数，因其效率太低

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
                    //printf("invalid token after value\n");
                    //in_tokens.getToken().debug_print();
                    in_value.set_type(ValueType::INVALID_TYPE);
                    return ParseStatus::ARRAY_INVALID_TOKEN_AFTER_VALUE;
                }
            }
        }
    }
    ParseStatus Reader::_parse_object(TokenStream &in_tokens, Value &in_value)
    {
        in_value.set_type(ValueType::OBJECT_TYPE);
        enum
        {
            LEFT_BRACE_STATUS,
            RIGHT_BRACE_STATUS,
            MEMBER_STATUS,
            COMMA_STATUS,
        } status;

        status = LEFT_BRACE_STATUS;
        in_tokens.next();

        //in_tokens.getToken().debug_print();
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

            //in_tokens.getToken().debug_print();
            in_tokens.next();
            if (in_tokens.getToken().type != TokenType::COLON_TOKEN)
            {
                // error.
                //in_tokens.getToken().debug_print();
                //printf("error 2\n");
                return ParseStatus::OBJECT_INVALID_TOKEN_AFTER_KEY;
            }

            /*deal : here*/
            in_tokens.next();
            if (!__valid_tokens_for_value(in_tokens.getToken().type))
            {
                // error.
                //printf("error 3\n");
                return ParseStatus::OBJECT_INVALID_TOKEN_AFTER_COLON;
            }

            /*deal value here.*/
            Value tmp_value;
            ParseStatus is_ok = _parse_value(in_tokens, tmp_value);
            if (is_ok != ParseStatus::OK)
            {
                return is_ok;
            }
            in_value.insert_object(key, std::move(tmp_value));
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
                //printf("error 4\n");
                //in_tokens.getToken().debug_print();
                return ParseStatus::OBJECT_INVALID_TOKEN_AFTER_VALUE;
            }

            /*deal ,*/
            //in_tokens.getToken().debug_print();
            in_tokens.next();
            if (in_tokens.getToken().type == TokenType::STRING_TOKEN)
            {
                status = MEMBER_STATUS;
            }
            else
            {
                // error
                //printf("error 5\n");
                //in_tokens.getToken().debug_print();
                return ParseStatus::INVALID_VALUE;
            }
        }

        /*deal }*/
        in_tokens.next();
        return ParseStatus::OK;
    }
}