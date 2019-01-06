//
// Created by Martin Eberle on 30.12.2018.
//

#include "base.h"

MovableGamelet::MovableGamelet() {
    setVelocity(getStartVelocity());
}

void MovableGamelet::setVelocity(const SDL_Point &velocity) {
    milliVelocity = velocity;
}

const SDL_Point &MovableGamelet::getStartVelocity() const {
    return startVelocity;
}

bool MovableGamelet::getStopped() {
    return stopped;
}

void MovableGamelet::setMilliVelocity(const MilliPoint &milliVelocity) {
    this->milliVelocity = milliVelocity;
}

const MilliPoint &MovableGamelet::getMilliVelocity() const {
    return milliVelocity;
}

void MovableGamelet::setStartVelocity(const SDL_Point &startVelocity) {
    this->startVelocity = startVelocity;
}

void MovableGamelet::addToLevel(std::shared_ptr<Level> level) {
    Gamelet::addToLevel(level);

    if (level != nullptr) {
        level->addMoveListener(shared_from_this());
    }
}

std::shared_ptr<MovableGamelet> MovableGamelet::shared_from_this() {
    return std::static_pointer_cast<MovableGamelet>(Gamelet::shared_from_this());
}

void MovableGamelet::removeFromLevel(std::shared_ptr<Level> level) {
    Gamelet::removeFromLevel(level);

    if (level != nullptr) {
        level->removeMoveListener(shared_from_this());
    }
}

void MovableGamelet::handleMoveEvent(const MoveEvent &ev) {
    switch (ev.getType()) {
        case MoveEvent::BEGIN:
            break;
        case MoveEvent::MOVE:
            if (!getStopped()) {
                if (auto dynamicFramework = std::dynamic_pointer_cast<DynamicFramework>(ev.getSource())) {
                    MilliPoint move = getMilliVelocity();
                    int moveDivider = ev.getMoveDivider();
                    if (moveDivider > 0) {
                        move.x /= moveDivider;
                        move.y /= moveDivider;
                    }
                    auto collisionEvent = dynamicFramework->wantMove(shared_from_this(), move);
                    if (move.x != 0 || move.y != 0) {
                        MilliPoint pos = getCalculatedMilliPos();
                        move = move + pos;
                        setCalculatedMilliPos(move);
                    }
                    if (collisionEvent != nullptr) {
                        dynamicFramework->fireCollisionEvent(*collisionEvent.get());
                    }
                }
            }
            break;
        case MoveEvent::UPDATE:
            break;
        case MoveEvent::END:
            setStopped(false);
            break;
    }
}

void MovableGamelet::setStopped(bool stopped) {
    this->stopped = stopped;
}

void MovableGamelet::prepareCollision(const CollisionEvent &ev) {
    Gamelet::prepareCollision(ev);

    // alle fuer performCollision benoetigten Daten von otherColl holen
    auto otherColl = ev.getOtherCollider(shared_from_this());
    this->collDir = SDL_Point{ev.getCollisionDirection(shared_from_this())};
}

void MovableGamelet::performCollision() {
    // nicht mehr auf den anderen Collider zugreifen
    Gamelet::performCollision();

    setStopped(true);
    // collDir fuer Abprallen verwenden => SetVelocity
    MilliPoint vel = getMilliVelocity();
    if (collDir.x < 0 && vel.x > 0) {
        vel.x = -vel.x;
    }
    if (collDir.x > 0 && vel.x < 0) {
        vel.x = -vel.x;
    }
    if (collDir.y < 0 && vel.y > 0) {
        vel.y = -vel.y;
    }
    if (collDir.y > 0 && vel.y < 0) {
        vel.y = -vel.y;
    }
    setMilliVelocity(vel);
}

AbstractMovableBrick::AbstractMovableBrick() {
    setMilliVelocity(MilliPoint{SDL_Point{5 ,0}});
}

MovableBrick::MovableBrick() : AbstractMovableBrick() {

}

Spinner::Spinner() : milliSpeed(5000), drift(200) {
    setMilliVelocity(MilliPoint{SDL_Point{0, 1}});
}

void Spinner::tickOccured(const TickEvent &ev) {
    Gamelet::tickOccured(ev);

    MilliPoint vel = (collDir == nullptr) ? getMilliVelocity() : MilliPoint{*collDir};
    collDir = nullptr;
    vel.x += vel.y * drift / 1000;
    vel.y -= vel.x * drift / 1000;
    auto len = (float) sqrt(vel.x * vel.x + vel.y * vel.y);
    if (len == 0.0) {
        vel.x = milliSpeed;
    } else {
        vel.x *= (float)milliSpeed / len;
        vel.y *= (float)milliSpeed / len;
    }
    setMilliVelocity(vel);
}

void Spinner::prepareCollision(const CollisionEvent &ev) {
    MovableGamelet::prepareCollision(ev);

    collDir = std::make_unique<SDL_Point>(ev.getCollisionDirection(shared_from_this()));
}
