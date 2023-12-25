// Copyright (C) 2021 Theo Niessink <theo@taletn.com>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>

#include <cmdline.hpp>

int main(int argc, char* argv[])
{
    cmdline::parser arg;

    arg.add<std::string>("binary", 'b', "binary file");
    arg.add<std::string>("object", 'o', "object file");

    arg.add<std::string>("symbol", 's', "symbol name of the binary data");
    arg.add<int>("align",
                 'a',
                 "align of binary data",
                 false,
                 1,
                 cmdline::oneOf<int>(1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192));
    arg.parse_check(argc, argv);

    auto path_b = std::filesystem::absolute(arg.get<std::string>("binary"));
    auto path_o = std::filesystem::absolute(arg.get<std::string>("object"));

    std::ifstream binary;
    binary.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    binary.open(path_b, std::ios_base::binary);

    std::ofstream object;
    object.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    object.open(path_o, std::ios_base::binary);

    IMAGE_FILE_HEADER    imgFile      = {};
    IMAGE_SECTION_HEADER imgSection   = {};
    IMAGE_SYMBOL         imgSymbol[2] = {};

    // Binary size
    binary.seekg(0, binary.end);
    size_t binary_s = binary.tellg();
    binary.seekg(0, binary.beg);

    imgFile.Machine              = IMAGE_FILE_MACHINE_UNKNOWN;
    imgFile.NumberOfSections     = 1;
    imgFile.TimeDateStamp        = (DWORD)time(nullptr);
    imgFile.PointerToSymbolTable = (DWORD)(sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_SECTION_HEADER) + binary_s);
    imgFile.NumberOfSymbols      = 2;

    const BYTE section[IMAGE_SIZEOF_SHORT_NAME] = ".rdata";
    memcpy(imgSection.Name, section, IMAGE_SIZEOF_SHORT_NAME);

    imgSection.SizeOfRawData    = (DWORD)binary_s;
    imgSection.PointerToRawData = sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_SECTION_HEADER);

    int align = arg.get<int>("align");

    switch (align)
    {
    case 1: imgSection.Characteristics = IMAGE_SCN_ALIGN_1BYTES; break;
    case 2: imgSection.Characteristics = IMAGE_SCN_ALIGN_2BYTES; break;
    case 4: imgSection.Characteristics = IMAGE_SCN_ALIGN_4BYTES; break;
    case 8: imgSection.Characteristics = IMAGE_SCN_ALIGN_8BYTES; break;
    case 16: imgSection.Characteristics = IMAGE_SCN_ALIGN_16BYTES; break;
    case 32: imgSection.Characteristics = IMAGE_SCN_ALIGN_32BYTES; break;
    case 64: imgSection.Characteristics = IMAGE_SCN_ALIGN_64BYTES; break;
    case 128: imgSection.Characteristics = IMAGE_SCN_ALIGN_128BYTES; break;
    case 256: imgSection.Characteristics = IMAGE_SCN_ALIGN_256BYTES; break;
    case 512: imgSection.Characteristics = IMAGE_SCN_ALIGN_512BYTES; break;
    case 1024: imgSection.Characteristics = IMAGE_SCN_ALIGN_1024BYTES; break;
    case 2048: imgSection.Characteristics = IMAGE_SCN_ALIGN_2048BYTES; break;
    case 4096: imgSection.Characteristics = IMAGE_SCN_ALIGN_4096BYTES; break;
    case 8192: imgSection.Characteristics = IMAGE_SCN_ALIGN_8192BYTES; break;
    }

    imgSection.Characteristics |= IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ;

    memcpy(imgSymbol[0].N.ShortName, section, IMAGE_SIZEOF_SHORT_NAME);
    imgSymbol[0].SectionNumber = 1;
    imgSymbol[0].StorageClass  = IMAGE_SYM_CLASS_STATIC;

    // String table has a 4byte size value at front
    size_t strTableOffset = sizeof(DWORD);

    auto symbol = arg.get<std::string>("symbol");
    if (symbol.size() <= sizeof(imgSymbol[1].N.ShortName))
    {
        memcpy(imgSymbol[1].N.ShortName, symbol.data(), symbol.size());
    }
    else
    {
        imgSymbol[1].N.Name.Long = (DWORD)strTableOffset;
        strTableOffset += symbol.size() + 1;
    }
    imgSymbol[1].SectionNumber = 1;
    imgSymbol[1].StorageClass  = IMAGE_SYM_CLASS_EXTERNAL;

    // COFF Header
    object.write((char*)&imgFile, sizeof(imgFile));
    // Section Header
    object.write((char*)&imgSection, sizeof(imgSection));
    // Section Raw Data
    object << binary.rdbuf();
    // Symbol Table
    object.write((char*)&imgSymbol, sizeof(imgSymbol));
    // String Table - Size
    object.write((char*)&strTableOffset, sizeof(DWORD));
    // String Table - Data
    object.write(symbol.data(), symbol.size() + 1);

    std::cout << "object file generated at: " << path_o << std::endl;

    exit(EXIT_SUCCESS);
}