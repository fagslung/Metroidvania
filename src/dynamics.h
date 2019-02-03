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
class IEventListener {
public:
    virtual ~IEventListener() = default;
};


template <class S>
void removeListenerFrom(std::vector<std::shared_ptr<S>> eventSources, std::shared_ptr<IEventListener> listener);


/**
 * Interface
 */
class IEventSource {
public:
    virtual ~IEventSource() = default;

    virtual std::string toString() const = 0;
    virtual void removeListener(std::shared_ptr<IEventListener> listener) = 0;
};


/**
 *
 */
class Event {
public:
    explicit Event(std::shared_ptr<IEventSource> source);
    virtual ~Event() = default;

    std::shared_ptr<IEventSource> getSource() const;
    virtual std::string toString() const;

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

    std::shared_ptr<ICollider> getCollider1() const;
    std::shared_ptr<ICollider> getCollider2() const;
    std::shared_ptr<ICollider> getOtherCollider(std::shared_ptr<ICollider> curCollider) const;
    const SDL_Point& getCollisionDirection(std::shared_ptr<ICollider> curCollider) const;

private:
    std::shared_ptr<ICollider> obj1;
    std::shared_ptr<ICollider> obj2;
    SDL_Point vec1to2;
    SDL_Point vec2to1;
};


/**
 *
 */
class ICollider : public virtual IEventListener {
public:
    ~ICollider() override = default;

    virtual void prepareCollision(const CollisionEvent& ev) = 0;
    virtual void performCollision() = 0;
};


/**
 *
 */
class ICollisionListener : public virtual IEventListener {
public:
    ~ICollisionListener() override = default;

    virtual void collisionOccured(const CollisionEvent& ev) = 0;
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
class IGameListener : public virtual IEventListener {
public:
    ~IGameListener() override = default;

    virtual void handleGameEvent(const GameEvent& ev) = 0;

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
class ILevelDoneListener : public virtual IEventListener {
public:
    ~ILevelDoneListener() override = default;

    virtual void handleLevelDoneEvent(const LevelDoneEvent& ev) = 0;

};


/**
 *
 */
class LivesEvent : public Event {
public:
    LivesEvent(std::shared_ptr<IEventSource> source, int diff);
    ~LivesEvent() override = default;

    int getDiff() const;

private:
    int diff;
};


/**
 *
 */
class ILivesListener : public virtual IEventListener {
public:
    ~ILivesListener() override = default;

    virtual void handleLivesEvent(const LivesEvent& ev) = 0;

};


/**
 *
 */
class PointsEvent : public GameEvent {
public:
    PointsEvent(std::shared_ptr<IEventSource> source, int diff);
    ~PointsEvent() override = default;

    int getDiff() const;

private:
    int diff;
};


/**
 *
 */
class IPointsListener : public virtual IEventListener {
public:
    ~IPointsListener() override = default;

    virtual void handlePointsEvent(const PointsEvent& ev) = 0;

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
class ITickListener : public virtual IEventListener {
public:
    ~ITickListener() override = default;

    virtual void tickOccured(const TickEvent& ev) = 0;

};


/**
 * forward declaration
 */
class Gamelet;


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

    int getType() const;
    int getMoveDivider() const;

private:
    int type;
    int moveDivider;
};


/**
 *
 */
class IMoveListener : public virtual IEventListener {
public:
    ~IMoveListener() override = default;

    virtual void handleMoveEvent(const MoveEvent& ev) = 0;
};


/**
 * forward declaration
 */
class Level;


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


/**
 * forward declaration
 */
class Intersection;


/**
 *
 */
class GameletIntersections {
public:
    GameletIntersections(std::shared_ptr<Gamelet> gl);

    bool isIntersected();

    std::shared_ptr<Gamelet> owner;
    std::vector<std::shared_ptr<Intersection>> intersections;
};


/**
 *
 */
class Intersection : public std::enable_shared_from_this<Intersection> {
public:
    Intersection(std::shared_ptr<Gamelet> gl1, std::shared_ptr<Gamelet> gl2, std::shared_ptr<Level> level);

