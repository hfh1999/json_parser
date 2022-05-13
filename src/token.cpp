#include <token.h>
#include <iostream>
const int NULL_STR_LEN = 4;
const int TRUE_STR_LEN = 4; // len("true") == 4
const int FALSE_STR_LEN = 5;
void TokenStream::_split_to_tokens()
{
    Token tmp_token;
    string::size_type index = 0;
    for (; index < _json_raw_str.size();)
    {
        switch (_json_raw_str[index])
        {
        /*white token*/
        case ' ':
        case '\r':
        case '\n':
            tmp_token.data = "";
            tmp_token.type = TokenType::WHITE_TOKEN;
            _tokens.push_back(tmp_token);
            index += 1;
            break;

        /*null token*/
        case 'n':
            if (index + NULL_STR_LEN - 1 >= _json_raw_str.size())
            {
                _status = TokensStatus::KEY_WORD_ERROR;
                return;
            }
            if (_json_raw_str[index + 1] == 'u' &&
                _json_raw_str[index + 2] == 'l' &&
                _json_raw_str[index + 3] == 'l')
            {
                tmp_token.data = "null";
                tmp_token.type = TokenType::NULL_TOKEN;
                _tokens.push_back(tmp_token);
                index += NULL_STR_LEN;
            }
            else
            {
                _status = TokensStatus::KEY_WORD_ERROR;
                return;
            }
            break;

        /*true token*/
        case 't':
            if (index + TRUE_STR_LEN - 1 >= _json_raw_str.size())
            {
                _status = TokensStatus::KEY_WORD_ERROR;
                return;
            }
            if (_json_raw_str[index + 1] == 'r' &&
                _json_raw_str[index + 2] == 'u' &&
                _json_raw_str[index + 3] == 'e')
            {
                tmp_token.data = "true";
                tmp_token.type = TokenType::TRUE_TOKEN;
                _tokens.push_back(tmp_token);
                index += TRUE_STR_LEN;
            }
            else
            {
                _status = TokensStatus::KEY_WORD_ERROR;
                return;
            }
            break;

        /*false token*/
        case 'f':
            if (index + FALSE_STR_LEN - 1 >= _json_raw_str.size())
            {
                _status = TokensStatus::KEY_WORD_ERROR;
                return;
            }
            if (_json_raw_str[index + 1] == 'a' &&
                _json_raw_str[index + 2] == 'l' &&
                _json_raw_str[index + 3] == 's' &&
                _json_raw_str[index + 4] == 'e')
            {
                tmp_token.data = "false";
                tmp_token.type = TokenType::FALSE_TOKEN;
                _tokens.push_back(tmp_token);
                index += FALSE_STR_LEN;
            }
            break;

        /*剩下的情况都是tokenlize出错的情况*/
        default:
            _status = TokensStatus::KEY_WORD_ERROR;
            return;
        }
    }
    tmp_token.data = "";
    tmp_token.type = TokenType::END_TOKEN;
    _tokens.push_back(tmp_token);
}
