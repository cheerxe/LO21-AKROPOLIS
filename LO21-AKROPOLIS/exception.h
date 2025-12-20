#pragma once
#include <string>

class AkropolisException {
    std::string info;
    std::string classe;
public:
    AkropolisException(const std::string& txt, const std::string& c = "Not Specified") : info(txt), classe(c) {}
    const std::string& getInfo() { return info; }
    const std::string& getClasse() { return classe; }
};