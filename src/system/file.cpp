#include "file.h"

#include <fstream>

namespace Game {
    std::vector<char> read_bytes(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) {
            GM_THROW("Could not find file [" << path << "]");
        }

        // ate: Start reading at the end of the file
        // binary: Read the file as binary file (avoid text transformations)
        std::ifstream file{path.c_str(), std::ios::ate | std::ios::binary};
        if (!file.is_open()) {
            GM_THROW("Could not open file with path [" << path << "]");
        }

        // Starting to read at the end of the file means we can use the read position to determine the size of the file.
        auto file_size = (u32) file.tellg();
        std::vector<char> buffer(file_size);

        // Seek back to the beginning of the file and read all bytes.
        file.seekg(0);
        file.read(buffer.data(), file_size);

        file.close();
        return buffer;
    }
}
