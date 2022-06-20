#include "pch.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "Texture.h"

#include <sstream>
#include <fstream>
#include <iostream>


SpriteManager::SpriteManager()
	: m_CurrentLoadIndex{	0 }
	, m_CurrentThemeIndex{	0 }
	, m_HasReadFolders{ false }
	, m_FolderReader{	new ReadFolder{} }
	, m_DataReader{		new ReadFolder{} }
	, m_SpriteReader{	new ReadFolder{} }
{
	SetRequiredTextures();
}

SpriteManager::~SpriteManager()
{
	delete m_FolderReader;
	m_FolderReader = nullptr;

	delete m_DataReader;
	m_DataReader = nullptr;

	delete m_SpriteReader;
	m_SpriteReader = nullptr;
	
	dispose();
}

void SpriteManager::dispose() 
{
	for (auto map = m_Sprites.begin(); map != m_Sprites.end(); map++)
	{
		delete (map->second);
	}
	m_Sprites.clear();
	m_CurrentLoadIndex = 0;
}

void SpriteManager::SetRequiredTextures()
{
	m_RequiredTextures.push_back("Player");
	m_RequiredTextures.push_back("Enemy");
	m_RequiredTextures.push_back("Objects");
	m_RequiredTextures.push_back("Bullets");
	m_RequiredTextures.push_back("Boss");

}

void SpriteManager::LoadTextures()
{
	if (!m_HasReadFolders)
	{
		std::cout << m_FolderReader->GetFolderContent().size() << std::endl;

		m_FolderReader->SetFolderPath("..\\Resources\\Textures");
		for (auto& map : m_FolderReader->GetFolderContent())
		{
			m_TextureThemes.push_back(map.first);

			m_DataReader->SetFolderPath(map.second + "\\Data");
			m_SpriteReader->SetFolderPath(map.second + "\\Sprites");
			m_HasReadFolders = true;

			m_DataMap[map.first] = m_DataReader->GetFolderContent();
			m_TextureMap[map.first] = m_SpriteReader->GetFolderContent();
		}
	}
	
	if (!IsDoneLoading())
	{
		ReadFile(m_DataMap[ m_TextureThemes[m_CurrentThemeIndex]] [m_RequiredTextures[m_CurrentLoadIndex]] );
	}
	m_CurrentLoadIndex++;
}
void SpriteManager::setTextureTheme(std::string name)
{
	bool foundTheme{ false };

	for (size_t index = 0; index < m_TextureThemes.size(); index++)
	{
		if (m_TextureThemes[index] == name)
		{
			foundTheme = true;
			m_CurrentTheme = name;
		}
	}
	if (foundTheme == false)
	{
		name = m_TextureThemes[0];
	}

	for (size_t index = 0; index < m_TextureThemes.size(); index++)
	{
		if (name == m_TextureThemes[index])
		{
			m_CurrentThemeIndex = int(index);
		}
	}

	dispose();

	while (IsDoneLoading() == false)
	{
		ReadFile(m_DataMap[m_TextureThemes[m_CurrentThemeIndex]][m_RequiredTextures[m_CurrentLoadIndex]]);
		m_CurrentLoadIndex++;
	}
}
int SpriteManager::LoadSize() const
{
	return int(m_RequiredTextures.size());
}
bool SpriteManager::IsDoneLoading() const
{
	if (m_CurrentLoadIndex >= int(m_RequiredTextures.size()))
	{
		return true;
	}
	return false;
}

SpriteInfo* SpriteManager::GetSprite(std::string name)
{
	return m_Sprites[name];
}

