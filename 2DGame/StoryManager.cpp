#include "pch.h"
#include "StoryManager.h"


StoryManager::StoryManager(Rectf window, ResourceManager* resourceManager)
	: m_CurrentLevel{ 0 }
	, m_ResourceManager{ resourceManager }
	, m_Window{ window }
	, m_LoadedLevel{ false }
{
	StoryLevels();
}

StoryManager::~StoryManager()
{
    m_ResourceManager = nullptr;

    delete m_FileReader;
    m_FileReader = nullptr;
}

void StoryManager::Update()
{

}
void StoryManager::Draw() const
{

}

void StoryManager::StoryLevels()
{
	m_StoryModeLevels.push_back("StoryMode1");
}
void StoryManager::LoadLevel()
{
	std::string filePath{ "..\\Resources\\StoryMode\\" + m_StoryModeLevels[m_CurrentLevel] + ".txt"};

	m_FileReader = new FileReader{ filePath, m_Window, m_ResourceManager };
	m_LoadedLevel = true;
}

bool StoryManager::HasLoadedLevel() const
{
    return m_LoadedLevel;
}

std::vector<Actor*> StoryManager::GetLoadedActors()
{
    if (m_LoadedLevel == true)
    {
        m_AddedActors = true;
        return m_FileReader->GetActors();
    }
    return std::vector<Actor*>();
}
std::map<std::string, std::string> StoryManager::getMapPath()
{
    if (m_LoadedLevel == true)
    {
        m_AddedMap = true;
        return m_FileReader->GetMapPath();
    }
    return std::map<std::string, std::string>();
}

bool StoryManager::HasAddedAll() const
{
    if (m_AddedActors == true && m_AddedMap == true)
    {
        return true;
    }
    return false;
}

void StoryManager::nextLevel()
{
    if (m_CurrentLevel < int(m_StoryModeLevels.size()) - 1)
    {
        ++m_CurrentLevel;
    }
    else
    {
        m_CurrentLevel = 0;
        std::cout << "FINISHED GAME" << std::endl;
    }

    m_LoadedLevel = false;
    m_AddedActors = false;
    m_AddedMap = false;
}

void StoryManager::Reset()
{
    m_CurrentLevel = 0;

    m_LoadedLevel = false;
    m_AddedActors = false;
    m_AddedMap = false;
}
