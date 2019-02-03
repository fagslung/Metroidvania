#include <utility>

#include <utility>

#include <utility>

//
// Created by Martin Eberle on 24.12.2018.
//

#include "dynamics.h"
#include <iostream>
#include <utility>
#include "base.h"
#include <algorithm>


template<class S>
void removeListenerFrom(std::vector<std::shared_ptr<S>> eventSources, std::shared_ptr<IEventListener> listener) {
    static_assert(std::is_base_of<IEventSource, S>::value, "parameter 'sources' is not an event source (of class IEventSource)");

    for (const auto &curEvtSrc : eventSources) {
        if (curEvtSrc != listener) {
            curEvtSrc->removeListener(listener);
        }
    }
}


std::shared_ptr<IEventSource> Event::getSource() const {
    return source;
}

std::string Event::toString() const {
    std::string classname = typeid(this).name();

    return classname + ", source=" + source->toString();
}

Event::Event(std::shared_ptr<IEventSource> source) : source(std::move(source)) {

}

std::shared_ptr<ICollider> CollisionEvent::getCollider1() const {
    return obj1;
}

std::shared_ptr<ICollider> CollisionEvent::getCollider2() const {
    return obj2;
}

std::shared_ptr<ICollider> CollisionEvent::getOtherCollider(std::shared_ptr<ICollider> curCollider) const {
    if (curCollider == obj1) {
        return obj2;
    }

    if (curCollider == obj2) {
        return obj1;
    }

    throw GameException("wrong collider");
}

const SDL_Point &CollisionEvent::getCollisionDirection(std::shared_ptr<ICollider> curCollider) const {
    if (curCollider == obj1) {
        return vec2to1;
    }

    if (curCollider == obj2) {
        return vec1to2;
    }

    throw GameException("wrong collider");
}

CollisionEvent::CollisionEvent(std::shared_ptr<IEventSource> source, std::shared_ptr<ICollider> obj1, std::shared_ptr<ICollider> obj2, const SDL_Point &vec1to2) :
        obj1(std::move(obj1)), obj2(std::move(obj2)), vec1to2(vec1to2), Event(std::move(source)), vec2to1{-vec1to2.x, -vec1to2.y} {}

GameEvent::GameEvent(std::shared_ptr<IEventSource> source) : Event(std::move(source)) {}

LevelDoneEvent::LevelDoneEvent(std::shared_ptr<IEventSource> source) : Event(std::move(source)) {}

LivesEvent::LivesEvent(std::shared_ptr<IEventSource> source, int diff) : Event(std::move(source)), diff(diff) {}

int LivesEvent::getDiff() const {
    return diff;
}

PointsEvent::PointsEvent(std::shared_ptr<IEventSource> source, int diff) : GameEvent(std::move(source)), diff(diff) {}

int PointsEvent::getDiff() const {
    return diff;
}

std::string DynamicFramework::toString() const {
    std::string classname = typeid(this).name();
    return  classname;
}

void DynamicFramework::handleGameEvent(const GameEvent& ev) {
    std::cout << "GameEvent: " << ev.toString();
}

void DynamicFramework::handleLevelDoneEvent(const LevelDoneEvent& ev) {
    std::cout << "LevelDoneEvent";
}

void DynamicFramework::handleLivesEvent(const LivesEvent& ev) {
    std::cout << "LivesEvent(diff = " << ev.getDiff() << ")";
}

void DynamicFramework::handlePointsEvent(const PointsEvent& ev) {
    std::cout << "PointsEvent(diff = " << ev.getDiff() << ")";
}

