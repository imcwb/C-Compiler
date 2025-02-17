#include<string>
enum class TokenType {

};

class Token {
private:
    std::string tokenName;
    TokenType tokenType;
public:
    Token(std::string name, TokenType type) {
        tokenName = name;
        tokenType = type;
    }; 
};