    bool check();
    void add();
    void free();
    static bool intersect(std::shared_ptr<Gamelet> gl1, std::shared_ptr<Gamelet> gl2);
    bool intersect();
    static std::shared_ptr<Intersection> getIntersection(std::shared_ptr<Gamelet> gl1, std::shared_ptr<Gamelet> gl2);
    void setIntersecting(bool is);
    bool getIntersecting();
    std::shared_ptr<Gamelet> getGamelet1();
    std::shared_ptr<Gamelet> getGamelet2();
    std::shared_ptr<Gamelet> getOtherGamelet(std::shared_ptr<Gamelet> gl);

private:
    std::shared_ptr<Gamelet> gl1;
    std::shared_ptr<Gamelet> gl2;
    std::shared_ptr<GameletIntersections> gi1;
    std::shared_ptr<GameletIntersections> gi2;
    std::shared_ptr<Level> associatedLevel;
    bool intersecting;
};


class IntersectionChangeEvent : public Event {
public:
    explicit IntersectionChangeEvent(std::shared_ptr<IEventSource> source, std::shared_ptr<Intersection> is);
    ~IntersectionChangeEvent() override = default;

private:
    std::shared_ptr<Intersection> is;
};


/**
 *
 */
class IIntersectionChangeListener : public virtual IEventListener {
public:
    ~IIntersectionChangeListener() override = default;

    virtual void intersectionChangeOccured(const IntersectionChangeEvent& ev) = 0;

};


/**
 *
 */
class Level : public std::enable_shared_from_this<Level>, public IGameListener, public ILevelDoneListener, public ILivesListener, public IPointsListener, public IEventSource {
public:
    std::string toString() const override;
    void handleGameEvent(const GameEvent& ev) override;
    void handleLevelDoneEvent(const LevelDoneEvent& ev) override;
    void handleLivesEvent(const LivesEvent& ev) override;
    void handlePointsEvent(const PointsEvent& ev) override;

    // manage game listeners
    void removeListener(std::shared_ptr<IEventListener> listener) override;

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
    // manage tick listeners
    void addTickListener(std::shared_ptr<ITickListener> l);
    void removeTickListener(std::shared_ptr<ITickListener> l);
    // manage move listeners
    void addMoveListener(std::shared_ptr<IMoveListener> l);
    void removeMoveListener(std::shared_ptr<IMoveListener> l);
    // manage intersection change listeners
    void addIntersectionChangeListener(std::shared_ptr<IIntersectionChangeListener> l);
    void removeIntersectionChangeListener(std::shared_ptr<IIntersectionChangeListener> l);

    void setSize(const Dimension& size);
    const Dimension& getSize() const;
    void addGamelet(std::shared_ptr<Gamelet> gl);

    void fireMoveEvent(const MoveEvent& me);
    void fireTickEvent();
    void removeGamelet(std::shared_ptr<Gamelet> gl);

    bool isDone(); // level successfully finished?
    void fireLevelDoneEvent(const LevelDoneEvent& e);

    void fireCollisionEvent(const CollisionEvent& ce);

    std::shared_ptr<GameletIterator> newNearGameletIterator(std::shared_ptr<Gamelet> gl);

    std::vector<std::shared_ptr<Intersection>>& getIntersections();
    void fireIntersectionChangeEvent(std::shared_ptr<Intersection> is);

private:
    // listener
    std::vector<std::shared_ptr<IGameListener>> gameListeners;
    std::vector<std::shared_ptr<ILevelDoneListener>> levelDoneListeners;
    std::vector<std::shared_ptr<ILivesListener>> livesListeners;
    std::vector<std::shared_ptr<IPointsListener>> pointsListeners;
    std::vector<std::shared_ptr<ITickListener>> tickListeners;
    std::vector<std::shared_ptr<IMoveListener>> moveListeners;
    std::vector<std::shared_ptr<ICollisionListener>> collisionListeners;
    std::vector<std::shared_ptr<IIntersectionChangeListener>> intersectionChangeListeners;

    Dimension size;

    std::vector<std::shared_ptr<Gamelet>> gamelets;
    std::vector<std::shared_ptr<Gamelet>> gameletsToRemove;
    std::vector<std::shared_ptr<Gamelet>> extraDGamelets;
    std::vector<std::shared_ptr<Gamelet>> invisibleGamelets;
    std::vector<std::shared_ptr<Intersection>> intersections;
};


/**
 *
 */
class DynamicFramework : public std::enable_shared_from_this<DynamicFramework>, public IGameListener, public ILevelDoneListener, public ILivesListener, public IPointsListener, public IEventSource {
public:
    static const auto NUMBER_OF_MOVE_STEPS = 10;

