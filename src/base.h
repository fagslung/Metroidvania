//
// Created by Martin Eberle on 30.12.2018.
//

#ifndef METROIDVANIA_BASE_H
#define METROIDVANIA_BASE_H


#include "dynamics.h"


/**
 *
 */
class MovableGamelet : public Gamelet, public IMoveListener {
public:
    static const auto DEFAULT_VEL_X = -20;
    static const auto DEFAULT_VEL_Y = -50;

    MovableGamelet();
    std::shared_ptr<MovableGamelet> shared_from_this();

    void setVelocity(const SDL_Point& velocity);
    const SDL_Point& getStartVelocity() const;
    bool getStopped();
    void setStopped(bool stopped);
    void setMilliVelocity(const MilliPoint& milliVelocity);
    const MilliPoint& getMilliVelocity() const;
    void setStartVelocity(const SDL_Point& startVelocity);
    void addToLevel(std::shared_ptr<Level> level) override;
    void removeFromLevel(std::shared_ptr<Level> level) override;
    void handleMoveEvent(const MoveEvent& ev) override;
    void prepareCollision(const CollisionEvent& ev) override;
    void performCollision() override;

private:
    MilliPoint milliVelocity;
    SDL_Point startVelocity{DEFAULT_VEL_X, DEFAULT_VEL_Y};
    bool stopped;
    SDL_Point collDir;
};


/**
 *
 */
class AbstractMovableBrick : public MovableGamelet {
public:
    AbstractMovableBrick();
};


/**
 *
 */
class MovableBrick : public AbstractMovableBrick {
public:
    MovableBrick();

};

class Spinner : public AbstractMovableBrick {
public:
    Spinner();

    void tickOccured(const TickEvent& ev) override;
    void prepareCollision(const CollisionEvent& ev) override;

private:
    int milliSpeed;
    int drift;
    std::unique_ptr<SDL_Point> collDir = nullptr;
};


#endif //METROIDVANIA_BASE_H
