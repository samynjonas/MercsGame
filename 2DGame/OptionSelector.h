#pragma once
#include "MouseButton.h"
#include "Texture.h"
#include "ResourceManager.h"
#include <vector>

class OptionSelector final
{
public:
	enum class HorizontalPlacement { left, center, right };
	enum class VerticalPlacement { top, center, bottom };

	OptionSelector(std::vector<std::string>& options, int textSize, Color4f mainColor, HorizontalPlacement horizontal, VerticalPlacement vertical, Rectf window, ResourceManager* resourceManager);
	OptionSelector(std::vector<std::string>& options, int textSize, Color4f mainColor, Point2f pos, Rectf window, ResourceManager* resourceManager);
	OptionSelector(std::vector<std::string>& options, int textSize, Color4f mainColor, HorizontalPlacement horizontal, float vertical, Rectf window, ResourceManager* resourceManager);
	OptionSelector(std::vector<std::string>& options, int textSize, Color4f mainColor, float horizontal, VerticalPlacement vertical, Rectf window, ResourceManager* resourceManager);

	~OptionSelector();
	OptionSelector(const OptionSelector& other) = delete;
	OptionSelector& operator=(const OptionSelector& other) = delete;
	OptionSelector(OptionSelector&& other) = delete;
	OptionSelector& operator=(OptionSelector&& other) = delete;

	void Draw(bool showBackGround) const;
	void Update();

	std::string getCurrentText() const;
	int getCurrentIndex() const;

	void setCurrentSelected(std::string text);

private:
	Point2f m_Position;
	int m_TextSize;
	Rectf m_Window;
	Color4f m_Color;

	ResourceManager* m_ResourceManager;

	std::vector<std::string>& m_Options;

	MouseButton* m_TabLeft;
	MouseButton* m_TabRight;

	Texture* m_CurrentTabTexture;
	int m_CurrentIndex;


	float VerticalAlligment(VerticalPlacement& vertical, Rectf window, int textSize) const;
	float HorizontalAlligment(HorizontalPlacement& horizontal, Rectf window, int textSize) const;

	void UpdateText();
};