void DynamicFramework::setLevel(std::shared_ptr<Level> level) {
    if (this->level != nullptr) { // clean up
        this->level->removeGameListener(shared_from_this());
        this->level->removeLevelDoneListener(shared_from_this());
        this->level->removeLivesListener(shared_from_this());
        this->level->removePointsListener(shared_from_this());
    }

    this->level = level;
    this->level->addGameListener(shared_from_this());
    this->level->addLevelDoneListener(shared_from_this());
    this->level->addLivesListener(shared_from_this());
    this->level->addPointsListener(shared_from_this());

    Dimension levelSize = level->getSize();

    // oben
    auto border = std::make_shared<Border>();
    border->setVisiblePos(SDL_Point{0, -100});
    border->setSize(Dimension{levelSize.width, 100});
    border->setOpen(false);
    level->addGamelet(border);
    // links
    border = std::make_shared<Border>();
    border->setVisiblePos(SDL_Point{-100, 0});
    border->setSize(Dimension{100, levelSize.height});
    border->setOpen(false);
    level->addGamelet(border);
    // rechts
    border = std::make_shared<Border>();
    border->setVisiblePos(SDL_Point{levelSize.width, 0});
    border->setSize(Dimension{100, levelSize.height});
    border->setOpen(false);
    level->addGamelet(border);
    // unten
    border = std::make_shared<Border>();
    border->setVisiblePos(SDL_Point{0, levelSize.height});
    border->setSize(Dimension{levelSize.width, 100});
    border->setOpen(true);
    level->addGamelet(border);
}

void DynamicFramework::tick() {
    if (level != nullptr) {
        const auto meBegin = MoveEvent(shared_from_this(), MoveEvent::BEGIN, NUMBER_OF_MOVE_STEPS);
        const auto meMove = MoveEvent(shared_from_this(), MoveEvent::MOVE, NUMBER_OF_MOVE_STEPS);
        const auto meUpdate = MoveEvent(shared_from_this(), MoveEvent::UPDATE, NUMBER_OF_MOVE_STEPS);
        const auto meEnd = MoveEvent(shared_from_this(), MoveEvent::END, NUMBER_OF_MOVE_STEPS);

        level->fireMoveEvent(meBegin);
        for (int i = 0; i < NUMBER_OF_MOVE_STEPS; i++) {
            level->fireMoveEvent(meMove);
            level->fireMoveEvent(meUpdate);
        }
        level->fireMoveEvent(meEnd);

        level->fireTickEvent();
        if (!levelDone) {
            // TODO ...
            if (level->isDone()) {
                levelDone = true;
                level->fireLevelDoneEvent(LevelDoneEvent(shared_from_this()));
            }
        }
    }
}

void DynamicFramework::removeListener(std::shared_ptr<IEventListener> listener) {

}

void DynamicFramework::fireCollisionEvent(const CollisionEvent &ce) {
    if (level != nullptr) {
        level->fireCollisionEvent(ce);
    }
}

