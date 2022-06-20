#pragma once
#include "Menu.h"

#include "OptionSelector.h"
#include "MouseButton.h"

#include "ResourceManager.h"

class OptionMenu final : public Menu
{
public:
	OptionMenu( Color4f backgroundColor, Color4f buttonColor, buttonBehaviour buttonBehaviour, ResourceManager* resourceManager, Rectf window );
	virtual ~OptionMenu();
	OptionMenu(const OptionMenu& other) = delete;
	OptionMenu& operator=(const OptionMenu& other) = delete;
	OptionMenu(OptionMenu&& other) = delete;
	OptionMenu& operator=(OptionMenu&& other) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw(Rectf window) const override;

	bool isBackPressed() const;

	std::vector<std::string> getVolumes() const;

	void Reset();

private:
	ResourceManager* m_ResourceManager;

	OptionSelector* m_SoundSelector;
	OptionSelector* m_TextureSelector;
	OptionSelector* m_VolumeSelector;
	OptionSelector* m_BackgroundVolumeSelector;



	std::vector<std::string> m_SoundThemes;
	std::vector<std::string> m_TextureThemes;
	std::vector<std::string> m_Volumes;

	MouseButton* m_BackButton;

	bool m_IsBackPressed;
};

