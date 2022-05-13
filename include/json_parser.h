# include <string>
using std::string;

class TokenStream;
namespace Json{
    // 表示解析过后的数据
    class Value{

    };
    // 从字符串中读取。
    class Reader{
        public:
        Reader(const string& in_str,Value& in_val);
        private:
        void _parse_json(TokenStream& in_tokens);
    };
}