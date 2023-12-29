#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>

#include <cmdline.hpp>

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, ".UTF-8");

    cmdline::parser arg;

    arg.add<std::string>("input", 'i', "binary file input");
    arg.add<std::string>("output", 'o', "C code output");
    arg.add<std::string>("symbol", 's', "array name");

    arg.parse_check(argc, argv);

    std::filesystem::path INPUT_FILE  = std::filesystem::absolute(arg.get<std::string>("input"));
    std::filesystem::path OUTPUT_FILE = std::filesystem::absolute(arg.get<std::string>("output"));
    std::string           SYMBOL      = arg.get<std::string>("symbol");

    if (!std::filesystem::exists(INPUT_FILE))
    {
        std::cerr << "input file not exist!"
                  << " path: " << INPUT_FILE << std::endl;
        exit(EXIT_FAILURE);
    }

    if (std::filesystem::is_directory(INPUT_FILE))
    {
        std::cerr << "input file is a directory!"
                  << " path: " << INPUT_FILE << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Converting [" << INPUT_FILE << "] in to a C file..." << std::endl;

    std::ofstream outFile;
    outFile.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    outFile.open(OUTPUT_FILE);

    std::ifstream inFile;
    inFile.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    inFile.open(INPUT_FILE, std::ios_base::binary);

    outFile << "const char " << SYMBOL << "[] = {" << std::endl;

    int bytesPerLine = 20;
    int i            = 1;

    using iter = std::istreambuf_iterator<char>;
    for (iter it = iter(inFile.rdbuf()); it != iter(); it++, i++)
    {
        outFile << std::format("{:#04x}", *it) << ", ";
        if (i % bytesPerLine == 0) outFile << std::endl;
    }

    outFile << std::endl << "};" << std::endl;
    outFile.close();

    inFile.close();

    std::cout << "C file generated at: " << OUTPUT_FILE << std::endl;

    exit(EXIT_SUCCESS);
}