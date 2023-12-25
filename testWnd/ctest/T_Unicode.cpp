#include <gtest/gtest.h>
#include <iostream>

TEST(Unicode, _setlocale)
{
    // UTF-8(File Encoding)
    std::cout << "你好世界！ Hello World!" << std::endl;
    // UTF-8
    std::cout << (const char*)u8"你好世界！ Hello World!" << std::endl;
    // UTF-16(Windows)/UTF-32(Linux)
    std::wcout << L"你好世界！ Hello World!" << std::endl;

    setlocale(LC_ALL, ".UTF-8");

    // UTF-8(File Encoding)
    std::cout << "你好世界！ Hello World!" << std::endl;
    // UTF-8
    std::cout << (const char*)u8"你好世界！ Hello World!" << std::endl;
    // UTF-16(Windows)/UTF-32(Linux)
    std::wcout << L"你好世界！ Hello World!" << std::endl;
}

int main(int argc, char** argv)
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}