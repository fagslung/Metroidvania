//
// Created by Martin Eberle on 24.12.2018.
//

#ifndef METROIDVANIA_DYNAMICS_H
#define METROIDVANIA_DYNAMICS_H

#include <stdio.h>
#include <string>
#include <SDL.h>
#include "gameutil.h"
#include <vector>


/**
 * Interface
 */
class IEventSource {
public:
    virtual ~IEventSource() = default;

    virtual std::string toString() = 0;
};


/**
 * Interface
 */
class IEventListener {
public:
    virtual ~IEventListener() = default;
};


/**
 *
 */
class Event {
public:
    explicit Event(std::shared_ptr<IEventSource> source);
    virtual ~Event() = default;

    std::shared_ptr<IEventSource> getSource();
    virtual std::string toString();

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
class CollisionEvent : public Event {
public:
    CollisionEvent(std::shared_ptr<IEventSource> source, std::shared_ptr<ICollider> obj1, std::shared_ptr<ICollider> obj2, const SDL_Point& vec1to2);
    ~CollisionEvent() override = default;

    std::shared_ptr<ICollider> getCollider1();
    std::shared_ptr<ICollider> getCollider2();
    std::shared_ptr<ICollider> getOtherCollider(std::shared_ptr<ICollider> curCollider);
    SDL_Point& getCollisionDirection(std::shared_ptr<ICollider> curCollider);

private:
    std::shared_ptr<ICollider> obj1;
    std::shared_ptr<ICollider> obj2;
    SDL_Point vec1to2;
    SDL_Point vec2to1;
};


/**
 *
 */
class ICollider : public IEventListener {
public:
    ~ICollider() override = default;

    virtual void prepareCollision(std::shared_ptr<CollisionEvent> ev) = 0;
    virtual void performCollision() = 0;
};


/**
 *
 */
class ICollisionListener : public IEventListener {
public:
    ~ICollisionListener() override = default;

    virtual void collisionOccured(std::shared_ptr<CollisionEvent> ev) = 0;
};


/**
 *
 */
class GameEvent : public Event {
public:
    explicit GameEvent(std::shared_ptr<IEventSource> source);
    ~GameEvent() override = default;
};


/**
 *
 */
class IGameListener : public IEventListener {
public:
    ~IGameListener() override = default;

    virtual void handleGameEvent(std::shared_ptr<GameEvent> ev) = 0;

};


/**
 *
 */
class LevelDoneEvent : public Event {
public:
    explicit LevelDoneEvent(std::shared_ptr<IEventSource> source);
    ~LevelDoneEvent() override = default;
};


/**
 *
 */
class ILevelDoneListener : public IEventListener {
public:
    ~ILevelDoneListener() override = default;

    virtual void handleLevelDoneEvent(std::shared_ptr<LevelDoneEvent> ev) = 0;

};


/**
 *
 */
class LivesEvent : public Event {
public:
    LivesEvent(std::shared_ptr<IEventSource> source, int diff);
    ~LivesEvent() override = default;

    int getDiff();

private:
    int diff;
};


/**
 *
 */
class ILivesListener : public IEventListener {
public:
    ~ILivesListener() override = default;

    virtual void handleLivesEvent(std::shared_ptr<LivesEvent> ev) = 0;

};


/**
 *
 */
class PointsEvent : public GameEvent {
public:
    PointsEvent(std::shared_ptr<IEventSource> source, int diff);
    ~PointsEvent() override = default;

    int getDiff();

private:
    int diff;
};


/**
 *
 */
class IPointsListener : public IEventListener {
public:
    ~IPointsListener() override = default;

    virtual void handlePointsEvent(std::shared_ptr<PointsEvent> ev) = 0;

};

/**
 *
 */
class DynamicFramework : public IGameListener, public ILevelDoneListener, public ILivesListener, public IPointsListener, public IEventSource {
public:
    std::string toString() override;
    void handleGameEvent(std::shared_ptr<GameEvent> ev) override;
    void handleLevelDoneEvent(std::shared_ptr<LevelDoneEvent> ev) override;
    void handleLivesEvent(std::shared_ptr<LivesEvent> ev) override;
    void handlePointsEvent(std::shared_ptr<PointsEvent> ev) override;

};

/**
 *
 */
class MoveEvent : public Event {
public:
    static const auto BEGIN = 0;
    static const auto MOVE = 1;
    static const auto UPDATE = 2;
    static const auto END = 3;

    explicit MoveEvent(std::shared_ptr<IEventSource> source, int type, int moveDivider);
    ~MoveEvent() override = default;

    int getType();
    int getMoveDevider();

private:
    int type;
    int moveDevider;
};


/**
 *
 */
class IMoveListener : public IEventListener {
public:
    ~IMoveListener() override = default;

    virtual void handleMoveEvent(std::shared_ptr<MoveEvent> ev) = 0;
};


/**
 *
 */
class Level : public IGameListener, public ILevelDoneListener, public ILivesListener, public IPointsListener {
public:
    void handleGameEvent(std::shared_ptr<GameEvent> ev) override;
    void handleLevelDoneEvent(std::shared_ptr<LevelDoneEvent> ev) override;
    void handleLivesEvent(std::shared_ptr<LivesEvent> ev) override;
    void handlePointsEvent(std::shared_ptr<PointsEvent> ev) override;
};

/**
 *
 */
class Gamelet : public ICollider, IGameListener {
public:
    ~Gamelet() override = default;

    virtual std::string getName() = 0;

private:
};

/**
 *
 */
class TestGamelet : public Gamelet {
public:
    TestGamelet() = default;

    std::string getName() override;
    void handleGameEvent(std::shared_ptr<GameEvent> ev) override;
    void prepareCollision(std::shared_ptr<CollisionEvent> ev) override;
    void performCollision() override;
};

/**
 *  abstract
 */
class GameletIterator {
public:
    GameletIterator(const Level& level);
    virtual ~GameletIterator() = default;

    virtual std::shared_ptr<Gamelet> nextGamelet() = 0;

protected:
   Level level;
};


/**
 *
 */
class SimpleGameletIterator : public GameletIterator {
public:
    SimpleGameletIterator(const Level& level, std::vector<std::shared_ptr<Gamelet>> gamelets /* copy required! */);

    std::shared_ptr<Gamelet> nextGamelet() override;

private:
    std::vector<std::shared_ptr<Gamelet>> gamelets;
    size_t index = 0;
};


#endif //METROIDVANIA_DYNAMICS_H
