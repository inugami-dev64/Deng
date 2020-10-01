#include "../../core/deng_core.h"

namespace dengCSS {
    CSSDataHandler::CSSDataHandler() {
        std::vector<std::string> local_folder_contents;
        std::vector<std::vector<std::string>> local_css_files_data;
        this->m_filemanager.getFolderContents("../css", &local_folder_contents, DENG_FOLDER_CONTENTS_READ_MODE_FILES_ONLY, DENG_FOLDER_CONTENTS_SORT_STYLE_ALL_ALPHABETICAL);
        local_css_files_data.resize(local_folder_contents.size());

        for(size_t i = 0; i < local_css_files_data.size(); i++)
            this->m_filemanager.getFileContents(local_folder_contents[i], nullptr, &local_css_files_data[i]);

        this->verifyTitleData(&local_css_files_data);
        this->getTitleData(&local_css_files_data);
        this->verifyBodyData(&local_css_files_data);
        this->getBodyData(&local_css_files_data);
        this->verifyMinimiseTriangleData(&local_css_files_data);
        this->getMinimiseTriangleData(&local_css_files_data);
    }

    void CSSDataHandler::getTitleData(std::vector<std::vector<std::string>> *p_css_files_data) {
        
    }
}