#pragma once


class Collider;

class Collision
{

public:
	static bool isLineIntersectingOBB(std::shared_ptr <Line> line, std::shared_ptr<Collider> coll1obb, float coefficient = 0.0f);
	static bool OBBCollision2D(std::shared_ptr<Collider> coll1, std::shared_ptr<Collider> coll2, TVector3 axis);
	static bool PointToLine(TVector3 point, std::shared_ptr <Line> line);
	static TVector3 ClosestPoint(TVector3 point, std::shared_ptr <Line> line);
	static bool LinesIntersect(Line line1, Line line2);
};

