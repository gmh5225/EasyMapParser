#include "EasyMapParserLib.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <ostream>
#include <regex>

namespace EasyMapParserLib {

MapParser::MapParser()
{
    mSymbols.clear();
}

const std::vector<Symbol> &
MapParser::GetSymbols() const
{
    return mSymbols;
}

bool
MapParser::Parse(const char *MapFilePath)
{
    auto ParseSymbol = [&](std::string &Line) {
        std::regex Reg(
            R"#(\s(\d+)\:([a-fA-F0-9]+)\s+(\S+)\s+([a-fA-F0-9]+)\s+(.+))#", std::regex_constants::ECMAScript);
        if (std::regex_match(Line, Reg))
        {
            std::smatch Match;
            for (; std::regex_search(Line, Match, Reg); Line = Match.suffix())
            {
                Symbol Sym = {0};
                Sym.Address = std::strtoull(Match[4].str().c_str(), nullptr, 16);

                if (Sym.Address == 0)
                {
                    continue;
                }

                Sym.SectionIndex = std::atoi(Match[1].str().c_str());
                Sym.SectionOffset = std::strtoul(Match[2].str().c_str(), nullptr, 16);
                Sym.SymbolName = Match[3].str();
                Sym.LibObjName = Match[5].str();

                mSymbols.emplace_back(Sym);
            }
        }
    };

    if ((access(MapFilePath, 0) != 0))
    {
        // file does not exist
        return false;
    }

    std::ifstream IfStream(MapFilePath);
    std::string Line = "";

    while (std::getline(IfStream, Line))
    {
        // read line
        ParseSymbol(Line);
    }

    if (mSymbols.empty())
    {
        return false;
    }

    return true;
}

} // namespace EasyMapParserLib
