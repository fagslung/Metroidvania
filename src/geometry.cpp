//
// Created by Martin Eberle on 28.12.2018.
//

#include "geometry.h"
#include <utility>


MilliPoint::MilliPoint() : x(0), y(0) {}

MilliPoint::MilliPoint(int x, int y) : x(x), y(y) {}

MilliPoint::MilliPoint(MilliPoint &&other) noexcept : x(std::exchange(other.x, 0)), y(std::exchange(other.y, 0)) {}

MilliPoint::MilliPoint(const SDL_Point &other) : x(other.x * 1000), y(other.y * 1000) {}

MilliPoint &MilliPoint::operator+(const MilliPoint &other) {
    x = x + other.x;
    y = y + other.y;

    return *this;
}

MilliPoint::operator SDL_Point() const {
    SDL_Point p{};
    p.x = (x > 0) ? (x + 500) / 1000 : (x - 499) / 1000;
    p.y = (y > 0) ? (y + 500) / 1000 : (y - 499) / 1000;

    return p;
}

MilliRect::MilliRect() : x(0), y(0), w(0), h(0) {}

MilliRect::MilliRect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

MilliRect::MilliRect(MilliRect &&other) noexcept : x(std::exchange(other.x, 0)), y(std::exchange(other.y, 0)), w(std::exchange(other.w, 0)), h(std::exchange(other.h, 0)) {}

MilliRect::MilliRect(const SDL_Rect &other) : x(other.x * 1000), y(other.y * 1000), w(other.w * 1000), h(other.h * 1000) {}

MilliRect::operator SDL_Rect() const {
    SDL_Rect r{};
    r.x = (x > 0) ? (x + 500) / 1000 : (x - 499) / 1000;
    r.y = (y > 0) ? (y + 500) / 1000 : (y - 499) / 1000;
    r.w  = (w > 0) ? (w + 500) / 1000 : (w - 499) / 1000;
    r.h = (h > 0) ? (h + 500) / 1000 : (h - 499) / 1000;

    return r;
}

MilliRect &MilliRect::setPosition(const MilliPoint &pos) {
    x = pos.x;
    y = pos.y;

    return *this;
}

MilliRect &MilliRect::setDimension(const MilliPoint &dim) {
    w = dim.x;
    h = dim.y;

    return *this;
}

MilliRect &MilliRect::move(const MilliPoint &pos) {
    x += pos.x;
    y += pos.y;

    return *this;
}

MilliRect &MilliRect::increaseDimension(const MilliPoint &dim) {
    if (dim.x < 0) {
        x += dim.x;
        w -= dim.x;
    } else {
        w += dim.x;
    }

    if (dim.y < 0) {
        y += dim.y;
        h -= dim.y;
    } else {
        h += dim.y;
    }

    return *this;
}

bool MilliRect::isIntersectingWith(MilliRect other) {
    return (x + w > other.x &&
            other.x + other.w > x &&
            y + h > other.y &&
            other.y + other.h > y);
}
