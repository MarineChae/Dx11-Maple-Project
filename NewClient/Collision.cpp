#include "pch.h"
#include "Collision.h"
#include "Collider.h"


bool Collision::isLineIntersectingOBB(std::shared_ptr<Line> line, std::shared_ptr<Collider> coll1obb, float coefficient)
{

    // 선을 OBB의 로컬 좌표계로 변환
    TVector3 localLineStart = line->From - (coll1obb->GetTransform() - TVector3(0, coefficient, 0));
    TVector3 localLineEnd = line->To - (coll1obb->GetTransform() - TVector3(0, coefficient, 0));

    // OBB의 각 축에 대해 선의 시작점과 끝점을 투영
    float minProj = INFINITY, maxProj = -INFINITY;
    for (int i = 0; i < 2; ++i) {
        float startProj = coll1obb->GetAxis(i).Dot(localLineStart);
        float endProj = coll1obb->GetAxis(i).Dot(localLineEnd);

        float minVal = min(startProj, endProj);
        float maxVal = max(startProj, endProj);

        // 투영 결과가 OBB의 반경(extents) 내에 있는지 확인 
        if (minVal > coll1obb->GetWidth() || maxVal < -coll1obb->GetWidth()) {
            return false;
        }

        // 각 축의 투영 범위를 갱신
        minProj = min(minProj, minVal);
        maxProj = max(maxProj, maxVal);
    }

    return true;
}



bool Collision::OBBCollision2D(std::shared_ptr<Collider> coll1, std::shared_ptr<Collider> coll2, TVector3 axis)
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

bool Collision::PointToLine(TVector3 point, std::shared_ptr<Line> line)
{

    //직선의 방정식
    float A = line->To.y - line->From.y;
    float B = line->From.x - line->To.x;
    float C = (line->To.x * line->From.y) - (line->From.x * line->To.y);

    float dist = std::abs(A * point.x + B * point.y + C) / std::sqrt(A * A + B * B);
    if (dist > 20 || dist < -20)
        return false;

    float minX = min(line->From.x, line->To.x);
    float maxX = max(line->From.x, line->To.x);
    float minY = min(line->From.y, line->To.y);
    float maxY = max(line->From.y, line->To.y);

    bool isWithinSegment = (point.x >= minX && point.x <= maxX) && (point.y >= minY - 50 && point.y <= maxY + 50);




    return isWithinSegment;
}

TVector3 Collision::ClosestPoint(TVector3 point, std::shared_ptr<Line> line)
{
    TVector3 ret;
    float A = line->To.y - line->From.y;
    float B = line->From.x - line->To.x;
    float C = (line->To.x * line->From.y) - (line->From.x * line->To.y);

    double denom = A * A + B * B;
    ret.x = point.x - A * (A * point.x + B * point.y + C) / denom;
    ret.y = point.y - B * (A * point.x + B * point.y + C) / denom;



    return ret;
}


bool Collision::LinesIntersect(Line line1, Line line2)
{
    float A1 = line1.To.y - line1.From.y;
    float B1 = line1.From.x - line1.To.x;
    float C1 = A1 * line1.From.x + B1 * line1.From.y;

    float A2 = line2.To.y - line2.From.y;
    float B2 = line2.From.x - line2.To.x;
    float C2 = A2 * line2.From.x + B2 * line2.From.y;

    float denominator = A1 * B2 - A2 * B1;

    if (std::abs(denominator) < std::numeric_limits<float>::epsilon()) {
        // 두 선분이 평행한 경우
        return false;
    }

    float intersectX = (C1 * B2 - C2 * B1) / denominator;
    float intersectY = (A1 * C2 - A2 * C1) / denominator;

    // 교차점이 두 선분 내에 있는지 확인
    bool isWithinLine1 = (min(line1.From.x, line1.To.x) <= intersectX && intersectX <= max(line1.From.x, line1.To.x)) &&
        (min(line1.From.y, line1.To.y) <= intersectY && intersectY <= max(line1.From.y, line1.To.y));

    bool isWithinLine2 = (min(line2.From.x, line2.To.x) <= intersectX && intersectX <= max(line2.From.x, line2.To.x)) &&
        (min(line2.From.y, line2.To.y) <= intersectY && intersectY <= max(line2.From.y, line2.To.y));

    return isWithinLine1 && isWithinLine2;

}