std::shared_ptr<CollisionEvent> DynamicFramework::wantMove(std::shared_ptr<Gamelet> gl, MilliPoint &move) {
    SDL_Point coll1to2{0, 0};
    SDL_Point b1to2{0, 0};
    auto it = level->newNearGameletIterator(gl);

    MilliRect bnd = gl->getCalculatedMilliBounds();
    MilliRect mBnd = bnd;
    mBnd.increaseDimension(move);

    std::shared_ptr<Gamelet> collGL;
    auto gl2 = it->nextGamelet();
    while (gl2 != nullptr) {
        MilliRect bnd2 = gl2->getCalculatedMilliBounds();
        if (gl != gl2 && mBnd.isIntersectingWith(bnd2)) {

            if (Intersection::intersect(gl, gl2)) {
                Intersection::getIntersection(gl, gl2)->check();
            }

            bool sideX = (bnd.x + bnd.w / 2 < bnd2.x + bnd2.w / 2);
            int d0x = sideX ? bnd2.x - (bnd.x + bnd.w) : bnd.x - (bnd2.x + bnd2.w);
            int mx = sideX ? move.x : -move.x;
            bool sideY = (bnd.y + bnd.h / 2 < bnd2.y + bnd2.h / 2);
            int d0y = sideY ? bnd2.y - (bnd.y + bnd.h) : bnd.y - (bnd2.y + bnd2.h);
            int my = sideY ? move.y : -move.y;
            long d0m = (long)d0y * (long)mx + (long)d0x * (long)my;
            b1to2.x = b1to2.y = 0;

            if (d0x < 0 && d0y < 0) { // gl und gl2 ueberschneiden sich bereits
                if (d0x < d0y) { // x-Ueberschneidung ist tiefer
                    if (my > 0) {
                        b1to2.y = sideY ? 1 : -1; mx = my = 0; collGL = gl2;
                    }
                } else { // y-Ueberschneidung ist tiefer
                    if (mx > 0) {
                        b1to2.x = sideX ? 1 : -1; mx = my = 0; collGL = gl2;
                    }
                }
                // mx == 0 && my == 0  =>  keine Ueberschneidungsberechnungen mehr
            }
            if (mx > 0 && d0x >= 0 && d0y >= 0) { // gl bewegt sich auf gl2 in x-Richtung zu
                if (d0x >= 0) {
                    // Kollisionsberechnung der zugewandten senkrechten Seiten durchfuehren
                    if (d0m <= 0 && d0m >= -(long)(bnd.h + bnd2.h) * (long)mx) {
                        // => Quer-Ueberschneidung bei dx == 0
                        my = (int)(((long)my * d0x) / mx);
                        mx = d0x;
                        b1to2.x = sideX ? 1 : -1;
                        collGL = gl2;
                    }
                }
            }
            if (my > 0 && d0x >= 0 && d0y >= 0) { // gl bewegt sich auf gl2 zu
                if (d0y >= 0) {
                    // Kollisionsberechnung der zugewandten senkrechten Seiten durchfuehren
                    if (d0m <= 0 && d0m >= -(long)(bnd.w + bnd2.w) * (long)my) {
                        // => Quer-Ueberschneidung bei dx == 0
                        mx = (int)(((long)mx * d0y) / my);
                        my = d0y;
                        b1to2.y = sideY ? 1 : -1;
                        collGL = gl2;
                    }
                }
            }
            if (b1to2.x != 0 && b1to2.y != 0) {
                if (d0x + mx < d0y + my) b1to2.x = 0;
                else b1to2.y = 0;
            }
            if (collGL == gl2) { // Kollision mit aktuellem Gamelet passiert
                coll1to2.x = b1to2.x;
                coll1to2.y = b1to2.y;
                move.x = sideX ? mx : -mx;
                move.y = sideY ? my : -my;
            }
        }

        gl2 = it->nextGamelet();
    }

    if (collGL != nullptr) {

        return std::make_shared<CollisionEvent>(shared_from_this(), gl, collGL, coll1to2);
    }

    return nullptr; // Provisorium
}

MoveEvent::MoveEvent(std::shared_ptr<IEventSource> source, int type, int moveDivider) : Event(std::move(source)), type(type), moveDivider(moveDivider) {

}

int MoveEvent::getMoveDivider() const {
    return moveDivider;
}

int MoveEvent::getType() const {
    return type;
}

GameletIterator::GameletIterator(std::shared_ptr<Level> level) : level(std::move(level)) {

}

void Level::handleGameEvent(const GameEvent& ev) {

}

void Level::handleLevelDoneEvent(const LevelDoneEvent& ev) {

}

void Level::handleLivesEvent(const LivesEvent& ev) {

}

void Level::handlePointsEvent(const PointsEvent& ev) {

}

void Level::removeGameListener(std::shared_ptr<IGameListener> l) {
    gameListeners.erase(std::remove(gameListeners.begin(), gameListeners.end(), l), gameListeners.end());
}

void Level::addGameListener(std::shared_ptr<IGameListener> l) {
    gameListeners.push_back(l);
}

const std::vector<std::shared_ptr<IGameListener>> &Level::getGameListeners() const {
    return gameListeners;
}

void Level::removeLevelDoneListener(std::shared_ptr<ILevelDoneListener> l) {
    levelDoneListeners.erase(std::remove(levelDoneListeners.begin(), levelDoneListeners.end(), l), levelDoneListeners.end());
}

void Level::removeLivesListener(std::shared_ptr<ILivesListener> l) {
    livesListeners.erase(std::remove(livesListeners.begin(), livesListeners.end(), l), livesListeners.end());
}

void Level::removePointsListener(std::shared_ptr<IPointsListener> l) {
    pointsListeners.erase(std::remove(pointsListeners.begin(), pointsListeners.end(), l), pointsListeners.end());
}

