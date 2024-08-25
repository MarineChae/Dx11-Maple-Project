#include"Netstd.h"
#include "Collision.h"


bool Collision::isLineIntersectingOBB(std::shared_ptr<Line> line, ColliderData coll1obb, float coefficient)
{

    // ���� OBB�� ���� ��ǥ��� ��ȯ
    TVector3 localLineStart = line->From - (coll1obb.GetPos() - TVector3(0, coefficient, 0));
    TVector3 localLineEnd = line->To - (coll1obb.GetPos() - TVector3(0, coefficient, 0));

    // OBB�� �� �࿡ ���� ���� �������� ������ ����
    float minProj = INFINITY, maxProj = -INFINITY;
    for (int i = 0; i < 2; ++i) {
        float startProj = coll1obb.GetAxis(i).Dot(localLineStart);
        float endProj = coll1obb.GetAxis(i).Dot(localLineEnd);

        float minVal = min(startProj, endProj);
        float maxVal = max(startProj, endProj);

        // ���� ����� OBB�� �ݰ�(extents) ���� �ִ��� Ȯ�� 
        if (minVal > coll1obb.GetWidth() || maxVal < -coll1obb.GetWidth()) {
            return false;
        }

        // �� ���� ���� ������ ����
        minProj = min(minProj, minVal);
        maxProj = max(maxProj, maxVal);
    }

    return true;
}


bool Collision::PointToLine(TVector3 point, std::shared_ptr<Line> line)
{

    //������ ������
    float A = line->To.y - line->From.y;
    float B = line->From.x - line->To.x;
    float C = (line->To.x * line->From.y) - (line->From.x * line->To.y);

    float dist = std::abs(A * point.x + B * point.y + C) / std::sqrt(A * A + B * B);
    if (dist > 30 || dist < -30)
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

