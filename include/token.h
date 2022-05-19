#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <iostream>
#include <vector>
using std::string;
using std::vector;
enum TokenType
{
    NUMBER_TOKEN,
    STRING_TOKEN,
    TRUE_TOKEN,
    FALSE_TOKEN,
    NULL_TOKEN,
    LEFT_OBJECT_TOKEN,  // {
    RIGHT_OBJECT_TOKEN, // }
    LEFT_ARRAY_TOKEN,   // [
    RIGHT_ARRAY_TOKEN,  // ]
    COMMA_TOKEN,        //,
    COLON_TOKEN,        // :
    WHITE_TOKEN,        // space,\r,\n...
    END_TOKEN,          // END
};
enum TokensStatus
{
    KEY_WORD_NULL_ERROR,//关键词null拼写错误
    KEY_WORD_TRUE_ERROR,//关键词true拼写错误
    KEY_WORD_FALSE_ERROR,//关键词false拼写错误
    NUMBER_FORMAT_ERROR, // 数字格式错误
    EXPECT_KEY_ERROR,    // 不是预期中的关键词
    TOKENS_GOOD,         // token状态完好
    STRING_BAD,          // string解析错误
};
struct NumData
{
    bool is_minus;
    uint64_t int_part;
    uint64_t frac_part;
    bool exp_sign; // + -> true
    uint64_t exp_part;
};
struct Token
{
    TokenType type;
    NumData num_data;
    string str_data;
    void debug_print()
    {
        switch (type)
        {
        case NUMBER_TOKEN:
            printf("number token.\n");
            break;
        case STRING_TOKEN:
            printf("string token.\n");
            break;
        case TRUE_TOKEN:
            printf("true token.\n");
            break;
        case FALSE_TOKEN:
            printf("false token.\n");
            break;
        case NULL_TOKEN:
            printf("null token.\n");
            break;
        case LEFT_OBJECT_TOKEN:
            printf("left_object token.\n");
            break;
        case RIGHT_OBJECT_TOKEN:
            printf("right_object token.\n");
            break;
        case LEFT_ARRAY_TOKEN:
            printf("left_array token.\n");
            break;
        case RIGHT_ARRAY_TOKEN:
            printf("right_array token.\n");
            break;
        case COMMA_TOKEN:
            printf("comma token.\n");
            break;
        case COLON_TOKEN:
            printf("colon token.\n");
            break;
        case WHITE_TOKEN:
            printf("white token.\n");
            break;
        case END_TOKEN:
            printf("end token.\n");
            break;
        }
    }
};
class TokenStream
{
public:
    TokenStream(const string &in_str) : _json_raw_str(in_str), _status(TokensStatus::TOKENS_GOOD), _pos(0)
    {
        _split_to_tokens();
    }
    // vector<Token>::const_iterator begin(){
    //     return _tokens.cbegin();
    // }
    // vector<Token>::const_iterator end(){
    //     return _tokens.cend();
    // }
    Token getToken()
    {
        return _tokens[_pos];
    }
    void next()
    {
        _pos = _pos + 1;
    }
    TokensStatus getStatus()
    {
        return _status;
    }
    void debug_print()
    {
        for(auto item :_tokens)
        {
            item.debug_print();
        }
    }

private:
    string _json_raw_str;
    TokensStatus _status;
    vector<Token>::size_type _pos;
    vector<Token> _tokens;

    /*若出错会设置_status出错状态*/
    void _split_to_tokens();
};
#endif