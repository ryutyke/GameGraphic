#include "Character/Character.h"

Character::Character(_In_ const std::filesystem::path& filePath) 
	: Model(filePath)
	, m_targetPosition({ 0.0f, 0.0f, 0.0f, 0.0f })
	, m_currentPosition({ 0.0f, 0.0f, 0.0f, 0.0f })
	, m_moveLeftRight(0.0f)
	, m_moveBackForward(0.0f)
	, m_currentRotation(DOWN)
	, m_movementSpeed(10.0f)
{
}

void Character::HandleInput(_In_ const InputDirections& directions, _In_ FLOAT deltaTime)
{
	//XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	if (directions.bFront)
	{
		m_moveBackForward += m_movementSpeed * deltaTime;
		switch (m_currentRotation)
		{
		case UP:
			break;
		case RIGHT:
			RotateInObjectCoordinate(XMConvertToRadians(270), m_currentPosition);
			break;
		case DOWN:
			RotateInObjectCoordinate(XMConvertToRadians(180), m_currentPosition);
			break;
		case LEFT:
			RotateInObjectCoordinate(XMConvertToRadians(90), m_currentPosition);
			break;
		}
		m_currentRotation = UP;
	}
	if (directions.bBack)
	{
		m_moveBackForward -= m_movementSpeed * deltaTime;
		switch (m_currentRotation)
		{
		case DOWN:
			break;
		case UP:
			RotateInObjectCoordinate(XMConvertToRadians(180), m_currentPosition);
			break;
		case RIGHT:
			RotateInObjectCoordinate(XMConvertToRadians(90), m_currentPosition);
			break;
		case LEFT:
			RotateInObjectCoordinate(XMConvertToRadians(270), m_currentPosition);
			break;
		}
		m_currentRotation = DOWN;
	}
	if (directions.bRight)
	{
		m_moveLeftRight += m_movementSpeed * deltaTime;
		switch (m_currentRotation)
		{
		case RIGHT:
			break;
		case UP:
			RotateInObjectCoordinate(XMConvertToRadians(90), m_currentPosition);
			break;
		case DOWN:
			RotateInObjectCoordinate(XMConvertToRadians(270), m_currentPosition);
			break;
		case LEFT:
			RotateInObjectCoordinate(XMConvertToRadians(180), m_currentPosition);
			break;
		}
		m_currentRotation = RIGHT;
	}
	if (directions.bLeft)
	{
		m_moveLeftRight -= m_movementSpeed * deltaTime;
		switch (m_currentRotation)
		{
		case LEFT:
			break;
		case UP:
			RotateInObjectCoordinate(XMConvertToRadians(270), m_currentPosition);
			break;
		case RIGHT:
			RotateInObjectCoordinate(XMConvertToRadians(180), m_currentPosition);
			break;
		case DOWN:
			RotateInObjectCoordinate(XMConvertToRadians(90), m_currentPosition);
			break;
		}
		m_currentRotation = LEFT;
	}
}

void Character::Update(_In_ FLOAT deltaTime)
{
	m_targetPosition += m_moveLeftRight * DEFAULT_RIGHT;
	m_targetPosition += m_moveBackForward * DEFAULT_FORWARD;

	Translate(m_targetPosition - m_currentPosition);
	m_currentPosition = m_targetPosition;

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;
}
