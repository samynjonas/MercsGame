#include "pch.h"
#include "ReadFolder.h"
#include <filesystem>

namespace fs = std::filesystem;

ReadFolder::ReadFolder()
	: m_FolderPath{ "" }
{

}
ReadFolder::~ReadFolder()
{

}

void ReadFolder::SetFolderPath(std::string path)
{
    m_FolderContent.clear();
	m_FolderPath = path;

    std::string name{};
    for (auto& p : fs::directory_iterator(path))
    {
        std::string filePath = p.path().string();
        for (size_t index = 0; index < filePath.size(); index++)
        {
            if (path.size() + index < filePath.size())
            {
                if (filePath[path.size() + index] != 92 && filePath[path.size() + index] != ' ')
                {
                    if (filePath[path.size() + index] == '.')
                    {
                        break;
                    }
                    name.push_back(filePath[path.size() + index]);
                }
            }
        }
        m_FolderContent[name] = filePath;

        name = "";
    }
}

std::map<std::string, std::string>& ReadFolder::GetFolderContent()
{
	return m_FolderContent;
}
