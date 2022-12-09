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
    mImageBase = 0;
}

const std::vector<Symbol> &
MapParser::GetSymbols() const
{
    return mSymbols;
}

unsigned long long
MapParser::GetImageBase()
{
    return mImageBase;
}

bool
MapParser::Parse(const char *MapFilePath)
{
    mSymbols.clear();
    mImageBase = 0;

    auto ParseImageBase = [&](std::string &Line) {
        auto Idx = Line.find("Preferred load address is ");
        if (Idx != std::string::npos)
        {
            auto StartIdx = Idx + strlen("Preferred load address is ");
            auto ImageBaseStr = Line.substr(StartIdx);
            mImageBase = std::strtoull(ImageBaseStr.c_str(), nullptr, 16);
        }
    };

    auto ParseSymbol = [&](std::string &Line) {
        // https://github.com/mike1k/perses/blob/master/src/mapfileparser.cpp#LL25
        std::regex Reg(
            R"#(\s(\d+):([a-fA-F0-9]+)\s+(\S+)\s+([a-fA-F0-9]+)\s+(.+))#", std::regex_constants::ECMAScript);
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

                if (Sym.Address < 0x1000)
                {
                    continue;
                }

                Sym.SectionIndex = std::atoi(Match[1].str().c_str());
                Sym.SectionOffset = std::strtoul(Match[2].str().c_str(), nullptr, 16);
                Sym.SymbolName = Match[3].str();
                Sym.LibObjName = Match[5].str();

                if (Line.find(" f ") != std::string::npos)
                {
                    Sym.IsFunction = true;

                    // Fix LibObjName
                    auto TempName = Sym.LibObjName;
                    auto LastSpaceIdx = TempName.rfind(" ");
                    if (LastSpaceIdx != std::string::npos)
                    {
                        Sym.LibObjName = Sym.LibObjName.substr(LastSpaceIdx + 1);
                    }
                }

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

        if (mImageBase == 0)
        {
            ParseImageBase(Line);
            continue;
        }

        ParseSymbol(Line);
    }

    if (mSymbols.empty())
    {
        return false;
    }

    return true;
}

} // namespace EasyMapParserLib
