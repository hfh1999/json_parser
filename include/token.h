#ifndef TOKEN_H
#define TOKEN_H
#include <string>
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
    KEY_WORD_ERROR,      // 关键词书写错误，不是null,true,false部分写对
    NUMBER_FORMAT_ERROR, // 数字格式错误
    EXPECT_KEY_ERROR,    // 不是预期中的关键词
    TOKENS_GOOD,         // token状态完好
    STRING_BAD,          // string解析错误
};
struct NumData
{
    bool is_minus;
    uint32_t int_part;
    uint32_t frac_part;
    bool exp_sign; // + -> true
    int32_t exp_part;
};
struct Token
{
    TokenType type;
    NumData num_data;
    string str_data;
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

private:
    string _json_raw_str;
    TokensStatus _status;
    vector<Token>::size_type _pos;
    vector<Token> _tokens;

    /*若出错会设置_status出错状态*/
    void _split_to_tokens();
};
#endif