#include <token.h>
#include <cassert>
#include <cctype>
const int NULL_STR_LEN = 4;
const int TRUE_STR_LEN = 4; // len("true") == 4
const int FALSE_STR_LEN = 5;
void TokenStream::_split_to_tokens()
{
    Token tmp_token;
    string strbuf;
    bool flag = false; // 转义flag
    string::size_type index = 0;
    for (; index < _json_raw_str.size();)
    {
        char ch = _json_raw_str[index];
        switch (ch)
        {
        /*white token*/
        case ' ':
        case '\r':
        case '\n':
            tmp_token.type = TokenType::WHITE_TOKEN;
            _tokens.push_back(tmp_token);
            index += 1;
            break;

        /*left object token: {*/
        case '{':
            tmp_token.type = TokenType::LEFT_OBJECT_TOKEN;
            _tokens.push_back(tmp_token);
            index += 1;
            break;

        /*right object token: }*/
        case '}':
            tmp_token.type = TokenType::RIGHT_OBJECT_TOKEN;
            _tokens.push_back(tmp_token);
            index += 1;
            break;

        case ':':
            tmp_token.type = TokenType::COLON_TOKEN;
            _tokens.push_back(tmp_token);
            index += 1;
            break;

        /*left array token: [*/
        case '[':
            tmp_token.type = TokenType::LEFT_ARRAY_TOKEN;
            _tokens.push_back(tmp_token);
            index += 1;
            break;

        /*right array token: ]*/
        case ']':
            tmp_token.type = TokenType::RIGHT_ARRAY_TOKEN;
            _tokens.push_back(tmp_token);
            index += 1;
            break;

        /*comma token: ,*/
        case ',':
            tmp_token.type = TokenType::COMMA_TOKEN;
            _tokens.push_back(tmp_token);
            index += 1;
            break;

        /*null token*/
        case 'n':
            if (index + NULL_STR_LEN - 1 >= _json_raw_str.size())
            {
                _status = TokensStatus::KEY_WORD_NULL_ERROR;
                return;
            }
            if (_json_raw_str[index + 1] == 'u' &&
                _json_raw_str[index + 2] == 'l' &&
                _json_raw_str[index + 3] == 'l')
            {
                tmp_token.type = TokenType::NULL_TOKEN;
                _tokens.push_back(tmp_token);
                index += NULL_STR_LEN;
            }
            else
            {
                _status = TokensStatus::KEY_WORD_NULL_ERROR;
                return;
            }
            break;

        /*true token*/
        case 't':
            if (index + TRUE_STR_LEN - 1 >= _json_raw_str.size())
            {
                _status = TokensStatus::KEY_WORD_TRUE_ERROR;
                return;
            }
            if (_json_raw_str[index + 1] == 'r' &&
                _json_raw_str[index + 2] == 'u' &&
                _json_raw_str[index + 3] == 'e')
            {
                tmp_token.type = TokenType::TRUE_TOKEN;
                _tokens.push_back(tmp_token);
                index += TRUE_STR_LEN;
            }
            else
            {
                _status = TokensStatus::KEY_WORD_TRUE_ERROR;
                return;
            }
            break;

        /*false token*/
        case 'f':
            if (index + FALSE_STR_LEN - 1 >= _json_raw_str.size())
            {
                _status = TokensStatus::KEY_WORD_FALSE_ERROR;
                return;
            }
            if (_json_raw_str[index + 1] == 'a' &&
                _json_raw_str[index + 2] == 'l' &&
                _json_raw_str[index + 3] == 's' &&
                _json_raw_str[index + 4] == 'e')
            {
                tmp_token.type = TokenType::FALSE_TOKEN;
                _tokens.push_back(tmp_token);
                index += FALSE_STR_LEN;
            }
            else
            {
                _status = TokensStatus::KEY_WORD_FALSE_ERROR;
                return;
            }
            break;

        /*number*/
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            /*状态机*/
            enum NumParseStatus
            {
                INT_STATUS,      // 数字的整数部分,不为0
                INT_ZERO_STATUS, // 数字的整数部分为0
                DOT_STATUS,      // 小数点
                E_STATUS,        // e指数符号
                FRAC_STATUS,     // 小数部分
                ESIGN_STATUS,    // e的正负号
                EINT_STATUS,     // 指数的数值
                END_STATUS,      // 结束解析
            };
            // printf("enter number deal...\n");
            NumParseStatus status;
            NumData num_data;
            if (ch == '-') // 处理开始的负号
            {
                status = NumParseStatus::INT_STATUS;
                index += 1;
                num_data.is_minus = true;
            }
            else if (ch == '0') // 先导0
            {
                index += 1;
                num_data.int_part = 0;
                if (index >= _json_raw_str.size())
                {
                    status = NumParseStatus::END_STATUS;
                }
                else
                {
                    if (_json_raw_str[index] == '.')
                    {
                        status = NumParseStatus::DOT_STATUS;
                    }
                    else if (_json_raw_str[index] == 'e' || _json_raw_str[index] == 'E')
                    {
                        status = NumParseStatus::E_STATUS;
                    }
                    else if (isdigit(_json_raw_str[index])) // 是数字则错误
                    {
                        // error.
                        _status = TokensStatus::NUMBER_FORMAT_ERROR;
                        return;
                    }
                    else
                    {
                        status = NumParseStatus::END_STATUS;
                    }
                }
            }
            else
            {
                status = NumParseStatus::INT_STATUS;
                num_data.is_minus = false;
            }

            // 要注意整数部分的先导0是不合法的
            if (status == NumParseStatus::INT_STATUS)
            {
                num_data.int_part = 0;
                if (index >= _json_raw_str.size() || !isdigit(_json_raw_str[index]))
                {
                    // 不应该什么都不干就结束
                    _status = TokensStatus::NUMBER_FORMAT_ERROR;
                    return;
                }

                while (index < _json_raw_str.size() && isdigit(_json_raw_str[index]))
                {
                    num_data.int_part = num_data.int_part * 10 + _json_raw_str[index] - '0';
                    index += 1;
                }
                if (index >= _json_raw_str.size())
                {
                    status = NumParseStatus::END_STATUS;
                }
                else
                {
                    if (_json_raw_str[index] == 'e' || _json_raw_str[index] == 'E')
                    {
                        status = NumParseStatus::E_STATUS;
                    }
                    else if (_json_raw_str[index] == '.')
                    {
                        status = NumParseStatus::DOT_STATUS;
                    }
                    else
                    {
                        status = NumParseStatus::END_STATUS;
                    }
                }
            }

            if (status == NumParseStatus::DOT_STATUS)
            {
                index += 1;
                if (index >= _json_raw_str.size())
                {
                    // printf("num parse error:dot with nothing.\n");
                    _status = TokensStatus::NUMBER_FORMAT_ERROR;
                    return;
                }

                if (isdigit(_json_raw_str[index]))
                {
                    status = NumParseStatus::FRAC_STATUS;
                }
                else
                {
                    // printf("num parse error:dot with unexpect char.\n");
                    _status = TokensStatus::NUMBER_FORMAT_ERROR;
                    return;
                }
            }

            if (status == NumParseStatus::FRAC_STATUS)
            {
                num_data.frac_part = 0;
                while (index < _json_raw_str.size() && isdigit(_json_raw_str[index]))
                {
                    num_data.frac_part = num_data.frac_part * 10 + _json_raw_str[index] - '0';
                    index += 1;
                }
                if (index >= _json_raw_str.size())
                {
                    status = NumParseStatus::END_STATUS;
                }
                else
                {
                    if (_json_raw_str[index] == 'e' || _json_raw_str[index] == 'E')
                    {
                        status = NumParseStatus::E_STATUS;
                    }
                    else
                    {
                        status = NumParseStatus::END_STATUS;
                    }
                }
            }
            if (status == NumParseStatus::E_STATUS)
            {
                index += 1;
                if (index >= _json_raw_str.size())
                {
                    // printf("num parse error:e char with nothing.\n");
                    _status = TokensStatus::NUMBER_FORMAT_ERROR;
                    return;
                }
                if (_json_raw_str[index] == '+' || _json_raw_str[index] == '-')
                {
                    status = NumParseStatus::ESIGN_STATUS;
                }
                else if (isdigit(_json_raw_str[index]))
                {
                    num_data.exp_sign = true;
                    status = NumParseStatus::E_STATUS;
                }
                else
                {
                    // printf("num parse error:e char with unexpect char -> %c.\n",_json_raw_str[index]);
                    _status = TokensStatus::NUMBER_FORMAT_ERROR;
                    return;
                }
            }

            if (status == NumParseStatus::ESIGN_STATUS)
            {
                if (_json_raw_str[index] == '+')
                {
                    num_data.exp_sign = true;
                }
                else
                {
                    num_data.exp_sign = false;
                }
                index += 1;
                if (index >= _json_raw_str.size())
                {
                    // printf("num parse error:e sign with nothing.\n");
                    _status = TokensStatus::NUMBER_FORMAT_ERROR;
                    return;
                }
                if (isdigit(_json_raw_str[index]))
                {
                    status = NumParseStatus::EINT_STATUS;
                }
                else
                {
                    // printf("num parse error:e sign with unexpect char.\n");
                    _status = TokensStatus::NUMBER_FORMAT_ERROR;
                    return;
                }
            }
            if (status == NumParseStatus::EINT_STATUS)
            {
                num_data.exp_part = 0;
                while (index < _json_raw_str.size() && isdigit(_json_raw_str[index]))
                {
                    num_data.exp_part = num_data.exp_part * 10 + _json_raw_str[index] - '0';
                    index += 1;
                }
                status = NumParseStatus::END_STATUS;
            }

            if (status == NumParseStatus::END_STATUS)
            {
                tmp_token.type = TokenType::NUMBER_TOKEN;
                tmp_token.num_data = num_data;
                _tokens.push_back(tmp_token);
            }

            // 不应该出现
            assert(false);
            break;

        /*字符串*/
        case '\"':
            index += 1;
            for (; index <= _json_raw_str.size(); index++)
            {
                char tmp_ch = _json_raw_str[index];
                if (flag == false)
                {
                    if (tmp_ch == '\"') // should index++
                    {
                        index += 1;
                        break;
                    }

                    if (tmp_ch == '\\')
                    {
                        flag = true;
                        continue;
                    }
                    strbuf.push_back(tmp_ch);
                }
                else
                {
                    switch (tmp_ch)
                    {
                    case '\"':
                        strbuf.push_back('\"');
                        break;
                    case '\\':
                        strbuf.push_back('\\');
                        break;
                    case '/':
                        strbuf.push_back('/'); // ???
                        break;
                    case 'b':
                        strbuf.push_back('\b');
                        break;
                    case 'f':
                        strbuf.push_back('\f');
                        break;
                    case 'n':
                        strbuf.push_back('\n');
                        break;
                    case 'r':
                        strbuf.push_back('\r');
                        break;
                    case 't':
                        strbuf.push_back('\t');
                        break;

                        /*to do*/
                        // \uxxxx 转义为utf8编码

                    default:
                        _status = TokensStatus::STRING_BAD;
                        return;
                    }
                    flag = false;
                }
            }
            tmp_token.type = TokenType::STRING_TOKEN;
            tmp_token.str_data = strbuf;
            _tokens.push_back(tmp_token);
            break;

        /*剩下的情况都是tokenlize出错的情况*/
        default:
            _status = TokensStatus::EXPECT_KEY_ERROR;
            return;
        }
    }
    tmp_token.type = TokenType::END_TOKEN;
    _tokens.push_back(tmp_token);
}