void Level::addLevelDoneListener(std::shared_ptr<ILevelDoneListener> l) {
    levelDoneListeners.push_back(l);
}

void Level::addLivesListener(std::shared_ptr<ILivesListener> l) {
    livesListeners.push_back(l);
}

void Level::addPointsListener(std::shared_ptr<IPointsListener> l) {
    pointsListeners.push_back(l);
}

const std::vector<std::shared_ptr<ILevelDoneListener>> &Level::getLevelDoneListener() const {
    return levelDoneListeners;
}

const std::vector<std::shared_ptr<ILivesListener>> &Level::getLivesListener() const {
    return livesListeners;
}

const std::vector<std::shared_ptr<IPointsListener>> &Level::getPointsListener() const {
    return pointsListeners;
}

void Level::setSize(const Dimension &size) {
    this->size = size;
}

const Dimension &Level::getSize() const {
    return size;
}

void Level::addGamelet(std::shared_ptr<Gamelet> gl) {
    if (gl != nullptr) {
        gl->addToLevel(shared_from_this());
        gl->addGameListener(shared_from_this());
        gamelets.push_back(gl);
    }
}

void Level::addTickListener(std::shared_ptr<ITickListener> l) {
    tickListeners.push_back(l);
}

void Level::fireMoveEvent(const MoveEvent &me) {
    for (const auto &ml : moveListeners) {
       ml->handleMoveEvent(me);
    }

    if (me.getType() == MoveEvent::END) {
        for (const auto &is : intersections) {
            is->check();
        }
    }
}

SimpleGameletIterator::SimpleGameletIterator(std::shared_ptr<Level> level, std::vector<std::shared_ptr<Gamelet>> gamelets) :
        GameletIterator(std::move(level)), gamelets(std::move(gamelets)) {

}

std::shared_ptr<Gamelet> SimpleGameletIterator::nextGamelet() {
    if (index < gamelets.size()) {

        return gamelets[index++];
    }

    return nullptr;
}

std::string TestGamelet::getName() const {
    return "TestGamelet";
}

void TestGamelet::prepareCollision(const CollisionEvent& ev) {

}

void TestGamelet::performCollision() {

}

void TestGamelet::removeListener(std::shared_ptr<IEventListener> listener) {
    Gamelet::removeListener(listener);
}

std::string TestGamelet::toString() const {
    return getName();
}

std::string Border::getName() const {
    return "Border";
}

bool Border::getOpen() {
    return open;
}

void Border::setOpen(bool open) {
    this->open = open;
}

void Border::prepareCollision(const CollisionEvent& ev) {
    Gamelet::prepareCollision(ev);
    if (open) {
        if (auto tmp = std::dynamic_pointer_cast<Gamelet>(ev.getOtherCollider(shared_from_this()))) {
            collidingGamelet = tmp;
        }
    }
}

void Border::performCollision() {
    Gamelet::performCollision();
    if (collidingGamelet != nullptr) {
        collidingGamelet->remove();
        collidingGamelet = nullptr;
    }
}

void Border::removeListener(std::shared_ptr<IEventListener> listener) {
    Gamelet::removeListener(listener);

    // TODO other listeners
}

std::string Border::toString() const {
    return getName();
}


void Gamelet::remove() {
    // TODO
//    setAnimIndex(-1);
//    if (levelAddedTo != null) levelAddedTo.gameletToRemove(this);
}

void Gamelet::performCollision() {

}

void Gamelet::prepareCollision(const CollisionEvent& ev) {

}

void Gamelet::addToLevel(std::shared_ptr<Level> level) {
    if (level != nullptr) {
        level->addTickListener(shared_from_this());
        levelAddedTo = level;
    }
}

void Gamelet::tickOccured(const TickEvent& ev) {
    if (intersections->isIntersected()) {
        drawCnt = 2;
    }
}

void Gamelet::addGameListener(std::shared_ptr<IGameListener> l) {
    gameListeners.push_back(l);
}

std::shared_ptr<Level> Gamelet::getLevelAddedTo() {
    return levelAddedTo;
}

void Gamelet::handleGameEvent(const GameEvent& ev) {

}

