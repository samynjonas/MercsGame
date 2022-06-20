#include "pch.h"
#include "FileWriter.h"
#include <iostream>
#include <fstream>
#include <sstream>

FileWriter::FileWriter(std::string mapPath)
	: m_MapPath{ mapPath }
{
}

FileWriter::~FileWriter()
{

}

void FileWriter::Write(const std::vector<Actor*> actors, std::string levelName, std::string mapName)
{
	std::ofstream output{ m_MapPath + "\\" + levelName + ".txt" };

	//Map
	output << "<Map" << std::endl;
	output << "\tname=\"" << mapName << "\"" << std::endl;
	output << "/>" << std::endl << std::endl;

	//Actors
	for (const Actor* actor : actors)
	{
		//std::cout << "<";
		output << "<";
		if (actor->getActorType() != Actor::actorType::object)
		{
			output << TypeToString(actor->getActorType()) << std::endl;
		}

		switch (actor->getActorType())
		{
		case Actor::actorType::player:
			output << "\tpos=\"" << Point2fToString(actor) << "\"" << std::endl;
			break;
		case Actor::actorType::enemy:
			output << "\tpos=\"" << Point2fToString(actor) << "\"" << std::endl;
			break;
		case Actor::actorType::object:	
		{
			const Object* pObject{ static_cast<const Object*>(actor) };
			output << TypeToString(pObject->getObjectType()) << std::endl;

			if (pObject->getObjectType() == Object::objectType::Crate)
			{
				output << "\tpos=\"" << Point2fToString(actor) << "\"" << std::endl;
				output << "\tdrop=\"" << dropToString(pObject) << "\"" << std::endl;
			}
			else
			{
				output << "\tpos=\"" << Point2fToString(actor) << "\"" << std::endl;
			}
		}
		break;
		case Actor::actorType::boss:
		{
			output << "\ttype=\"" << "Jet" << "\"" << std::endl;
			output << "\tpos=\"" << Point2fToString(actor) << "\"" << std::endl;
			output << "\twidth=\"" << widthToString(actor) << "\"" << std::endl;
			output << "\theight=\"" << heightToString(actor) << "\"" << std::endl;
		}
		break;
		}
		output << "/>" << std::endl << std::endl;

	}
	output.close();
}


std::string FileWriter::TypeToString(Actor::actorType actorType) const
{
	switch (actorType)
	{
	case Actor::actorType::player:
		return "Player";
		break;
	case Actor::actorType::enemy:
		return "Enemy";
		break;
	case Actor::actorType::actor:
		return "Actor";
		break;
	case Actor::actorType::object:
		return "Object";
		break;
	case Actor::actorType::powerUp:
		return "powerUp";
		break;
	case Actor::actorType::boss:
		return "Boss";
		break;
	}
	return " ";
}
std::string FileWriter::TypeToString(Object::objectType objectType) const
{
	switch (objectType)
	{
	case Object::objectType::Tree:
		return "Tree";
		break;
	case Object::objectType::House:
		return "House";
		break;
	case Object::objectType::Crate:
		return "Crate";
		break;
	case Object::objectType::SandbagWall:
		return "SandbagWall";
		break;
	}
	return " ";
}

std::string FileWriter::Point2fToString(const Actor* actor) const
{
	float x = actor->getActorRect().left;
	float y = actor->getActorRect().bottom;

	std::string xstr = std::to_string(x);
	std::string ystr = std::to_string(y);

	return xstr+","+ystr;
}
std::string FileWriter::dropToString(const Object* object) const
{
	switch (object->getCrateType())
	{
	case Object::CrateType::Power:
		
		return "Power";
		break;
	case Object::CrateType::Food:
		
		return "Food";
		break;
	case Object::CrateType::Health:

		return "Health";
		break;
	case Object::CrateType::Bomb:
		
		return "Bomb";
		break;
	}
	return " ";
}
std::string FileWriter::BossTypeToString(const BossClass* boss) const
{
	switch (boss->getBossType())
	{
	case BossClass::BossType::Jet:
		return "Jet";
		break;
	case BossClass::BossType::Tank:
		return "Tank";
		break;
	}
	return " ";
}

std::string FileWriter::widthToString(const Actor* actor) const
{
	std::string width{ std::to_string(actor->getActorRect().width) };
	return width;
}
std::string FileWriter::heightToString(const Actor* actor) const
{
	std::string height{ std::to_string(actor->getActorRect().height) };
	return height;
}
