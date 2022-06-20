#include "pch.h"
#include "Camera.h"

Camera::Camera(float width, float height)
	: m_Width{ width }
	, m_Height{ height }
	, m_CameraPos{  }
{

}

Camera::~Camera()
{

}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Transform(const Rectf& target)
{
	if (m_CameraMovement == CameraMovement::follow)
	{
		m_CameraPos = Track(target);
		Clamp(m_CameraPos);
	}
	else if (m_CameraMovement == CameraMovement::freeze)
	{
		float cameraSpeed{ 0.5f };
		if (m_CameraPos.x > m_TransitionPoint.x)
		{
			m_CameraPos.x -= cameraSpeed;
		}
		else if (m_CameraPos.x < m_TransitionPoint.x)
		{
			m_CameraPos.x += cameraSpeed;
		}

		if (m_CameraPos.y > m_TransitionPoint.y)
		{
			m_CameraPos.y -= cameraSpeed;
		}
		else if (m_CameraPos.y < m_TransitionPoint.y)
		{
			m_CameraPos.y += cameraSpeed;
		}
	}
	//Placement
	glTranslatef(-m_CameraPos.x, -m_CameraPos.y, 0);
}

Point2f Camera::Track(const Rectf& target)
{
	return Point2f{ (target.left + target.width / 2) - m_Width / 2, (target.bottom + target.height / 2) - m_Height / 2};
}
void Camera::Clamp(Point2f& bottomLeftPos)
{
	if (bottomLeftPos.x < m_LevelBoundaries.left)
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}
	else if (bottomLeftPos.x + m_Width > m_LevelBoundaries.left + m_LevelBoundaries.width)
	{
		bottomLeftPos.x = m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width;
	}

	if (bottomLeftPos.y + m_Height > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height;
	}
	else if (bottomLeftPos.y <= m_LevelBoundaries.bottom)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}
}

void Camera::changeCameraMovement(CameraMovement newCameraMovement)
{
	m_CameraMovement = newCameraMovement;
}
void Camera::SnapAndFreeze(Point2f centerCamera)
{
	m_CameraMovement = CameraMovement::freeze;

	m_TransitionPoint.x = centerCamera.x - m_Width / 2;
	m_TransitionPoint.y = centerCamera.y - m_Height / 2;
}

Rectf Camera::getCameraView() const
{
	return Rectf{ m_CameraPos.x, m_CameraPos.y, m_Width, m_Height };
}