void Gamelet::setVisiblePos(const SDL_Point &visiblePos) {
    this->visiblePos = visiblePos;
    setCalculatedPos(visiblePos);
}

void Gamelet::setCalculatedPos(const SDL_Point &calculatedPos) {
    this->calculatedPos = calculatedPos;
    this->calculatedBounds.x = calculatedPos.x;
    this->calculatedBounds.y = calculatedPos.y;
    this->calculatedMilliPos = calculatedPos;
    this->calculatedMilliBounds.setPosition(calculatedMilliPos);
}

void Gamelet::setSize(const Dimension &size) {
    this->size = size;
    this->calculatedBounds.h = size.height;
    this->calculatedBounds.w = size.width;
    this->calculatedMilliBounds = size;
}

bool Gamelet::getDirty() {
    auto posDiffers = calculatedPos.x != visiblePos.x || calculatedPos.y != visiblePos.y;

    return  posDiffers || dirty;
}

void Gamelet::removeGameListener(std::shared_ptr<IGameListener> l) {
    gameListeners.erase(std::remove(gameListeners.begin(), gameListeners.end(), l), gameListeners.end());
}

void Gamelet::removeFromLevel(std::shared_ptr<Level> level) {
    if (level != nullptr) {
        levelAddedTo = nullptr;
        level->removeTickListener(shared_from_this());
    }
}

void Gamelet::removeListener(std::shared_ptr<IEventListener> listener) {
    if (auto tmp = std::dynamic_pointer_cast<IGameListener>(listener)) {
        removeGameListener(tmp);
    }
    // TODO other listeners
}

bool Gamelet::isForLevelDone() {
    return false;
}

const MilliRect &Gamelet::getCalculatedMilliBounds() const {
    return calculatedMilliBounds;
}

std::shared_ptr<GameletIntersections> Gamelet::getIntersections() {
    if (intersections == nullptr) {
        intersections = std::make_shared<GameletIntersections>(shared_from_this());
    }

    return intersections;
}

void Gamelet::prepareIntersectionChange(std::shared_ptr<Gamelet> other, bool newIs) {

}

void Gamelet::performIntersectionChange() {

}

const MilliPoint &Gamelet::getCalculatedMilliPos() const {
    return calculatedMilliPos;
}

void Gamelet::setCalculatedMilliPos(const MilliPoint &calculatedMilliPos) {
    this->calculatedMilliPos = calculatedMilliPos;
    calculatedMilliBounds.setPosition(calculatedMilliPos);
    this->calculatedPos = this->calculatedMilliPos;
    calculatedBounds.x = calculatedPos.x;
    calculatedBounds.y = calculatedPos.y;
}

Gamelet::Gamelet() {
//    setCalculatedPos(visiblePos);
//    String[] paths = getImagePaths();
//    if (paths == null || paths.length == 0) {
//        sizedImageAnimations = new SizedImage[1];
//        sizedImageAnimations[0] =
//                new SizedImage(getStandardImagePath(), getStandardImagePath());
//    } else {
//        sizedImageAnimations = new SizedImage[paths.length];
//        for (int i=0; i<paths.length; i++)
//            sizedImageAnimations[i] = new SizedImage(paths[i], getStandardImagePath());
//    }
//    setDirty(true);
//    drawCnt = 0;
//
//    setAnimIndex(0);
//    setSize(getImageSize());
}

void Gamelet::setDirty(bool dirty) {
    this->dirty = dirty;
}

void Gamelet::setAnimIndex(int animIndex) {
//    this->animIndex = animIndex;
//    if (animIndex >= 0 && animIndex < sprites.size()) {
//        setSizedImage(sprites[animIndex]);
//    } else {
//        setSizedImage(null);
//    }
}

TickEvent::TickEvent(std::shared_ptr<IEventSource> source) : Event(std::move(source)) {

}

void Level::fireTickEvent() {
    auto te = TickEvent(shared_from_this());
    for (const auto &tl : tickListeners) {
        tl->tickOccured(te);
    }

    for (const auto &gtr : gameletsToRemove) {
        removeGamelet(gtr);
    }
}

