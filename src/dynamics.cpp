#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <utility>

//
// Created by Martin Eberle on 24.12.2018.
//

#include "dynamics.h"
#include <iostream>


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

MoveEvent::MoveEvent(std::shared_ptr<IEventSource> source, int type, int moveDivider) : Event(std::move(source)), type(type), moveDevider(moveDivider) {

}

int MoveEvent::getMoveDevider() {
    return moveDevider;
}

int MoveEvent::getType() {
    return type;
}
