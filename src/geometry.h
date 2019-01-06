//
// Created by Martin Eberle on 28.12.2018.
//

#ifndef METROIDVANIA_GEOMETRY_H
#define METROIDVANIA_GEOMETRY_H

#include <SDL.h>
#include <vector>
#include <memory>



/**
 *
 */
 struct Dimension {
     int width;
     int height;

     Dimension& operator= (const SDL_Rect& rect);
 };

/**
 *
 */
struct MilliPoint {
public:
    int x;
    int y;

    /*
     * constructors
     */
    MilliPoint();
    MilliPoint(int x, int y);
    MilliPoint(const MilliPoint& other) = default;
    MilliPoint(MilliPoint&& other) noexcept;
    /*explicit*/ MilliPoint(const SDL_Point& other);

    /*
     * operators
     */
    MilliPoint& operator+ (const MilliPoint& other);
    /*explicit*/ operator SDL_Point() const; // cast
    MilliPoint& operator= (const SDL_Point& other);
    MilliPoint& operator= (const MilliPoint& other) = default;
};


/**
 *
 */
struct MilliRect {
public:
    int x;
    int y;
    int w;
    int h;

    /*
     * constructors
     */
    MilliRect();
    MilliRect(int x, int y, int w, int h);
    MilliRect(const MilliRect& other) = default;
    MilliRect(MilliRect&& other) noexcept;
    /*explicit*/ MilliRect(const SDL_Rect& other);

    /*
     * operators
     */
    /*explicit*/ operator SDL_Rect() const; // cast
    MilliRect& operator= (const Dimension& other);

    /*
     * methods
     */
    MilliRect& setPosition(const MilliPoint& pos);
    MilliRect& setDimension(const MilliPoint& dim);
    MilliRect& move(const MilliPoint& pos);
    MilliRect& increaseDimension(const MilliPoint& dim);
    bool isIntersectingWith(MilliRect other) const;
};


/**
 *
 */
class SizedImage {
public:
    SizedImage() = default;

private:
    Dimension size;
    std::shared_ptr<SDL_Texture> image;
    std::vector<SDL_Rect> sprites;
};

#endif //METROIDVANIA_GEOMETRY_H
