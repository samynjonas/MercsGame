#pragma once
#include <map>

class ReadFolder final
{
public:
	ReadFolder();
	~ReadFolder();
	ReadFolder(const ReadFolder& other) = delete;
	ReadFolder& operator=(const ReadFolder& other) = delete;
	ReadFolder(ReadFolder&& other) = delete;
	ReadFolder& operator=(ReadFolder&& other) = delete;

	void SetFolderPath( std::string path );
	std::map<std::string, std::string>& GetFolderContent();

private:
	std::string m_FolderPath;

	std::map<std::string, std::string> m_FolderContent;
};

