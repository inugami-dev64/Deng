#include "../../../core/deng_core.h"

// test passed on GNU/Linux
// not tested on Windows
int main() {
    dengUtils::FileManager fm;
    std::vector<std::string> files_data;

    std::cout << "Enter file path to search for!\n";
    std::string path;
    std::getline(std::cin, path);
    fm.getFolderContents(path, &files_data, DENG_FOLDER_CONTENTS_READ_MODE_RECURSIVE, DENG_FOLDER_CONTENTS_SORT_STYLE_DONT_CARE);
    fm.filterByFileExtension(&files_data, std::string(DENG_FILE_EXTENSION_CSS));
    
    std::cout << "Folder contains: " << files_data.size() << " items\n";
    for(std::string &str : files_data)
        std::cout << str << "\n";

    return 0;
}