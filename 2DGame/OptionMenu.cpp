#include "pch.h"
#include "OptionMenu.h"

OptionMenu::OptionMenu(Color4f backgroundColor, Color4f buttonColor, buttonBehaviour buttonBehaviour, ResourceManager* resourceManager, Rectf window)
	: Menu{ backgroundColor, buttonBehaviour }
	, m_ResourceManager{ resourceManager }
	, m_IsBackPressed{ false }
{
	m_SoundThemes	= m_ResourceManager->Sounds()->getSoundThemes();
	m_TextureThemes = m_ResourceManager->Sprites()->getThemes();
	
	m_SoundSelector		= new OptionSelector{ m_SoundThemes,	20, buttonColor, OptionSelector::HorizontalPlacement::center, window.bottom + window.height - window.height / 3, window, resourceManager };
	m_TextureSelector	= new OptionSelector{ m_TextureThemes,	20, buttonColor, OptionSelector::HorizontalPlacement::center, window.bottom + window.height - window.height / 3 * 1.2f, window, resourceManager };

	m_Volumes = getVolumes();
	m_VolumeSelector = new OptionSelector{ m_Volumes, 20, buttonColor, OptionSelector::HorizontalPlacement::center, window.bottom + window.height - window.height / 3 * 1.4f, window, resourceManager};
	m_BackgroundVolumeSelector = new OptionSelector{ m_Volumes, 20, buttonColor, OptionSelector::HorizontalPlacement::center, window.bottom + window.height - window.height / 3 * 1.6f, window, resourceManager };
	
	m_ResourceManager->Sounds()->SetVolume(stoi(m_Volumes[2]));
	m_VolumeSelector->setCurrentSelected(m_Volumes[2]);

	m_ResourceManager->Sounds()->getBackground()->SetVolume(stoi(m_Volumes[2]));
	m_BackgroundVolumeSelector->setCurrentSelected(m_Volumes[2]);


	m_BackButton = new MouseButton( Rectf{ 50, 50, 100, 50 }, "BACK", backgroundColor, 30, buttonColor, window, 0 );
}

OptionMenu::~OptionMenu()
{
	m_ResourceManager = nullptr;

	delete m_SoundSelector;
	m_SoundSelector = nullptr;

	delete m_TextureSelector;
	m_TextureSelector = nullptr;

	delete m_BackButton;
	m_BackButton = nullptr;
}

void OptionMenu::Update(float elapsedSec)
{
	m_SoundSelector->Update();
	if (m_ResourceManager->Sounds()->getCurrentSoundTheme() != m_SoundSelector->getCurrentText())
	{
		m_ResourceManager->Sounds()->SelectSoundTheme(m_SoundSelector->getCurrentText());
	}

	m_TextureSelector->Update();
	if (m_ResourceManager->Sprites()->getCurrentTheme() != m_TextureSelector->getCurrentText())
	{
		m_ResourceManager->Sprites()->setTextureTheme(m_TextureSelector->getCurrentText());
	}
	
	m_VolumeSelector->Update();
	if (m_ResourceManager->Sounds()->GetVolume() != std::stoi(m_VolumeSelector->getCurrentText()))
	{
		m_ResourceManager->Sounds()->SetVolume(std::stoi(m_VolumeSelector->getCurrentText()));
	}

	m_BackgroundVolumeSelector->Update();
	if (m_ResourceManager->Sounds()->getBackground()->GetVolume() != std::stoi(m_BackgroundVolumeSelector->getCurrentText()))
	{
		m_ResourceManager->Sounds()->getBackground()->SetVolume(std::stoi(m_BackgroundVolumeSelector->getCurrentText()));
	}

	if (m_BackButton->IsButtonPressed())
	{
		m_IsBackPressed = true;
	}
}

void OptionMenu::Draw(Rectf window) const
{
	m_SoundSelector->Draw(false);
	m_TextureSelector->Draw(false);
	m_VolumeSelector->Draw(false);
	m_BackgroundVolumeSelector->Draw(false);
	m_BackButton->Draw(true);
}

bool OptionMenu::isBackPressed() const
{
	return m_IsBackPressed;
}

void OptionMenu::Reset()
{
	m_IsBackPressed = false;
}

std::vector<std::string> OptionMenu::getVolumes() const
{	
	int steps{ 5 };
	int minVolume{ 0 };
	int maxVolume{ 100 };

	std::vector<std::string> volumes;
	for (int index = minVolume; index * steps <= maxVolume; index++)
	{
		volumes.push_back(std::to_string(index * steps));
	}
	return volumes;
}
