//
// Created by Martin Eberle on 28.12.2018.
//

#include "gtest/gtest.h"
#include "geometry.h"
#include <SDL.h>

TEST(Geometry, Millipoint) {
    MilliPoint mp = MilliPoint();
    //std::cout << "x=" << mp.x << ", y=" << mp.y << std::endl;
    ASSERT_TRUE(mp.x == 0 && mp.y == 0);

    MilliPoint mp2 = MilliPoint(3, 4);
    ASSERT_TRUE(mp2.x == 3 && mp2.y == 4);

    MilliPoint mp3(mp2);
    ASSERT_TRUE(mp3.x == mp2.x && mp3.y == mp2.y);

    MilliPoint mp4(MilliPoint(-1, -2));
    ASSERT_TRUE(mp4.x == -1 && mp4.y == -2);

    SDL_Point sp{1, 2};
    MilliPoint mp5(sp);
    ASSERT_TRUE(mp5.x == 1000 && mp5.y == 2000);

    MilliPoint mp6 = mp5;
    ASSERT_TRUE(mp6.x == 1000 && mp6.y == 2000);

    MilliPoint mp7 = sp;
    ASSERT_TRUE(mp7.x == 1000 && mp7.y == 2000);
    ASSERT_TRUE(sp.x == 1 && sp.y == 2);

    MilliPoint mp8 = mp7 + mp6;
    ASSERT_TRUE(mp8.x == 2000 && mp8.y == 4000);

    SDL_Point sp2 = mp8;
    ASSERT_TRUE(sp2.x == 2 && sp2.y == 4);

    MilliPoint mp9{20, 30};
    SDL_Point sp3{60, 10};
    mp9 = sp3;
    ASSERT_EQ(60000, mp9.x);
    ASSERT_EQ(10000, mp9.y);
}



TEST(Geometry, Millirect) {

    MilliRect mr = MilliRect();
    ASSERT_TRUE(mr.x == 0 && mr.y == 0 && mr.w == 0 && mr.h == 0);

    MilliRect mr2 = MilliRect(1, 2, 3, 4);
    ASSERT_TRUE(mr2.x == 1 && mr2.y == 2 && mr2.w == 3 && mr2.h == 4);

    MilliRect mr3(mr2);
    ASSERT_TRUE(mr3.x == mr2.x && mr3.y == mr2.y && mr3.w == mr2.w && mr3.h == mr2.h);

    MilliRect mr4(MilliRect(10, 20, 5, 5));
    ASSERT_TRUE(mr4.x == 10 && mr4.y == 20 && mr4.w == 5 && mr4.h == 5);

    SDL_Rect sr{1, 2, 5, 5};
    MilliRect mr5(sr);
    ASSERT_TRUE(mr5.x == 1000 && mr5.y == 2000 && mr5.w == 5000 && mr5.h == 5000);

    MilliRect mr6 = mr5;
    ASSERT_TRUE(mr6.x == 1000 && mr6.y == 2000 && mr6.w == 5000 && mr6.h == 5000);

    MilliRect mr7 = sr;
    ASSERT_TRUE(mr7.x == 1000 && mr7.y == 2000 && mr7.w == 5000 && mr7.h == 5000);
    ASSERT_TRUE(sr.x == 1 && sr.y == 2 && sr.w == 5 && sr.h == 5);

    SDL_Rect sr2 = mr7;
    ASSERT_TRUE(sr2.x == sr.x && sr2.y == sr.y && sr2.w == sr.w && sr2.h == sr.h);

    mr7.setPosition(MilliPoint{900, 900});
    ASSERT_TRUE(mr7.x == 900 && mr7.y == 900 && mr7.w == 5000 && mr7.h == 5000);

    mr7.setDimension(MilliPoint{750, 430});
    ASSERT_TRUE(mr7.x == 900 && mr7.y == 900 && mr7.w == 750 && mr7.h == 430);

    mr7.move(MilliPoint{23, 40});
    ASSERT_TRUE(mr7.x == 923 && mr7.y == 940 && mr7.w == 750 && mr7.h == 430);

    mr7.increaseDimension(MilliPoint{1, 1});
    ASSERT_TRUE(mr7.x == 923 && mr7.y == 940 && mr7.w == 751 && mr7.h == 431);

    mr7.increaseDimension(MilliPoint{-1, -1});
    ASSERT_TRUE(mr7.x == 922 && mr7.y == 939 && mr7.w == 752 && mr7.h == 432);

    ASSERT_FALSE(mr7.isIntersectingWith(mr6));
    ASSERT_FALSE(mr7.isIntersectingWith(mr5));
    ASSERT_TRUE(MilliRect(100, 200, 50, 60).isIntersectingWith(MilliRect(120, 150, 30, 80)));

    Dimension d{1, 2};
    mr7 = d;
    ASSERT_EQ(1000, mr7.w);
    ASSERT_EQ(2000, mr7.h);
}