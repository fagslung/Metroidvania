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


std::shared_ptr<IEventSource> Event::getSource() {
    return source;
}

std::string Event::toString() {
    std::string classname = typeid(this).name();

    return classname + ", source=" + source->toString();
}

Event::Event(std::shared_ptr<IEventSource> source) : source(std::move(source)) {

}

std::shared_ptr<ICollider> CollisionEvent::getCollider1() {
    return obj1;
}

std::shared_ptr<ICollider> CollisionEvent::getCollider2() {
    return obj2;
}

std::shared_ptr<ICollider> CollisionEvent::getOtherCollider(std::shared_ptr<ICollider> curCollider) {
    if (curCollider == obj1) {
        return obj2;
    }

    if (curCollider == obj2) {
        return obj1;
    }

    throw GameException("wrong collider");
}

SDL_Point &CollisionEvent::getCollisionDirection(std::shared_ptr<ICollider> curCollider) {
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

int LivesEvent::getDiff() {
    return diff;
}

PointsEvent::PointsEvent(std::shared_ptr<IEventSource> source, int diff) : GameEvent(std::move(source)), diff(diff) {}

int PointsEvent::getDiff() {
    return diff;
}

std::string DynamicFramework::toString() {
    std::string classname = typeid(this).name();
    return  classname;
}

void DynamicFramework::handleGameEvent(std::shared_ptr<GameEvent> ev) {
    if (auto pev = dynamic_cast<PointsEvent*>(ev.get())) {
        std::cout << "Points event handled";
    }
}

void DynamicFramework::handleLevelDoneEvent(std::shared_ptr<LevelDoneEvent> ev) {
}

void DynamicFramework::handleLivesEvent(std::shared_ptr<LivesEvent> ev) {
}

void DynamicFramework::handlePointsEvent(std::shared_ptr<PointsEvent> ev) {
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

MoveEvent::MoveEvent(std::shared_ptr<IEventSource> source, int type, int moveDivider) : Event(std::move(source)), type(type), moveDevider(moveDivider) {

}

int MoveEvent::getMoveDevider() {
    return moveDevider;
}

int MoveEvent::getType() {
    return type;
}

GameletIterator::GameletIterator(std::shared_ptr<Level> level) : level(std::move(level)) {

}

void Level::handleGameEvent(std::shared_ptr<GameEvent> ev) {

}

void Level::handleLevelDoneEvent(std::shared_ptr<LevelDoneEvent> ev) {

}

void Level::handleLivesEvent(std::shared_ptr<LivesEvent> ev) {

}

void Level::handlePointsEvent(std::shared_ptr<PointsEvent> ev) {

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

SimpleGameletIterator::SimpleGameletIterator(std::shared_ptr<Level> level, std::vector<std::shared_ptr<Gamelet>> gamelets) :
        GameletIterator(std::move(level)), gamelets(std::move(gamelets)) {

}

std::shared_ptr<Gamelet> SimpleGameletIterator::nextGamelet() {
    if (index < gamelets.size()) {

        return gamelets[index++];
    }

    return nullptr;
}

std::string TestGamelet::getName() {
    return "TestGamelet";
}

void TestGamelet::prepareCollision(std::shared_ptr<CollisionEvent> ev) {

}

void TestGamelet::performCollision() {

}

std::string Border::getName() {
    return "Border";
}

bool Border::getOpen() {
    return open;
}

void Border::setOpen(bool open) {
    this->open = open;
}

void Border::prepareCollision(std::shared_ptr<CollisionEvent> ev) {
    Gamelet::prepareCollision(ev);
    if (open) {
        if (auto tmp = std::dynamic_pointer_cast<Gamelet>(ev->getOtherCollider(shared_from_this()))) {
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


void Gamelet::remove() {
    // TODO
//    setAnimIndex(-1);
//    if (levelAddedTo != null) levelAddedTo.gameletToRemove(this);
}

void Gamelet::performCollision() {

}

void Gamelet::prepareCollision(std::shared_ptr<CollisionEvent> ev) {

}

void Gamelet::addToLevel(std::shared_ptr<Level> level) {
    if (level != nullptr) {
        level->addTickListener(shared_from_this());
        levelAddedTo = level;
    }
}

void Gamelet::tickOccured(std::shared_ptr<TickEvent> ev) {
    // TODO
    // if (intersections.isIntersected()) drawCnt = 2;
}

void Gamelet::addGameListener(std::shared_ptr<IGameListener> l) {
    gameListeners.push_back(l);
}

std::shared_ptr<Level> Gamelet::getLevelAddedTo() {
    return levelAddedTo;
}

void Gamelet::handleGameEvent(std::shared_ptr<GameEvent> ev) {

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

TickEvent::TickEvent(std::shared_ptr<IEventSource> source) : Event(std::move(source)) {

}
