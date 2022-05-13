/*只考虑处理c_orent string.*/
#include <iostream>
#include <json_parser.h>
#include <token.h>

Json::Reader::Reader(const string& in_str,Value& in_val){
    TokenStream tokens(in_str); // token流
    if(tokens.getStatus() != TokensStatus::TOKENS_GOOD)
    {
        printf("Parse Error when parse tokens.\n");
        return;
    }
    _parse_json(tokens);
}
void Json::Reader::_parse_json(TokenStream& in_tokens){
    
}