void Level::removeGamelet(std::shared_ptr<Gamelet> gl) {
    if (gl != nullptr) {
        // remove element gl (erase-remove idiom)
        gamelets.erase(std::remove(gamelets.begin(), gamelets.end(), gl), gamelets.end());

        auto rmIt = std::remove(gameletsToRemove.begin(), gameletsToRemove.end(), gl);
        bool removed = rmIt != gameletsToRemove.end();
        gameletsToRemove.erase(rmIt, gameletsToRemove.end());
        if (removed && gl->getDirty()) {
            extraDGamelets.push_back(gl);
        }
        gl->removeGameListener(shared_from_this());
        gl->removeFromLevel(shared_from_this());

        // -- disconnect from possible other event sources
        removeListenerFrom<Gamelet>(gamelets, gl);
        removeListenerFrom<Gamelet>(invisibleGamelets, gl);
        this->removeListener(gl);
    }
}


void Level::removeTickListener(std::shared_ptr<ITickListener> l) {
    tickListeners.erase(std::remove(tickListeners.begin(), tickListeners.end(), l), tickListeners.end());
}

void Level::removeListener(std::shared_ptr<IEventListener> listener) {
    if (auto tmp = std::dynamic_pointer_cast<IGameListener>(listener)) {
        removeGameListener(tmp);
    }

    if (auto tmp = std::dynamic_pointer_cast<ILevelDoneListener>(listener)) {
        removeLevelDoneListener(tmp);
    }

    if (auto tmp = std::dynamic_pointer_cast<ILivesListener>(listener)) {
        removeLivesListener(tmp);
    }

    if (auto tmp = std::dynamic_pointer_cast<IPointsListener>(listener)) {
        removePointsListener(tmp);
    }

    if (auto tmp = std::dynamic_pointer_cast<ITickListener>(listener)) {
        removeTickListener(tmp);
    }

    if (auto tmp = std::dynamic_pointer_cast<IMoveListener>(listener)) {
        removeMoveListener(tmp);
    }

    // TODO other listeners
}

void Level::addMoveListener(std::shared_ptr<IMoveListener> l) {
    moveListeners.push_back(l);
}

void Level::removeMoveListener(std::shared_ptr<IMoveListener> l) {
    moveListeners.erase(std::remove(moveListeners.begin(), moveListeners.end(), l), moveListeners.end());
}

std::string Level::toString() const {
    return "Level";
}

bool Level::isDone() {
    for (const auto &curGamelet : gamelets) {
        if (curGamelet->isForLevelDone()) {

            return false;
        }
    }

    return true;
}

void Level::fireLevelDoneEvent(const LevelDoneEvent &e) {
    for (const auto &curListener : levelDoneListeners) {
        curListener->handleLevelDoneEvent(e);
    }
}

void Level::fireCollisionEvent(const CollisionEvent &ce) {
    ce.getCollider1()->prepareCollision(ce);
    ce.getCollider2()->prepareCollision(ce);
    ce.getCollider1()->performCollision();
    ce.getCollider2()->performCollision();

    for (const auto &cur : collisionListeners) {
        cur->collisionOccured(ce);
    }
}

std::shared_ptr<GameletIterator> Level::newNearGameletIterator(std::shared_ptr<Gamelet> gl) {
    return std::make_shared<SimpleGameletIterator>(shared_from_this(), gamelets);
}

std::vector<std::shared_ptr<Intersection>>& Level::getIntersections() {
    return intersections;
}

void Level::fireIntersectionChangeEvent(std::shared_ptr<Intersection> is) {
    auto ice = IntersectionChangeEvent(shared_from_this(), std::move(is));

    for (const auto &curListener : intersectionChangeListeners) {
        curListener->intersectionChangeOccured(ice);
    }
}

void Level::addIntersectionChangeListener(std::shared_ptr<IIntersectionChangeListener> l) {
    intersectionChangeListeners.push_back(l);
}

void Level::removeIntersectionChangeListener(std::shared_ptr<IIntersectionChangeListener> l) {
    intersectionChangeListeners.erase(std::remove(intersectionChangeListeners.begin(), intersectionChangeListeners.end(), l), intersectionChangeListeners.end());
}

