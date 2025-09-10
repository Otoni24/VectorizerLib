#pragma once

#include <iostream>
#include <vector>

namespace Vectorizer
{
    namespace Math
    {
        struct Point {
            float x, y;
            bool operator==(const Point& other) const {
                return std::fabs(x - other.x) < 0.0001f && std::fabs(y - other.y) < 0.0001f;
            }
            bool operator!=(const Point& other) const {
                return !(*this == other);
            }
            Point operator+(const Point& other) const {
                return Point{ x + other.x, y + other.y };
            }
            Point operator-(const Point& other) const {
                return Point{ x - other.x, y - other.y };
            }
        };

        using Chain = std::vector<Math::Point>;

        inline Point operator*(const Point& p, float f) {
            return { p.x * f, p.y * f };
        }

        inline Point operator*(float f, const Point& p) {
            return p * f;
        }

        struct Segment {
            Point start, end;
        };

        inline float dotProduct(Point vector1, Point vector2)
        {
            return vector1.x * vector2.x + vector1.y * vector2.y;
        }

        float pointsDistance(Point a, Point b);

        float pointToSegmentDistance(Segment segment, Point p);
    }
}