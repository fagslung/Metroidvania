//
// Created by Martin Eberle on 23.12.2018.
//

#include "gtest/gtest.h"
#include "dynamics.h"

TEST(Dynamics, DynamicFramework) {
    std::shared_ptr<DynamicFramework> df = std::make_shared<DynamicFramework>();
    std::cout << df->toString() << std::endl;
}