void SpriteManager::ReadFile(const std::string& spriteDataPath)
{
	std::ifstream input(spriteDataPath);
	std::string sentence{};

	bool foundSentence{ false };

	while (input.peek() != EOF)
	{
		std::string readText{}; //String that saves the data
		std::getline(input, readText);

		sentence += readText;
		for (size_t index = 0; index < sentence.size(); index++)
		{
			if (sentence[index] == '>')
			{
				foundSentence = true;
			}
		}

		if (foundSentence == true)
		{
			for (size_t index = 0; index < sentence.size(); index++)
			{
				if (sentence[index] == '<')
				{
					//std::cout << sentence << std::endl;
					//CreateActor(sentence);
					FillInInfo(sentence);


					sentence = "";
					foundSentence = false;
				}
			}
		}
	}
}
void SpriteManager::FillInInfo(const std::string& sentence)
{
	m_Sprites[GetAttributeValue("name", sentence)] = new SpriteInfo{ InitializeTexture(m_TextureMap[m_TextureThemes[m_CurrentThemeIndex]][m_RequiredTextures[m_CurrentLoadIndex]]),
																					ToRectf(GetAttributeValue("SpriteRect", sentence)),
																					ToInt(GetAttributeValue("Cols", sentence)), 
																					ToInt(GetAttributeValue("Rows", sentence)), 
																					tofloat(GetAttributeValue("fps", sentence)), 
																					tofloat(GetAttributeValue("Scale", sentence)), 
																					};
	
	/*std::cout << GetAttributeValue("name", sentence)
		<< " : " << ToRectf(GetAttributeValue("SpriteRect", sentence)).left
		<< " : " << ToRectf(GetAttributeValue("SpriteRect", sentence)).bottom
		<< " : " << ToRectf(GetAttributeValue("SpriteRect", sentence)).width
		<< " : " << ToRectf(GetAttributeValue("SpriteRect", sentence)).height
		<< std::endl;*/	
}
Texture* SpriteManager::InitializeTexture(const std::string& filename) const
{
	return new Texture{ filename };
}

std::string SpriteManager::GetAttributeValue(const std::string& attrName, const std::string& element) const
{
	std::string attribute{};
	bool startWord{ false };

	for (size_t index = attributeLastIndex(attrName, element); index < element.size(); index++)
	{
		if (index > 0)
		{
			if (element[index - 1] == '"' && startWord == false)
			{
				startWord = true;
				attribute = "";
			}
			if (startWord == true)
			{
				attribute += element[index];
			}
			if (element[index + 1] == '"' && startWord == true)
			{
				return attribute;
			}
		}
	}
	return attribute;
}
int SpriteManager::attributeLastIndex(const std::string& attrName, const std::string& element) const
{
	for (size_t index = 0; index < element.size(); index++)
	{
		if (index > 0)
		{
			if (element[index] == attrName[0])
			{
				//First letter
				if (element[index + 1] == attrName[1])
				{
					//Second letter
					return int(index);
				}
			}
		}
	}
	return 0;
}

int SpriteManager::ToInt(const std::string& intStr) const
{
	if (intStr.empty())
	{
		return 0;
	}
	else
	{
		return std::stoi(intStr);
	}
}
float SpriteManager::tofloat(const std::string& floatstr) const
{
	if (floatstr.empty())
	{
		return 0.f;
	}
	else
	{
		return std::stof(floatstr);
	}
}
Point2f SpriteManager::ToPoint2f(const std::string& point2fStr) const
{
	std::string x;
	std::string y;
	bool filledX{ false };

	for (size_t index = 0; index < point2fStr.size(); index++)
	{
		if (point2fStr[index] != ',' && filledX == false)
		{
			x += point2fStr[index];
		}
		else if (point2fStr[index] == ',')
		{
			filledX = true;
		}
		else if (point2fStr[index] != ',' && filledX == true)
		{
			y += point2fStr[index];
		}
	}

	Point2f point;
	if (x.empty() || y.empty())
	{
		point = Point2f{ 0, 0 };
	}
	else
	{
		point.x = std::stof(x);
		point.y = std::stof(y);
	}
	return point;
}
Rectf SpriteManager::ToRectf(const std::string& rectStr) const
{
	std::string x{};
	bool filledX{ false };
	std::string y{};
	bool filledY{ false };
	std::string width{};
	bool filledWidth{ false };
	std::string height{};
	if (!rectStr.empty())
	{
		for (size_t index = 0; index < rectStr.size(); index++)
		{
			if (rectStr[index] != ',' && filledX == false)
			{
				x += rectStr[index];
			}
			else if (rectStr[index] == ',')
			{
				if (filledY == true)
				{
					filledWidth = true;
				}
				else if (filledX == true)
				{
					filledY = true;
				}
				filledX = true;
			}
			else if (rectStr[index] != ',' && filledY == false)
			{
				y += rectStr[index];
			}
			else if (rectStr[index] != ',' && filledWidth == false)
			{
				width += rectStr[index];
			}
			else if (rectStr[index] != ',' && filledWidth == true)
			{
				height += rectStr[index];
			}
		}
		return Rectf{ tofloat(x), tofloat(y), tofloat(width), tofloat(height)};
	}
	else
	{
		return Rectf{0, 0, 0, 0};
	}
}

std::vector<std::string> SpriteManager::getThemes() const
{
	return m_TextureThemes;
}
std::string SpriteManager::getCurrentTheme() const
{
	return m_TextureThemes[m_CurrentThemeIndex];
}
