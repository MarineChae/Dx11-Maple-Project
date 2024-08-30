#pragma once
enum OBJECT_TYPE;
class ObjectData
{
private:
	float m_fX;
	float m_fY;
	float m_fRotate;
	char	m_csName[80];
	OBJECT_TYPE m_objectType;

public:
	void Init(float x, float y ,float rotate, char* name, OBJECT_TYPE type);
	
public:
	float GetPosX() const{ return m_fX; }
	float GetPosY() const{ return m_fY; }
	float GetRotate() const { return m_fRotate; }
	char* GetName() { return m_csName; };
	OBJECT_TYPE GetObjectType();
	void SetRotate(float rotate) { m_fRotate = rotate; };
	void AddRotate(float value);

public:
	ObjectData();
	~ObjectData();
};

