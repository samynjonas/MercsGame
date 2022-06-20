#pragma once
class Camera final
{
public:
	enum class CameraMovement { freeze, follow };

	Camera(float width, float height);
	~Camera();
	
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;
	Camera(Camera&& other) = delete;
	Camera& operator=(Camera&& other) = delete;

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target);
	void changeCameraMovement(CameraMovement cameraMovement);
	void SnapAndFreeze( Point2f centerCamera );

	Rectf getCameraView() const;
private:
	float m_Width;
	float m_Height;

	Point2f m_CameraPos;
	Point2f m_TransitionPoint;

	Rectf m_LevelBoundaries;
	
	CameraMovement m_CameraMovement{ CameraMovement::follow };

	Point2f Track(const Rectf& target);
	void Clamp(Point2f& bottomLeftPos);

};

