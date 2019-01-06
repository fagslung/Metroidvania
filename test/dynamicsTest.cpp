//
// Created by Martin Eberle on 23.12.2018.
//

#include "gtest/gtest.h"
#include "dynamics.h"
#include "geometry.h"

TEST(Dynamics, DynamicFramework) {
    std::shared_ptr<DynamicFramework> df = std::make_shared<DynamicFramework>();
    std::cout << df->toString() << std::endl;

    auto l = std::make_shared<Level>();
    ASSERT_EQ(0, l->getGameListeners().size());

    df->setLevel(l);
    ASSERT_EQ(1, l->getGameListeners().size());

    l->removeGameListener(df);
    ASSERT_EQ(0, l->getGameListeners().size());
}

TEST(Dynamics, MoveEvent) {
    //typedef std::shared_ptr<IEventSource> EventSource;
    auto df = std::make_shared<DynamicFramework>();
    auto me = std::make_shared<MoveEvent>(df, MoveEvent::BEGIN, 10);
    //MoveEvent me = MoveEvent(df, MoveEvent::BEGIN, 10);
    std::cout << me->toString() << std::endl;
    ASSERT_TRUE(me->getMoveDivider() == 10 && me->getType() == MoveEvent::BEGIN);
}

TEST(Dynamics, SimpleGameletIterator) {
    auto l = std::make_shared<Level>();
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

TEST(Dynamics, Level) {
    Level l;

    l.removeGameListener(nullptr);
    ASSERT_TRUE(l.getGameListeners().empty());

    l.removeGameListener(std::make_shared<TestGamelet>());
    ASSERT_TRUE(l.getGameListeners().empty());

    auto gl0 = std::make_shared<TestGamelet>();
    l.addGameListener(gl0);
    ASSERT_TRUE(l.getGameListeners().size() == 1);

    auto gl1 = std::make_shared<TestGamelet>();
    l.addGameListener(gl1);
    ASSERT_TRUE(l.getGameListeners().size() == 2);

    l.removeGameListener(gl0);
    ASSERT_TRUE(l.getGameListeners().size() == 1);

    l.removeGameListener(gl0);
    ASSERT_TRUE(l.getGameListeners().size() == 1);

    l.addGameListener(gl0);
    ASSERT_TRUE(l.getGameListeners().size() == 2);

    auto gl2 = std::make_shared<DynamicFramework>();
    l.addGameListener(gl2);
    ASSERT_TRUE(l.getGameListeners().size() == 3);

    l.removeGameListener(gl0);
    l.removeGameListener(gl2);
    l.removeGameListener(gl1);
    ASSERT_TRUE(l.getGameListeners().empty());

    auto df = DynamicFramework();
    auto pdf = std::make_shared<DynamicFramework>(df);
    l.addGameListener(pdf);
    ASSERT_TRUE(l.getGameListeners().size() == 1);

    l.removeGameListener(std::make_shared<DynamicFramework>(df));
    ASSERT_TRUE(l.getGameListeners().size() == 1);

    l.removeGameListener(pdf);
    ASSERT_TRUE(l.getGameListeners().empty());

    l.setSize(Dimension{1, 2});
    ASSERT_EQ(1, l.getSize().width);
    ASSERT_EQ(2, l.getSize().height);

    auto pL = std::make_shared<Level>();
    auto border = std::make_shared<Border>();
    pL->addGamelet(border);
    ASSERT_TRUE(border->getLevelAddedTo() == pL);

    // TODO   Level->getIntersections().erase
}


