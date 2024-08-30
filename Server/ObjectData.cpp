#include"Netstd.h"
#include "ObjectData.h"

void ObjectData::Init(float x, float y, float rotate, char* name, OBJECT_TYPE type)
{
	m_fX =x;
	m_fY =y;
	m_fRotate = rotate;
	strcpy_s(m_csName, name);
	m_objectType = type ;
}

OBJECT_TYPE ObjectData::GetObjectType()
{
	return m_objectType;
}

void ObjectData::AddRotate(float value)
{ 
	if (m_fRotate >= 360 )
		m_fRotate=0;
	m_fRotate += value;
};


ObjectData::ObjectData()
{
}

ObjectData::~ObjectData()
{
}
