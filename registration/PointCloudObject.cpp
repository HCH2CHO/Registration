#include "PointCloudObject.h"

PointCloudObject::PointCloudObject()
{
}

PointCloudObject::~PointCloudObject()
{
}

void PointCloudObject::initial()
{
	m_pointCloud.reset(new PointCloudT);
	m_pointCloud->resize(0);
	m_strFilePathName = "";
	m_strFileName = "";
	m_strDirectory = "";
}