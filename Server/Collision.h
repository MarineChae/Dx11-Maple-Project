#pragma once


class Collider;

class Collision
{

public:

	static bool PointToLine(TVector3 point, std::shared_ptr <Line> line);
	static TVector3 ClosestPoint(TVector3 point, std::shared_ptr <Line> line);

};

