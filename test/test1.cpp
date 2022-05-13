#include <json_parser.h>
int main(){
    Json::Value v;
    Json::Reader reader2("hello",v);
    say_hello();
    return 0;
}