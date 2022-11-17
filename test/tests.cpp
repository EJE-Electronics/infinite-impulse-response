#include <gtest/gtest.h>

#include "exponential_moving_average.h"


TEST(ExponentialMovingAverage, NoFilter) {
    exponential_moving_average_t ema;

    // Use coefficient 1, so that the filtered value is the same as the input.
    ema_init(&ema, 1000, 1, 0, 1);
    EXPECT_EQ(ema.filtered, 0);
    EXPECT_EQ(ema_update(&ema, 1, 1000), 1);
    EXPECT_EQ(ema.filtered, 1);
}

TEST(ExponentialMovingAverage, Simple) {
    exponential_moving_average_t ema;

    // Use coefficient 1/2, so that the filtered value is the average of the
    // input and the previous filtered value.
    ema_init(&ema, 1000, 1, 1, 1);
    EXPECT_EQ(ema.filtered, 0);
    EXPECT_EQ(ema_update(&ema, 2, 1000), 1);
    EXPECT_EQ(ema.filtered, 1);
    EXPECT_EQ(ema_update(&ema, 11, 1000), 6);
}

TEST(ExponentialMovingAverage, HalfIntervals) {
    exponential_moving_average_t ema;

    // Use coefficient 1/2, so that the filtered value is the average of the
    // input and the previous filtered value.
    ema_init(&ema, 1000, 1, 1, 1);
    EXPECT_EQ(ema.filtered, 0);
    EXPECT_EQ(ema_update(&ema, 1000, 500), 0);
    EXPECT_EQ(ema.filtered, 0);
    EXPECT_EQ(ema_update(&ema, 2, 500), 1);
}

TEST(ExponentialMovingAverage, NegativeValues) {
    exponential_moving_average_t ema;

    // Use coefficient 1/2, so that the filtered value is the average of the
    // input and the previous filtered value.
    ema_init(&ema, 1000, 1, 1, 1);
    EXPECT_EQ(ema.filtered, 0);
    EXPECT_EQ(ema_update(&ema, -2, 1000), -1);
    EXPECT_EQ(ema_update(&ema, -11, 1000), -6);
}
