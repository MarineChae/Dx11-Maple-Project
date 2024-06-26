#include "pch.h"
#include "Collision.h"
#include "Collider.h"




bool Collision::OBBCollision2D(std::shared_ptr<Collider> coll1 ,std::shared_ptr<Collider> coll2, TVector3 axis)
{
    float projection1 = 0;

    projection1 += std::abs((coll1->GetAxis(0).Dot(axis) * coll1->GetWidth()));
    projection1 += std::abs((coll1->GetAxis(1).Dot(axis) * coll1->GetHeight()));

    float projection2 = 0;

    projection2 += std::abs((coll2->GetAxis(0).Dot(axis) * coll2->GetWidth()));
    projection2 += std::abs((coll2->GetAxis(1).Dot(axis) * coll2->GetHeight()));

    auto a = coll1->GetTransform() - coll2->GetTransform();
    float distance = std::abs(a.Dot(axis));

    return distance <= projection1 + projection2;

}

bool Collision::PointToLine(TVector3 point, Line line)
{




    //직선의 방정식
    float A = line.To.y - line.From.y;
    float B = line.From.x - line.To.x;
    float C = (line.To.x * line.From.y) - (line.From.x * line.To.y);

    float dist = std::abs(A * point.x + B * point.y + C) / std::sqrt(A * A + B * B);

    bool ret = dist < 10;
    if(ret)
    OutputDebugString(std::to_wstring(dist).c_str());
    return ret;
}

TVector3 Collision::ClosestPoint(TVector3 point, Line line)
{
    TVector3 ret;
    float A = line.To.y - line.From.y;
    float B = line.From.x - line.To.x;
    float C = (line.To.x * line.From.y) - (line.From.x * line.To.y);

    double denom = A * A + B * B;
    ret.x = point.x - A * (A * point.x + B * point.y + C) / denom;
    ret.y = point.y - B * (A * point.x + B * point.y + C) / denom;



    return ret;
}
