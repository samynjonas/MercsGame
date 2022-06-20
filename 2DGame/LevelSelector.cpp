#include "pch.h"
#include "LevelSelector.h"
#include <string>

LevelSelector::LevelSelector( Rectf window, ResourceManager* resourceManager)
    : m_FontPath{ "..\\Resources\\Fonts\\RetroFont.ttf" }
    , m_Window{ window }
    , m_IsButtonPressed{ true }
    , m_LoadedLevel{ false }
    , m_LoadedEditFile{ false }
    , m_CreateNewFile{ false }
    , m_ResourceManager{ resourceManager }
    , m_ScrollBox{ new ScrollBox{ Rectf{ 10, 10, window.width - 300, window.height - 20 }, Color4f{0.5f, 0.5f, 0, 1}, window} }
    , m_Edit{ new MouseButton{ Rectf{   window.left + window.width - 200.f, window.bottom + 300, 150, 75 }, "Edit Level", Color4f{1, 1, 1, 1}, 30, Color4f{0, 0, 0, 1}, window, 0} }
    , m_Play{ new MouseButton{ Rectf{   window.left + window.width - 200.f, window.bottom + 200, 150, 75 }, "Play Level", Color4f{1, 1, 1, 1}, 30, Color4f{0, 0, 0, 1}, window, 0} }
    , m_Create{ new MouseButton{ Rectf{ window.left + window.width - 200.f, window.bottom + 100, 150, 75 }, "Create new", Color4f{1, 1, 1, 1}, 30, Color4f{0, 0, 0, 1}, window, 0} }
    , m_FolderReader{ new ReadFolder{} }
    , m_FileReader{ nullptr }
{
    SearchForFiles();
    InitializeKeys();
    AddToScroll();

    //ReadKeys();
}

LevelSelector::~LevelSelector()
{
    if (m_FileReader != nullptr)
    {
        delete m_FileReader;
        m_FileReader = nullptr;
    }

    delete m_FolderReader;
    m_FolderReader = nullptr;

    m_ResourceManager = nullptr;

    delete m_ScrollBox;
    m_ScrollBox = nullptr;

    delete m_Edit;
    m_Edit = nullptr;

    delete m_Play;
    m_Play = nullptr;

    delete m_Create;
    m_Create = nullptr;
}

void LevelSelector::Update(float elapsedSec)
{
    m_ScrollBox->Update();

    if (m_Play->IsButtonPressed())
    {
        if (m_ScrollBox->IsAButtonPressed())
        {
            if (m_ResourceManager->Debug()->IsEnabled())
            {
                std::cout << "load map: " << m_ScrollBox->GetButtonText() << std::endl;
            }
            std::string filePath{ "..\\Resources\\customMaps\\ " + m_ScrollBox->GetButtonText() + ".txt" };

            m_FileReader = new FileReader{ filePath, m_Window, m_ResourceManager };

            m_IsButtonPressed = true;
            m_LoadedLevel = true;
        }
    }
    else if (m_Edit->IsButtonPressed())
    {
        if (m_ScrollBox->IsAButtonPressed())
        {
            if (m_ResourceManager->Debug()->IsEnabled())
            {
                std::cout << "edit map: " << m_ScrollBox->GetButtonText() << std::endl;
            }
            std::string filePath{ "..\\Resources\\customMaps\\ " + m_ScrollBox->GetButtonText() + ".txt" };

            m_FileReader = new FileReader{ filePath, m_Window, m_ResourceManager };

            m_IsButtonPressed = true;
            m_LoadedEditFile = true;
        }
    }
    else if (m_Create->IsButtonPressed())
    {
        m_IsButtonPressed = true;
        m_CreateNewFile = true;
    }
}

void LevelSelector::Draw() const
{
    m_ScrollBox->Draw();
    m_Edit->Draw(true);
    m_Play->Draw(true);
    m_Create->Draw(true);
}

void LevelSelector::SearchForFiles()
{
    m_FolderReader->SetFolderPath("..\\Resources\\customMaps");
    m_LevelFiles = m_FolderReader->GetFolderContent();

    if (m_ResourceManager->Debug()->IsEnabled())
    {
        std::cout << "level amount: " << m_FolderReader->GetFolderContent().size() << std::endl;
    }
}
void LevelSelector::ReadMap() const
{
    /*std::cout << "map contains:" << std::endl;
    for (const auto& map : m_LevelFiles)
    {
        std::cout << map.first << " : " << map.second << std::endl;
    }*/
}
void LevelSelector::InitializeKeys()
{
    for (const auto& map : m_LevelFiles)
    {
        m_Keys.push_back( map.first );
    }
}
void LevelSelector::ReadKeys()
{
    for (size_t index = 0; index < m_Keys.size(); index++)
    {
        if (m_ResourceManager->Debug()->IsEnabled())
        {
            std::cout << index << ": " << m_Keys[index] << " : " << m_LevelFiles[m_Keys[index]] << std::endl;
        }
    }
}

bool LevelSelector::HasLoadLevel() const
{
    return m_LoadedLevel;
}
bool LevelSelector::HasLoadEditor() const
{
    return m_LoadedEditFile;
}
bool LevelSelector::NeedNewFile() const
{
    return m_CreateNewFile;
}

std::vector<Actor*> LevelSelector::GetLoadedActors() const
{
    if (m_LoadedLevel == true || m_LoadedEditFile == true)
    {
        return m_FileReader->GetActors();
    }
    return std::vector<Actor*>();
}
std::map<std::string, std::string> LevelSelector::getMapPath()
{
    if (m_LoadedLevel == true || m_LoadedEditFile == true)
    {
        return m_FileReader->GetMapPath();
    }
    return std::map<std::string, std::string>();
}

std::string LevelSelector::GetFileName() const
{
    return std::string{ " " + m_ScrollBox->GetButtonText() };
}

void LevelSelector::Reset()
{
    m_IsButtonPressed = false;
    m_LoadedLevel = false;
    m_LoadedEditFile = false;
    m_CreateNewFile = false;

    m_Edit->DeActivate();
    m_Play->DeActivate();
    m_Create->DeActivate();

    m_Keys.clear();
    m_LevelFiles.clear();

    delete m_ScrollBox;
    m_ScrollBox = new ScrollBox{ Rectf{ 10, 10, m_Window.width - 300, m_Window.height - 20 }, Color4f{0.5f, 0.5f, 0, 1}, m_Window };

    SearchForFiles();
    InitializeKeys();
    AddToScroll();
}

void LevelSelector::AddToScroll()
{
    for (size_t index = 0; index < m_Keys.size(); index++)
    {
        m_ScrollBox->AddButton(m_Keys[index], Color4f{ 1, 1, 1, 1 }, Color4f{ 0, 0, 0, 1 }, m_Window);
    }
}