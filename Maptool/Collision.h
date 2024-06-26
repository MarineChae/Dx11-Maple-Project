#pragma once


class Collider;

class Collision
{

public:
	static bool OBBCollision2D(std::shared_ptr<Collider> coll1, std::shared_ptr<Collider> coll2, TVector3 axis);
	static bool PointToLine(TVector3 point, Line line);
	static TVector3 ClosestPoint(TVector3 point, Line line);

};

