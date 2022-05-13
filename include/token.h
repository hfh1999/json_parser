# include <string>
# include <vector>
using std::vector;
using std::string;
enum TokenType{
    NUMBER_TOKEN,
    STRING_TOKEN,
    BOOLEN_TOKEN,
    NULL_TOKEN,
    LEFT_OBJECT_TOKEN,// {
    RIGHT_OBJECT_TOKEN,// }
    LEFT_ARRAY_TOKEN,// [
    RIGHT_ARRAY_TOKEN,// ]
    COMMA_TOKEN,//,
    COLON_TOKEN,// :
    END_TOKEN,// END
};
enum TokensStatus{
    KEY_WORD_ERROR,// 关键词书写错误，不是null,true,false,中的任何一个
    TOKENS_GOOD,// token状态完好
};
struct Token{
    TokenType type;
    string data;
};
class TokenStream{
    public:
    TokenStream(const string& in_str):_json_raw_str(in_str),_status()
    {
        _status = _split_to_tokens();
    }
    vector<Token>::const_iterator begin(){
        return _tokens.cbegin();
    }
    vector<Token>::const_iterator end(){
        return _tokens.cend();
    }
    TokensStatus getStatus(){
        return _status;
    }
    private:
    string _json_raw_str;
    TokensStatus _status;
    vector<Token> _tokens;
    TokensStatus _split_to_tokens()
    {

    }
};