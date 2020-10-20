#include "../../../core/deng_core.h"

// test passed on GNU/Linux
// not tested on Windows
int main() {
    dengUtils::FileManager fm;
    std::vector<std::string> local_files_data;

    std::cout << "Enter file path to search for!\n";
    std::string local_path;
    std::getline(std::cin, local_path);
    fm.getFolderContents(local_path, &local_files_data, DENG_FOLDER_CONTENTS_READ_MODE_RECURSIVE, DENG_FOLDER_CONTENTS_SORT_STYLE_DONT_CARE);
    fm.filterByFileExtension(&local_files_data, std::string(DENG_FILE_EXTENSION_CSS));
    
    std::cout << "Folder contains: " << local_files_data.size() << " items\n";
    for(std::string &str : local_files_data)
        std::cout << str << "\n";

    return 0;
}