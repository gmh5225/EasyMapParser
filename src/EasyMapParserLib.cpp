#include "EasyMapParserLib.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <ostream>

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
    }

    if (mSymbols.empty())
    {
        return false;
    }

    return true;
}

} // namespace EasyMapParserLib
