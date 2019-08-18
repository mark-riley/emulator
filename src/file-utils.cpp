#include "file-utils.h"
#include <fstream>
#include <iterator>

std::vector<uint8_t> readFileToBuffer(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    file.unsetf(std::ios::skipws);

    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> vec;
    vec.reserve(fileSize);

    vec.insert(
        vec.begin(),
        std::istream_iterator<uint8_t>(file),
        std::istream_iterator<uint8_t>()
    );

    return vec;
}