/**
 * Copyright (c) EJE Electronics B.V. All rights reserved.
 *
 * Licensed under the MIT License.
 */

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

TEST(ExponentialMovingAverage, OptimizeCoefficientSimple) {
    exponential_moving_average_t ema;

    ema_init(&ema, 1000, 1, 0, 1);
    ema_optimize_coefficient(&ema);
    EXPECT_EQ(ema.k_num, 1 << ema.k_log2_denom);
    EXPECT_EQ(ema.filtered, 0);
    EXPECT_EQ(ema_update(&ema, 1, 1000), 1);
}

TEST(ExponentialMovingAverage, OptimizeCoefficient) {
    exponential_moving_average_t ema;

    // Use coefficient 1/4
    ema_init(&ema, 1000, 1, 2, 1);
    ema_optimize_coefficient(&ema);
    EXPECT_EQ(ema.k_num, 1 << (ema.k_log2_denom - 2));

    // Filtered value is 2/4 = 1/2 after update
    EXPECT_EQ(ema_update(&ema, 2, 1000), 0);
    EXPECT_EQ(ema._filtered_num, 2 << (ema.k_log2_denom - 2));

    // Filtered value is (3/4) * (1/2) + 1/2 = 7/8 after update
    EXPECT_EQ(ema_update(&ema, 2, 1000), 0);
    EXPECT_EQ(ema._filtered_num, 7 << (ema.k_log2_denom - 3));

    // Filtered value is (3/4) * (7/8) + 1/2 = 37/32 after update
    EXPECT_EQ(ema_update(&ema, 2, 1000), 1);
    EXPECT_EQ(ema._filtered_num, 37 << (ema.k_log2_denom - 5));
}
