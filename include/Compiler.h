#include <string>
class Compiler {
private:
    std::string fileName;
public:
    Compiler(int argc, const char *argv[]) {
        parseArgs(argc, argv);
    };

    void parseArgs(int argc, const char *argv[]) {
        for (int i = 0; i < argc; i++) {
            std::string s = argv[i];
            if (s.rfind(".") != std::string::npos && s.substr(s.rfind("."), s.length()) == ".cpp") {
                fileName = s;
            }
        }
    };
};
