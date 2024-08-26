#pragma once
#include"ColliderData.h"

class Collider;

class Collision
{

public:
	static bool OBBCollision2D(ColliderData coll1, ColliderData coll2, TVector3 axis);
	static bool isLineIntersectingOBB(std::shared_ptr<Line> line, ColliderData coll1obb, float coefficient);
	static bool PointToLine(TVector3 point, std::shared_ptr <Line> line);
	static TVector3 ClosestPoint(TVector3 point, std::shared_ptr <Line> line);

};