    std::string toString() const override;
    void removeListener(std::shared_ptr<IEventListener> listener) override;
    void handleGameEvent(const GameEvent& ev) override;
    void handleLevelDoneEvent(const LevelDoneEvent& ev) override;
    void handleLivesEvent(const LivesEvent& ev) override;
    void handlePointsEvent(const PointsEvent& ev) override;

    void setLevel(std::shared_ptr<Level> level);
    void tick(); // principal function: has to be called once per tick
    void fireCollisionEvent(const CollisionEvent& ce);
    /*
     * => null:  keine Kollision
     * => sonst: Kollision, die nach dem Ausfuehren des moves auftreten wird und move wird entsprechend verkuerzt
     */
    std::shared_ptr<CollisionEvent> wantMove(std::shared_ptr<Gamelet> gl, MilliPoint& move /* out parameter! */);

private:
    std::shared_ptr<Level> level = nullptr;
    bool levelDone = false;
};


/**
 *
 */
class Gamelet : public std::enable_shared_from_this<Gamelet>, public ICollider, public IGameListener, public ITickListener, public IEventSource {
public:
    static const auto ANIM_LOOPUP = 0;
    static const auto ANIM_LOOPDOWN = 1;
    static const auto ANIM_PINGPONG = 2;
    static const auto ANIM_PONGPING = 3;
    static const auto ANIM_FLAG_STARTRANDOMLY = 0x4000;
    static const auto ANIM_FLAG_COUNTUP = 0x2000;

    Gamelet();
    ~Gamelet() override = default;

    void removeListener(std::shared_ptr<IEventListener> listener) override;
    void prepareCollision(const CollisionEvent& ev) override;
    void performCollision() override;
    void tickOccured(const TickEvent& ev) override;
    void handleGameEvent(const GameEvent& ev) override;

    virtual std::string getName() const = 0;
    void remove();
    virtual void addToLevel(std::shared_ptr<Level> level);
    void addGameListener(std::shared_ptr<IGameListener> l);
    void removeGameListener(std::shared_ptr<IGameListener> l);
    std::shared_ptr<Level> getLevelAddedTo();
    virtual void removeFromLevel(std::shared_ptr<Level> level);

    void setVisiblePos(const SDL_Point& visiblePos);
    void setCalculatedPos(const SDL_Point& calculatedPos);
    void setSize(const Dimension& size);
    bool getDirty();
    void setDirty(bool dirty);
    virtual bool isForLevelDone(); // gamelet to be destroyed for succeeding level?
    const MilliRect& getCalculatedMilliBounds() const;
    const MilliPoint& getCalculatedMilliPos() const;
    void setCalculatedMilliPos(const MilliPoint& calculatedMilliPos);
    std::shared_ptr<GameletIntersections> getIntersections();
    void prepareIntersectionChange(std::shared_ptr<Gamelet> other, bool newIs);
    void performIntersectionChange();
    void setAnimIndex(int animIndex);

private:
    // listener
    std::vector<std::shared_ptr<IGameListener>> gameListeners;

    std::shared_ptr<Level> levelAddedTo = nullptr;
    std::shared_ptr<GameletIntersections> intersections = nullptr;

    SizedImage sizedImage;
    std::vector<SDL_Rect> sprites;
    int animIndex;
    int animBaseIndex;
    int animLen;
    int animCount;
    int animOffset;
    short animType;

    Dimension size;
    Dimension visibleSize;
    SDL_Point visiblePos;
    SDL_Point calculatedPos;
    MilliPoint calculatedMilliPos;
    SDL_Rect calculatedBounds;
    MilliRect calculatedMilliBounds;

    SDL_Point calculatedDPos;
    SDL_Texture* dImage;
    Dimension dSize;
    bool dDirty;

    bool dirty;
    int drawCnt;
};


/**
 *
 */
class TestGamelet : public Gamelet {
public:
    TestGamelet() = default;

    std::string getName() const override;
    std::string toString() const override;
    void removeListener(std::shared_ptr<IEventListener> listener) override;
    void prepareCollision(const CollisionEvent& ev) override;
    void performCollision() override;
};


/**
 *
 */
class Border : public Gamelet {
public:
    Border() = default;

    std::string getName() const override;
    std::string toString() const override;
    void removeListener(std::shared_ptr<IEventListener> listener) override;
    void prepareCollision(const CollisionEvent& ev) override;
    void performCollision() override;

    bool getOpen();
    void setOpen(bool open);

private:
    // listener

    bool open;
    std::shared_ptr<Gamelet> collidingGamelet = nullptr;
};


#endif //METROIDVANIA_DYNAMICS_H
