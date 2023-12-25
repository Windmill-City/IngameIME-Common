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

    arg.add<std::string>("embed", 'e', "embed file root");
    arg.add<std::string>("index", 'i', "asset index output file");
    arg.add<std::string>("block", 'b', "asset block output file");

    arg.parse_check(argc, argv);

    std::filesystem::path EMBED_ASSETS_DIR        = std::filesystem::absolute(arg.get<std::string>("embed"));
    std::filesystem::path EMBED_ASSETS_INDEX_FILE = std::filesystem::absolute(arg.get<std::string>("index"));
    std::filesystem::path EMBED_ASSETS_BLOCK_FILE = std::filesystem::absolute(arg.get<std::string>("block"));

    if (!std::filesystem::exists(EMBED_ASSETS_DIR))
    {
        std::cerr << "EMBED_ASSETS_DIR not exist!"
                  << " path: " << EMBED_ASSETS_DIR << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!std::filesystem::is_directory(EMBED_ASSETS_DIR))
    {
        std::cerr << "EMBED_ASSETS_DIR is not a directory!"
                  << " path: " << EMBED_ASSETS_DIR << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "searching embed assets at: " << EMBED_ASSETS_DIR << std::endl;

    /**
     * Memory Layout
     * |Element Count(size_t)|Path Size(size_t)|Path String(UTF-8)|File Block Size(size_t)|File Block Offset(size_t)|
     */
    std::ofstream indexAssets;
    indexAssets.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    indexAssets.open(EMBED_ASSETS_INDEX_FILE, std::ios_base::binary);
    /**
     * Memory Layout
     * |File 1 Binary|File 2 Binary|...|File n Binary|
     */
    std::ofstream blockAssets;
    blockAssets.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    blockAssets.open(EMBED_ASSETS_BLOCK_FILE, std::ios_base::binary);

    std::list<std::filesystem::path> files;
    for (auto& it : std::filesystem::recursive_directory_iterator(EMBED_ASSETS_DIR))
    {
        if (!it.is_directory())
        {
            files.push_back(it.path());
        }
    }

    // Element count
    auto count = files.size();
    indexAssets.write((char*)&count, sizeof(count));

    std::ifstream fs;
    fs.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    size_t offset = 0;

    for (auto& it : files)
    {
        // Resource data
        fs.open(it, std::ios_base::binary);
        blockAssets << fs.rdbuf();

        // Resource path
        auto path = std::filesystem::relative(it, EMBED_ASSETS_DIR).string();
        std::replace(path.begin(), path.end(), '\\', '/');
        auto path_s = path.size();
        indexAssets.write((char*)&path_s, sizeof(path_s));
        indexAssets.write((char*)path.data(), path_s);

        // Resource size
        size_t file_s = fs.tellg();
        indexAssets.write((char*)&file_s, sizeof(file_s));
        fs.close();

        // Resource offset
        indexAssets.write((char*)&offset, sizeof(offset));
        offset += file_s;

        std::cout << "inserted file: [" << it.string() << "], size: " << file_s << std::endl;
    }

    indexAssets.close();
    blockAssets.close();

    std::cout << "indexAssets generated at: " << EMBED_ASSETS_INDEX_FILE << std::endl;
    std::cout << "blockAssets generated at: " << EMBED_ASSETS_BLOCK_FILE << std::endl;

    exit(EXIT_SUCCESS);
}