bool Intersection::intersect(std::shared_ptr<Gamelet> gl1, std::shared_ptr<Gamelet> gl2) {
    return gl1 != nullptr && gl2 != nullptr && gl1 != gl2 &&
        gl1->getCalculatedMilliBounds().isIntersectingWith(gl2->getCalculatedMilliBounds());
}

std::shared_ptr<Intersection> Intersection::getIntersection(std::shared_ptr<Gamelet> gl1, std::shared_ptr<Gamelet> gl2) {
    std::shared_ptr<Intersection> ints = nullptr;

    if (gl1 != nullptr && gl2 != nullptr) {
        auto l1 = gl1->getLevelAddedTo();
        auto l2 = gl2->getLevelAddedTo();
        if (l1 == l2) {
            ints = std::make_shared<Intersection>(gl1, gl2, l1);
            auto intersections = l1->getIntersections();
            auto result = std::find(intersections.begin(), intersections.end(), ints);
            if (result == intersections.end()) {
                ints->add();
            } else {
                ints = *result;
            }
        }
    }

    return ints;
}

Intersection::Intersection(std::shared_ptr<Gamelet> gl1, std::shared_ptr<Gamelet> gl2, std::shared_ptr<Level> level) :
    gl1(std::move(gl1)), gl2(std::move(gl2)), associatedLevel(std::move(level)),
    gi1(std::move(gl1->getIntersections())), gi2(std::move(gl2->getIntersections())) {
}

void Intersection::add() {
    if (associatedLevel != nullptr) {
        associatedLevel->getIntersections().push_back(shared_from_this());

    }
}

bool Intersection::check() {
    setIntersecting(intersect());

    return getIntersecting();
}

void Intersection::setIntersecting(bool is) {
    is = is && (associatedLevel != nullptr);
    if (is != intersecting) {
        if (is) {
            gi1->intersections.push_back(shared_from_this());
            gi2->intersections.push_back(shared_from_this());
        } else {
            gi1->intersections.erase(std::remove(gi1->intersections.begin(), gi1->intersections.end(), shared_from_this()), gi1->intersections.end());
            gi2->intersections.erase(std::remove(gi2->intersections.begin(), gi2->intersections.end(), shared_from_this()), gi2->intersections.end());
        }
        intersecting = is;
        gl1->prepareIntersectionChange(gl2, is);
        gl2->prepareIntersectionChange(gl1, is);
        gl1->performIntersectionChange();
        gl2->performIntersectionChange();
        if (associatedLevel != nullptr)
            associatedLevel->fireIntersectionChangeEvent(shared_from_this());
        if (!is) {
            free();
        }
    }
}

bool Intersection::intersect() {
    return intersect(gl1, gl2);
}

bool Intersection::getIntersecting() {
    return intersecting;
}

std::shared_ptr<Gamelet> Intersection::getGamelet1() {
    return gl1;
}

std::shared_ptr<Gamelet> Intersection::getGamelet2() {
    return gl2;
}

std::shared_ptr<Gamelet> Intersection::getOtherGamelet(std::shared_ptr<Gamelet> gl) {
    if (gl == gl1) {
        return gl2;
    }
    if (gl == gl2) {
        return gl1;
    }

    return nullptr;
}

void Intersection::free() {
    if (getIntersecting()) {
        setIntersecting(true);
    }
    if (associatedLevel != nullptr) {
        associatedLevel->getIntersections().erase(std::remove(associatedLevel->getIntersections().begin(), associatedLevel->getIntersections().end(), shared_from_this()), associatedLevel->getIntersections().end());
        associatedLevel = nullptr;
    }
}

GameletIntersections::GameletIntersections(std::shared_ptr<Gamelet> gl) : owner(std::move(gl)) {

}

bool GameletIntersections::isIntersected() {
    return !intersections.empty();
}

IntersectionChangeEvent::IntersectionChangeEvent(std::shared_ptr<IEventSource> source, std::shared_ptr<Intersection> is) : Event(std::move(source)), is(std::move(is)) {

}
