#include "pch.h"
#include "FileReader.h"

#include <sstream>
#include <fstream>
#include <iostream>

FileReader::FileReader(std::string filePath, Rectf window, ResourceManager* resourceManager)
	: m_Window{ window }
	, m_MapReader{ new ReadFolder{} }
	, m_SVGReader{ new ReadFolder{} }
	, m_ResourceManager{ resourceManager }
{
	ReadPath(filePath);

	m_MapReader->SetFolderPath("..\\Resources\\Maps\\PNG");
	m_SVGReader->SetFolderPath("..\\Resources\\Maps\\SVG");
}

FileReader::~FileReader()
{
	delete m_MapReader;
	m_MapReader = nullptr;

	delete m_SVGReader;
	m_SVGReader = nullptr;

	m_ResourceManager = nullptr;
}

void FileReader::ReadPath(std::string filePath)
{
	std::ifstream input(filePath);
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
					CreateActor(sentence);
					
					sentence = "";
					foundSentence = false;
				}
			}
		}
	}
}

void FileReader::CreateActor(const std::string& actorElement)
{
	if (actorElement.find("Map") != std::string::npos)
	{
		createMap(actorElement);
	}
	if (actorElement.find("Player") != std::string::npos)
	{
		//Create player
		createPlayer(actorElement);
	}
	if (actorElement.find("Enemy") != std::string::npos)
	{
		createEnemySpawner(actorElement);
	}
	if (actorElement.find("Crate") != std::string::npos)
	{
		CreateCrate(actorElement);
	}
	if (actorElement.find("Tree") != std::string::npos)
	{
		CreateTree(actorElement);
	}
	if (actorElement.find("House") != std::string::npos)
	{
		CreateHouse(actorElement);
	}
	if (actorElement.find("Sandbag") != std::string::npos)
	{
		CreateSandbag(actorElement);
	}
	if (actorElement.find("Boss") != std::string::npos)
	{
		CreateBoss(actorElement);
	}
}

void FileReader::createMap(const std::string& actorElement)
{
	m_MapReader->SetFolderPath("..\\Resources\\Maps\\PNG");
	m_SVGReader->SetFolderPath("..\\Resources\\Maps\\SVG");

	std::string mapName{ GetAttributeValue("name", actorElement)};

	std::string pngPath{ m_MapReader->GetFolderContent()[mapName] };
	std::string SVGPath{ m_SVGReader->GetFolderContent()[mapName] };
	m_Map[pngPath] = SVGPath;
}

void FileReader::createPlayer(const std::string& actorElement)
{
	Point2f position = ToPoint2f(GetAttributeValue("pos", actorElement));
	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << "Spawn player on: " << position.x << " : " << position.y << std::endl;
	}
	m_Actors.push_back( new Player{ Rectf{position.x, position.y, 30, 40}, m_Window, m_ResourceManager });
}
void FileReader::createEnemySpawner(const std::string& actorElement)
{
	Point2f position = ToPoint2f(GetAttributeValue("pos", actorElement));
	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << "Spawn enemySpawner on: " << position.x << " : " << position.y << std::endl;
	}
	m_Actors.push_back(new EnemyAssaultTrooper{ Rectf{position.x, position.y, 30, 40}, m_Window, m_ResourceManager });
}
void FileReader::CreateName(const std::string& actorElement)
{
	std::string name = GetAttributeValue("Name", actorElement);
	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << "levelName: " << name << std::endl;
	}
}

void FileReader::CreateCrate(const std::string& actorElement)
{
	Point2f position = ToPoint2f(GetAttributeValue("pos", actorElement));
	std::string drop = GetAttributeValue("drop", actorElement);

	//return new player
	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << "Spawn crate on: " << position.x << " : " << position.y << " with " << drop << std::endl;
	}

	m_Actors.push_back(new Object{ Rectf{ position.x, position.y, 50, 50 }, m_Window, m_ResourceManager, Object::objectType::Crate, Object::CrateType::Power });

}
void FileReader::CreateTree(const std::string& actorElement)
{
	Point2f position = ToPoint2f(GetAttributeValue("pos", actorElement));

	//return new player
	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << "Spawn tree on: " << position.x << " : " << position.y << std::endl;
	}

	m_Actors.push_back(new Object{ Rectf{ position.x, position.y, 10, 150 },  m_Window, m_ResourceManager, Object::objectType::Tree });
}
void FileReader::CreateHouse(const std::string& actorElement)
{
	Point2f position = ToPoint2f(GetAttributeValue("pos", actorElement));

	//return new player
	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << "Spawn house on: " << position.x << " : " << position.y << std::endl;
	}

	m_Actors.push_back(new Object{ Rectf{ position.x, position.y, 10, 150 },  m_Window, m_ResourceManager, Object::objectType::House });
}
void FileReader::CreateSandbag(const std::string& actorElement)
{
	Point2f position = ToPoint2f(GetAttributeValue("pos", actorElement));

	//return new player
	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << "Spawn sandbag on: " << position.x << " : " << position.y << std::endl;
	}
	m_Actors.push_back(new Object{ Rectf{ position.x, position.y, 10, 150 },  m_Window, m_ResourceManager, Object::objectType::SandbagWall });
}

void FileReader::CreateBoss(const std::string& actorElement)
{
	std::string type = GetAttributeValue("type", actorElement);
	Point2f position = ToPoint2f(GetAttributeValue("pos", actorElement));
	float width = std::strtof(GetAttributeValue("width", actorElement).data(), nullptr);
	float height = std::strtof(GetAttributeValue("height", actorElement).data(), nullptr);

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << "Boss: " << type << ", bossRoom: " << position.x << " : " << position.y << " : " << width << " : " << height << std::endl;
	}
}

Point2f FileReader::ToPoint2f(const std::string& point2fStr) const
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

std::string FileReader::GetAttributeValue(const std::string& attrName, const std::string& element) const
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
int FileReader::attributeLastIndex(const std::string& attrName, const std::string& element) const
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

void FileReader::testString(std::string string) const
{
	std::cout << "test: " << string << std::endl;
}

std::vector<Actor*> FileReader::GetActors() const
{
	return m_Actors;
}
std::map<std::string, std::string> FileReader::GetMapPath()
{
	return m_Map;
}


