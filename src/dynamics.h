//
// Created by Martin Eberle on 24.12.2018.
//

#ifndef METROIDVANIA_DYNAMICS_H
#define METROIDVANIA_DYNAMICS_H

#include <stdio.h>
#include <string>
#include <SDL.h>
#include "gameutil.h"


/**
 * Interface
 */
class IEventSource {
public:
    virtual ~IEventSource() {}

    virtual std::string toString() = 0;
};


/**
 * Interface
 */
class IEventListener {
public:
    virtual ~IEventListener() {}
};


/**
 *
 */
class Event {
public:
    Event(std::shared_ptr<IEventSource> source) : source(source) {}

    std::shared_ptr<IEventSource> getSource() {
        return source;
    }

    virtual std::string toString() {
        std::string classname = typeid(this).name();
        return classname + ", source=" + source->toString();
    }

private:
    std::shared_ptr<IEventSource> source;
};





/**
 * Forward declaration
 */
class ICollider;


/**
 *
 */
class CollisionEvent : Event {
public:
    CollisionEvent(std::shared_ptr<IEventSource> source, std::shared_ptr<ICollider> obj1, std::shared_ptr<ICollider> obj2, const SDL_Point& vec1to2) : obj1(obj1), obj2(obj2), vec1to2(vec1to2), Event(source), vec2to1{-vec1to2.x, -vec1to2.y} {}

    std::shared_ptr<ICollider> getCollider1() {
        return obj1;
    }

    std::shared_ptr<ICollider> getCollider2() {
        return obj2;
    }

    std::shared_ptr<ICollider> getOtherCollider(std::shared_ptr<ICollider> curCollider) {
        if (curCollider == obj1) {
            return obj2;
        }

        if (curCollider == obj2) {
            return obj1;
        }

        throw GameException("wrong collider");
    }

    SDL_Point& getCollisionDirection(std::shared_ptr<ICollider> curCollider) {
        if (curCollider == obj1) {
            return vec2to1;
        }

        if (curCollider == obj2) {
            return vec1to2;
        }

        throw GameException("wrong collider");
    }

private:
    std::shared_ptr<ICollider> obj1, obj2;
    SDL_Point vec1to2, vec2to1;
};


/**
 *
 */
class ICollider : IEventListener {
public:
    virtual ~ICollider() {}

    virtual void prepareCollision(std::shared_ptr<CollisionEvent> ev) = 0;
    virtual void performCollision() = 0;
};


/**
 *
 */
class ICollisionListener : IEventListener {
public:
    virtual ~ICollisionListener() {}
    virtual void collisionOccured(std::shared_ptr<CollisionEvent> ev) = 0;
};


/**
 *
 */
class Gamelet {

};


/**
 *
 */
class GameEvent : Event {
public:
    GameEvent(std::shared_ptr<IEventSource> source) : Event(source) {}
};


/**
 *
 */
class IGameListener : IEventListener {
public:
    virtual ~IGameListener() {}
    virtual void handleGameEvent(std::shared_ptr<GameEvent> ev) = 0;

};


/**
 *
 */
class LevelDoneEvent : Event {
public:
    LevelDoneEvent(std::shared_ptr<IEventSource> source) : Event(source) {}
};


/**
 *
 */
class ILevelDoneListener : IEventListener {
public:
    virtual ~ILevelDoneListener() {}
    virtual void handleLevelDoneEvent(std::shared_ptr<LevelDoneEvent> ev) = 0;

};


/**
 *
 */
class LivesEvent : Event {
public:

    LivesEvent(std::shared_ptr<IEventSource> source, int diff) : Event(source), diff(diff) {}

    int getDiff() {
        return diff;
    }

private:
    int diff;
};


/**
 *
 */
class ILivesListener : IEventListener {
public:
    virtual ~ILivesListener() {}
    virtual void handleLivesEvent(std::shared_ptr<LivesEvent> ev) = 0;

};


/**
 *
 */
class PointsEvent : GameEvent {
public:
    PointsEvent(std::shared_ptr<IEventSource> source, int diff) : GameEvent(source), diff(diff) {}

    int getDiff() {
        return diff;
    }

private:
    int diff;
};


/**
 *
 */
class IPointsListener : IEventListener {
public:
    virtual ~IPointsListener() {}
    virtual void handlePointsEvent(std::shared_ptr<PointsEvent> ev) = 0;

};

/**
 *
 */
class DynamicFramework : IGameListener, ILevelDoneListener, ILivesListener, IPointsListener, IEventSource {
public:
    virtual std::string toString() override {
        std::string classname = typeid(this).name();
        return  classname;
    };

    virtual void handleGameEvent(std::shared_ptr<GameEvent> ev) override {
        if (PointsEvent* pev = dynamic_cast<PointsEvent*>(ev.get())) {
            std::cout << "Points event handled";
        }
    }

    virtual void handleLevelDoneEvent(std::shared_ptr<LevelDoneEvent> ev) override {
    }

    virtual void handleLivesEvent(std::shared_ptr<LivesEvent> ev) override {
    }

    virtual void handlePointsEvent(std::shared_ptr<PointsEvent> ev) override {
    }


};


#endif //METROIDVANIA_DYNAMICS_H
