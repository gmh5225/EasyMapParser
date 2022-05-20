#ifndef _EASY_MAP_PARSER_LIB_H
#define _EASY_MAP_PARSER_LIB_H

#include <string>
#include <vector>

namespace EasyMapParserLib {
struct Symbol
{
    unsigned int SectionIndex;
    unsigned int SectionOffset;
    unsigned long long Address;
    std::string SymbolName;
    std::string LibObjName;
};

class MapParser
{
public:
    MapParser();
    virtual ~MapParser() = default;

public:
    bool Parse(const char *MapFilePath);
    const std::vector<Symbol> &GetSymbols() const;

protected:
    std::vector<Symbol> mSymbols;
};

} // namespace EasyMapParserLib

#endif
