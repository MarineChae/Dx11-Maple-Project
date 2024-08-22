#include"Netstd.h"
#include "Collision.h"




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

    bool isWithinSegment = (point.x >= minX  && point.x <= maxX) && (point.y >= minY -50 && point.y <= maxY + 50);




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

