#include <algorithm>
#include "Vectorizer/Math.h"

namespace Vectorizer
{
    namespace Math
    {
        float pointsDistance(Point a, Point b)
        {
            float deltaX = b.x - a.x;
            float deltaY = b.y - a.y;

            return std::sqrt(deltaX * deltaX + deltaY * deltaY);
        }
        float pointToSegmentDistance(Segment segment, Point p)
        {
            if (segment.start == segment.end)
            {
                return pointsDistance(p, segment.start);
            }
            Point segmentVector = segment.end - segment.start;
            Point pointVector = p - segment.start;
            float dotProd = dotProduct(segmentVector, pointVector);
            float dotProdAB = dotProduct(segmentVector, segmentVector);
            float t = dotProd / dotProdAB;
            float tClamped = std::clamp(t, 0.0f, 1.0f);
            Point pProjection = segment.start + segmentVector * tClamped;
            return pointsDistance(p, pProjection);
        }
    }
}
