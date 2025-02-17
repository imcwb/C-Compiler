#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <functional>

struct ProduceExpr{
    std::vector<std::string> lhs;
    std::vector<std::string> rhs;
    int next;

    bool operator==(const ProduceExpr& other) const {
        if (this->lhs[0] != other.lhs[0])
            return false;
        if (this->rhs.size() != other.rhs.size())
            return false;
        for (int i = 0; i < this->rhs.size(); i++) {
            if (this->rhs[i] != other.rhs[i])
                return false;
        }
        return true;
    };

    void dump() {
        std::cout << lhs[0];
        std::cout << " -> ";
        for (auto E : rhs) {
            std::cout << E;
            std::cout << " ";
        }
        std::cout << std::endl;
    };
};

bool tokenCompare(const std::string &S1, const std::string &S2) {
    return S1 < S2;
};

struct Grammar {
    std::vector<ProduceExpr> grammar;
    std::set<std::string> tSymbol;
    std::set<std::string> ntSymbol;
    std::map<std::string, std::set<std::string>> firstSet;
    std::map<std::string, std::set<std::string>> followSet;

    void dump() {
        std::cout << "==========dump prod ===========" << std::endl;
        for (auto &P : grammar) {
            P.dump();
        }
        std::cout << "=========dump tSymbol =========" << std::endl;
        for (auto &TS : tSymbol) {
            std::cout << TS << std::endl;
        }
        std::cout << "=========dump ntSymbol =========" << std::endl;
        for (auto &NTS : ntSymbol) {
            std::cout << NTS << std::endl;
        }
        std::cout << "=========dump fisrt set ========" << std::endl;
        for (auto &S : tSymbol) {
            std::cout << S << " : ";
            for (auto &SS : firstSet[S]) {
                std::cout << SS << " ";
            }
            std::cout << std::endl;
        }
        for (auto &S : ntSymbol) {
            std::cout << S << " : ";
            for (auto &SS : firstSet[S]) {
                std::cout << SS << " ";
            }
            std::cout << std::endl;            
        }
        std::cout << "=========dump follow set ========" << std::endl;
        for (auto &S : ntSymbol) {
            std::cout << S << " : ";
            for (auto &SS : followSet[S]) {
                std::cout << SS << " ";
            }
            std::cout << std::endl;            
        }
    };
};

bool isTSymbol(std::string &token, Grammar &g) {
    for (auto &S : g.tSymbol) {
        if (S == token)
            return true;
    }
    return false;
}

bool isNTSymbol(std::string &token, Grammar &g) {
    if (isTSymbol(token, g))
        return false;
    return true;
}

void getFirstSet(Grammar &g) { 
    // 终结符的first集就是自身
    for (auto &S : g.tSymbol) {
        g.firstSet[S].insert(S);
    }

    // 非终结符的first集
    for (auto &P : g.grammar) {
        if (isTSymbol(P.rhs[0], g)) {
            g.firstSet[P.lhs[0]].insert(P.rhs[0]);
        }
    }
    bool change = true;
    while (change) {
        change = false;
        for (auto &P : g.grammar) {
            if (isNTSymbol(P.rhs[0], g)) {
                for (auto &S : g.firstSet[P.rhs[0]]) {
                    auto statu = g.firstSet[P.lhs[0]].insert(S);
                    if (statu.second) {
                        change = true;
                    }
                }
            }
        }
    }
};

void getFollowSet (Grammar &g) {
    for (auto &P : g.grammar) {
        for (int i = 0; i < P.rhs.size(); i++) {
            if (i+1 < P.rhs.size() && isNTSymbol(P.rhs[i], g)) {
                for (auto &S : g.firstSet[P.rhs[i+1]]) {
                    g.followSet[P.rhs[i]].insert(S);
                }
            }
        }
    }
}

void initGrammar(std::ifstream &fs, Grammar &g) {
    std::string aLine;
    while (std::getline(fs, aLine)) {
        std::stringstream ss(aLine);
        std::string token;
        ProduceExpr prod;
        std::getline(ss, token, ' ');
        prod.lhs.push_back(token);
        g.ntSymbol.insert(token);
        while (std::getline(ss, token, ' ')) {
            if (token == "->")
                continue;
            prod.rhs.push_back(token);
            if (token[0] == '<' && token.back() == '>')
                continue;
            g.tSymbol.insert(token);
        }
        g.grammar.push_back(prod);
    }
    getFirstSet(g);
    getFollowSet(g);
}

int main() {
    std::ifstream fs("/mnt/d/UbuntuData/C-compiler/tools/LR1/grammar.txt");
    Grammar g;
    if (!fs) {
        std::cout << "Can not open grammar.txt!";
        return -1;
    }
    initGrammar(fs, g);
    g.dump();
    fs.close();
    return 0;
}
