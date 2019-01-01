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
#include "geometry.h"


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
class TickEvent : public Event {
public:
    TickEvent(std::shared_ptr<IEventSource> source);
    ~TickEvent() override = default;

};


/**
 *
 */
class ITickListener : public IEventListener {
public:
    ~ITickListener() override = default;

    virtual void tickOccured(std::shared_ptr<TickEvent> ev) = 0;

};


/**
 * forward declaration
 */
class Gamelet;


/**
 *
 */
class Level : public std::enable_shared_from_this<Level>, public IGameListener, public ILevelDoneListener, public ILivesListener, public IPointsListener {
public:
    void handleGameEvent(std::shared_ptr<GameEvent> ev) override;
    void handleLevelDoneEvent(std::shared_ptr<LevelDoneEvent> ev) override;
    void handleLivesEvent(std::shared_ptr<LivesEvent> ev) override;
    void handlePointsEvent(std::shared_ptr<PointsEvent> ev) override;

    // manage game listeners
    void removeGameListener(std::shared_ptr<IGameListener> l);
    void addGameListener(std::shared_ptr<IGameListener> l);
    const std::vector<std::shared_ptr<IGameListener>>& getGameListeners() const;
    // manage level done listeners
    void removeLevelDoneListener(std::shared_ptr<ILevelDoneListener> l);
    void addLevelDoneListener(std::shared_ptr<ILevelDoneListener> l);
    const std::vector<std::shared_ptr<ILevelDoneListener>>& getLevelDoneListener() const;
    // manage lives listeners
    void removeLivesListener(std::shared_ptr<ILivesListener> l);
    void addLivesListener(std::shared_ptr<ILivesListener> l);
    const std::vector<std::shared_ptr<ILivesListener>>& getLivesListener() const;
    // manage points listeners
    void removePointsListener(std::shared_ptr<IPointsListener> l);
    void addPointsListener(std::shared_ptr<IPointsListener> l);
    const std::vector<std::shared_ptr<IPointsListener>>& getPointsListener() const;

    void setSize(const Dimension& size);
    const Dimension& getSize() const;
    void addGamelet(std::shared_ptr<Gamelet> gl);
    void addTickListener(std::shared_ptr<ITickListener> l);

private:
    std::vector<std::shared_ptr<IGameListener>> gameListeners;
    std::vector<std::shared_ptr<ILevelDoneListener>> levelDoneListeners;
    std::vector<std::shared_ptr<ILivesListener>> livesListeners;
    std::vector<std::shared_ptr<IPointsListener>> pointsListeners;
    std::vector<std::shared_ptr<ITickListener>> tickListeners;

    Dimension size;
    std::vector<std::shared_ptr<Gamelet>> gamelets;
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
 * TODO!!!
 */
class DynamicFramework : public std::enable_shared_from_this<DynamicFramework>, public IGameListener, public ILevelDoneListener, public ILivesListener, public IPointsListener, public IEventSource {
public:
    static const auto NUMBER_OF_MOVE_STEPS = 10;

    std::string toString() override;
    void handleGameEvent(std::shared_ptr<GameEvent> ev) override;
    void handleLevelDoneEvent(std::shared_ptr<LevelDoneEvent> ev) override;
    void handleLivesEvent(std::shared_ptr<LivesEvent> ev) override;
    void handlePointsEvent(std::shared_ptr<PointsEvent> ev) override;

    void setLevel(std::shared_ptr<Level> level);

private:
    std::shared_ptr<Level> level = nullptr;
    bool levelDone = false;

//    MoveEvent meBegin = MoveEvent(shared_from_this(), MoveEvent::BEGIN, NUMBER_OF_MOVE_STEPS);
//    MoveEvent meMove = MoveEvent(shared_from_this(), MoveEvent::MOVE, NUMBER_OF_MOVE_STEPS);
//    MoveEvent meUpdate = MoveEvent(shared_from_this(), MoveEvent::UPDATE, NUMBER_OF_MOVE_STEPS);
//    MoveEvent meEnd = MoveEvent(shared_from_this(), MoveEvent::END, NUMBER_OF_MOVE_STEPS);
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
class Gamelet : public std::enable_shared_from_this<Gamelet>, public ICollider, public IGameListener, public ITickListener {
public:
    ~Gamelet() override = default;

    void prepareCollision(std::shared_ptr<CollisionEvent> ev) override;
    void performCollision() override;
    void tickOccured(std::shared_ptr<TickEvent> ev) override;
    void handleGameEvent(std::shared_ptr<GameEvent> ev) override;

    virtual std::string getName() = 0;
    void remove();
    void addToLevel(std::shared_ptr<Level> level);
    void addGameListener(std::shared_ptr<IGameListener> l);
    std::shared_ptr<Level> getLevelAddedTo();

    void setVisiblePos(const SDL_Point& visiblePos);
    void setCalculatedPos(const SDL_Point& calculatedPos);
    void setSize(const Dimension& size);

private:
    std::shared_ptr<Level> levelAddedTo = nullptr;
    std::vector<std::shared_ptr<IGameListener>> gameListeners;

    Dimension size;
    SDL_Point visiblePos;
    SDL_Point calculatedPos;
    SDL_Rect calculatedBounds;
    MilliPoint calculatedMilliPos;
    MilliRect calculatedMilliBounds;
};


/**
 *
 */
class TestGamelet : public Gamelet {
public:
    TestGamelet() = default;

    std::string getName() override;
    void prepareCollision(std::shared_ptr<CollisionEvent> ev) override;
    void performCollision() override;
};


/**
 *
 */
class Border : public Gamelet {
public:
    Border() = default;

    std::string getName() override;
    void prepareCollision(std::shared_ptr<CollisionEvent> ev) override;
    void performCollision() override;

    bool getOpen();
    void setOpen(bool open);

private:
    bool open;
    std::shared_ptr<Gamelet> collidingGamelet = nullptr;
};


/**
 *  abstract
 */
class GameletIterator {
public:
    GameletIterator(std::shared_ptr<Level> level);
    virtual ~GameletIterator() = default;

    virtual std::shared_ptr<Gamelet> nextGamelet() = 0;

protected:
    std::shared_ptr<Level> level;
};


/**
 *
 */
class SimpleGameletIterator : public GameletIterator {
public:
    SimpleGameletIterator(std::shared_ptr<Level> level, std::vector<std::shared_ptr<Gamelet>> gamelets /* copy required! */);

    std::shared_ptr<Gamelet> nextGamelet() override;

private:
    std::vector<std::shared_ptr<Gamelet>> gamelets;
    size_t index = 0;
};



#endif //METROIDVANIA_DYNAMICS_H
