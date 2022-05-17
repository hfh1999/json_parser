/*只考虑处理c_orent string.*/
#include <iostream>
#include <json_parser.h>
#include <token.h>

namespace Json
{
    Reader::Reader(const string &in_str, Value &in_val):_raw_str(in_str),_parsed_result(in_val)
    {
    }
    
    ParseError Reader::parse()
    {
        TokenStream tokens(_raw_str);
        if (tokens.getStatus() != TokensStatus::TOKENS_GOOD)
        {
            switch(tokens.getStatus())
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
                default:
                printf("Unknow parse token error.\n");
            }
            return ParseError::ERROR_KEY_WORD;
        }
        return _parse_json(tokens,_parsed_result);
    }
    ParseError Reader::_parse_json(TokenStream &in_tokens,Value& in_value)
    {
        _parse_whitespace(in_tokens);
        return _parse_value(in_tokens,in_value);
    }

    ParseError Reader::_parse_value(TokenStream &in_tokens,Value& in_value)
    {
        switch (in_tokens.getToken().type)
        {
        case TokenType::NULL_TOKEN:
            return _parse_null(in_tokens,in_value);
        case TokenType::TRUE_TOKEN:
        case TokenType::FALSE_TOKEN:
            return _parse_boolen(in_tokens,in_value);
        case TokenType::NUMBER_TOKEN:
            return _parse_number(in_tokens,in_value);
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

    ParseError Reader::_parse_null(TokenStream &in_tokens,Value& in_value){
        in_value = Value(ValueType::NULL_TYPE);
        in_tokens.next();
        return ParseError::OK; 
    }

    ParseError Reader::_parse_boolen(TokenStream &in_tokens,Value& in_value)
    {
        if(in_tokens.getToken().type == TokenType::TRUE_TOKEN)
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

        in_value = Value(ValueType::NUMBER_TYPE,number);
        in_tokens.next();
        return ParseError::OK;
    }
}