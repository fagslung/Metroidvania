//
// Created by Martin Eberle on 23.12.2018.
//

#include "gtest/gtest.h"
#include "dynamics.h"

TEST(Dynamics, DynamicFramework) {
    std::shared_ptr<DynamicFramework> df = std::make_shared<DynamicFramework>();
    std::cout << df->toString() << std::endl;
}

TEST(Dynamics, MoveEvent) {
    //typedef std::shared_ptr<IEventSource> EventSource;
    auto df = std::make_shared<DynamicFramework>();
    auto me = std::make_shared<MoveEvent>(df, MoveEvent::BEGIN, 10);
    //MoveEvent me = MoveEvent(df, MoveEvent::BEGIN, 10);
    std::cout << me->toString() << std::endl;
    ASSERT_TRUE(me->getMoveDevider() == 10 && me->getType() == MoveEvent::BEGIN);
}

TEST(Dynamics, SimpleGameletIterator) {
    Level l{};
    std::vector<std::shared_ptr<Gamelet>> gamelets;
    auto gameletIter = SimpleGameletIterator(l, gamelets);
    ASSERT_TRUE(gameletIter.nextGamelet() == nullptr);

    gamelets.push_back(std::make_shared<TestGamelet>());
    gameletIter = SimpleGameletIterator(l, gamelets);
    ASSERT_TRUE(gameletIter.nextGamelet()->getName() == "TestGamelet");
    ASSERT_TRUE(gameletIter.nextGamelet() == nullptr);

    gamelets.push_back(std::make_shared<TestGamelet>());
    gameletIter = SimpleGameletIterator(l, gamelets);
    ASSERT_TRUE(gameletIter.nextGamelet()->getName() == "TestGamelet");
    ASSERT_TRUE(gameletIter.nextGamelet()->getName() == "TestGamelet");
    ASSERT_TRUE(gameletIter.nextGamelet() == nullptr);
}


