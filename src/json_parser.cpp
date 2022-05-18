/*只考虑处理c_orent string.*/
#include <iostream>
#include <json_parser.h>
#include <token.h>

namespace Json
{
    Reader::Reader(const string &in_str, Value &in_val) : _raw_str(in_str), _parsed_result(in_val)
    {
    }

    ParseError Reader::parse()
    {
        TokenStream tokens(_raw_str);
        if (tokens.getStatus() != TokensStatus::TOKENS_GOOD)
        {
            switch (tokens.getStatus())
            {
            case TokensStatus::EXPECT_KEY_ERROR:
                printf("parse token error: expect key.\n");
                break;
            case TokensStatus::KEY_WORD_ERROR:
                printf("parse token error: key word not correct.\n");
                break;
            case TokensStatus::NUMBER_FORMAT_ERROR:
                printf("parse token error: number format error.\n");
                break;
            case TokensStatus::STRING_BAD:
                printf("parse token error: string bad.\n");
                break;
            default:
                printf("Unknow parse token error.\n");
            }
            return ParseError::ERROR_KEY_WORD;
        }
        //printf("*****************\n");
        //tokens.debug_print();
        //printf("*****************\n");
        return _parse_json(tokens, _parsed_result);
    }
    ParseError Reader::_parse_json(TokenStream &in_tokens, Value &in_value)
    {
        _parse_whitespace(in_tokens);
        return _parse_value(in_tokens, in_value);
    }

    ParseError Reader::_parse_value(TokenStream &in_tokens, Value &in_value)
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
            return _parse_array(in_tokens,in_value);
        default:
            return ParseError::INVALID_VALUE;
        }
    }

    void Reader::_parse_whitespace(TokenStream &in_tokens)
    {
        while (in_tokens.getToken().type == TokenType::WHITE_TOKEN)
        {
            in_tokens.next();
        }
    }

    ParseError Reader::_parse_null(TokenStream &in_tokens, Value &in_value)
    {
        in_value = Value(ValueType::NULL_TYPE);
        in_tokens.next();
        return ParseError::OK;
    }

    ParseError Reader::_parse_boolen(TokenStream &in_tokens, Value &in_value)
    {
        if (in_tokens.getToken().type == TokenType::TRUE_TOKEN)
        {
            in_value = Value(ValueType::TRUE_TYPE);
            in_tokens.next();
            return ParseError::OK;
        }
        else
        {
            in_value = Value(ValueType::FALSE_TYPE);
            in_tokens.next();
            return ParseError::OK;
        }
    }

    ParseError Reader::_parse_number(TokenStream &in_tokens, Value &in_value)
    {
        auto token = in_tokens.getToken();
        /*将number token转换为double.*/
        double number = 0;
        //*todo*

        in_value = Value(ValueType::NUMBER_TYPE, number);
        in_tokens.next();
        return ParseError::OK;
    }

    ParseError Reader::_parse_string(TokenStream &in_tokens, Value &in_value)
    {
        auto token = in_tokens.getToken();

        in_value = Value(ValueType::STRING_TYPE,token.str_data);
        in_tokens.next();
        return ParseError::OK;
    }

    ParseError Reader::_parse_array(TokenStream &in_tokens,Value& in_value)
    {
        in_value.set_type(ValueType::ARRAY_TYPE);
        bool flag = false;// 状态机，由于状态少直接用bool表示
        while(in_tokens.getToken().type != TokenType::END_TOKEN)
        {
            if(flag == false)// ',' 或 '['(初始) 状态
            {
                /*do nothing else here*/
                in_tokens.next();

                _parse_whitespace(in_tokens);// 确保空白字符不影响解析

                if (in_tokens.getToken().type == TokenType::FALSE_TOKEN ||
                    in_tokens.getToken().type == TokenType::TRUE_TOKEN ||
                    in_tokens.getToken().type == TokenType::NULL_TOKEN ||
                    in_tokens.getToken().type == TokenType::NUMBER_TOKEN ||
                    in_tokens.getToken().type == TokenType::STRING_TOKEN ||
                    in_tokens.getToken().type == TokenType::LEFT_ARRAY_TOKEN)
                {
                    flag = true;
                }
                else
                {
                    //error:逗号之后遇到了无效token
                    printf("invalid token after ,\n");
                    in_tokens.getToken().debug_print();
                    in_value.set_type(ValueType::INVALID_TYPE);
                    return ParseError::INVALID_VALUE;
                }
            }
            else // value状态
            {
                Value tmp_value;
                ParseError ret_errorno = _parse_value(in_tokens,tmp_value);
                if(ret_errorno != ParseError::OK)
                    return ret_errorno;
                in_value.push_value(std::move(tmp_value)); // value没有实现copy函数，因其效率太低

                _parse_whitespace(in_tokens);// 确保空白字符不影响解析

                if(in_tokens.getToken().type == TokenType::COMMA_TOKEN)
                {
                    flag = false;
                }
                else if(in_tokens.getToken().type == TokenType::RIGHT_ARRAY_TOKEN)
                {
                    // 结束array解析
                    return ParseError::OK;
                }
                else
                {
                    //error:value之后遇到了除',' and ']'的token
                    printf("invalid token after value\n");
                    in_tokens.getToken().debug_print();
                    in_value.set_type(ValueType::INVALID_TYPE);
                    return ParseError::INVALID_VALUE;
                }
            }
       }

        return ParseError::OK;
    }
}