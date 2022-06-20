#pragma once
#include <map>
#include <vector>

#include "ReadFolder.h"

class Sprite;
class Texture;

class SpriteManager final
{
public:
	SpriteManager();
	~SpriteManager();
	SpriteManager(const SpriteManager& other) = delete;
	SpriteManager& operator=(const SpriteManager& other) = delete;
	SpriteManager(SpriteManager&& other) = delete;
	SpriteManager& operator=(SpriteManager&& other) = delete;

	SpriteInfo* GetSprite( std::string name );

	void setTextureTheme(std::string name);

	void LoadTextures();
	int LoadSize() const;
	bool IsDoneLoading() const;

	std::vector<std::string> getThemes() const;
	std::string getCurrentTheme() const;

private:
	int m_CurrentLoadIndex;
	int m_CurrentThemeIndex;

	bool m_HasReadFolders;

	void dispose();

	ReadFolder* m_FolderReader;
	ReadFolder* m_DataReader;
	ReadFolder* m_SpriteReader;

	std::string m_CurrentTheme;

	std::vector<std::string> m_TextureThemes;
	std::vector<std::string> m_RequiredTextures;

	std::map <std::string, std::map<std::string, std::string> > m_DataMap;
	std::map <std::string, std::map<std::string, std::string> > m_TextureMap;


	std::map<std::string, SpriteInfo*> m_Sprites;

	void ReadFile(const std::string& spriteDataPath);
	void FillInInfo( const std::string& sentence);

	void SetRequiredTextures();

	std::string GetAttributeValue(const std::string& attrName, const std::string& element) const;
	int attributeLastIndex(const std::string& attrName, const std::string& element) const;
	Texture* InitializeTexture(const std::string& filename) const;

	int ToInt( const std::string& intStr ) const;
	float tofloat(const std::string& floatstr ) const;
	Point2f ToPoint2f(const std::string& point2fStr) const;
	Rectf ToRectf(const std::string& rectStr) const;

};

