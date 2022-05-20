
#include <stdlib.h>
#include <stdio.h>
#include "EasyMapParserLib.h"

int
main()
{
    EasyMapParserLib::MapParser parser;
    bool ok = parser.Parse(EASYMAPPARSER_SRC_DIR "/sample/testexe.map");
    if (!ok)
    {
        printf("parse failed\n");
        return -1;
    }

    auto base = parser.GetImageBase();
    printf("base=%I64X\n", base);
    for (auto &v : parser.GetSymbols())
    {
        int offset;
        if (v.Address >= base)
        {
            offset = v.Address - base;
        }
        else
        {
            offset = v.Address;
        }

        printf(
            "SectionIndex=0x%x, SectionOffset=0x%x, Offset=0x%x, SymbolName=%s, LibObjName=%s\n",
            v.SectionIndex,
            v.SectionOffset,
            offset,
            v.SymbolName.c_str(),
            v.LibObjName.c_str());
    }
    return 0;
}
