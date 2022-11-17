/**
 * Copyright (c) EJE Electronics B.V. All rights reserved.
 *
 * Licensed under the MIT License.
 */

#ifndef EXPONENTIAL_MOVING_AVERAGE_H
#define EXPONENTIAL_MOVING_AVERAGE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Describes an exponential moving average filter.
 *
 * At each interval, the filter output is updated as follows:
 *   filtered += ((input - filtered) * k_num) >> k_log2_denom
 */
typedef struct {
    uint32_t interval;          /**< The interval between filter updates */
    uint32_t time_since_update; /**< The time since the last filter update */
    uint32_t max_iterations;    /**< The maximum number of iterations to perform
                                   per update */

    /*@{*/
    int32_t k_num; /**< numerator of the filter coefficient */
    int8_t
        k_log2_denom; /**< log2 of the denominator of the filter coefficient */
    /*@}*/

    int64_t _filtered_num; /**< filtered value * 2^k_log2_denom */
    int32_t filtered;      /**< the current filtered value */
} exponential_moving_average_t;

/**
 * Sets the filter coefficient.
 *
 * @param ema The filter struct to update.
 * @param k_num The numerator of the filter coefficient.
 * @param k_log2_denom The log2 of the denominator of the filter coefficient.
 */
static void ema_set_coefficient(exponential_moving_average_t *ema,
                                int32_t k_num, int8_t k_log2_denom) {
    ema->k_num = k_num;
    ema->k_log2_denom = k_log2_denom;
}

static void ema_init(exponential_moving_average_t *ema, uint32_t interval,
                     int32_t k_num, int8_t k_log2_denom,
                     uint32_t max_iterations) {
    ema->interval = interval;
    ema->time_since_update = 0;
    ema->max_iterations = max_iterations;
    ema->_filtered_num = 0;
    ema->filtered = 0;
    ema_set_coefficient(ema, k_num, k_log2_denom);
}

/**
 * Updates the filter with a new input value.
 *
 * @param ema The filter state.
 * @param input Current value.
 * @param dt Time since last call. Must have the same units as ema->interval.
 * @returns The new filtered value.
 */
static int32_t ema_update(exponential_moving_average_t *ema, int32_t input,
                          uint32_t dt) {
    ema->time_since_update += dt;
    if (ema->time_since_update >= ema->interval * (ema->max_iterations + 1)) {
        ema->time_since_update = 0;
        ema->_filtered_num = (int64_t)input << ema->k_log2_denom;
        ema->filtered = input;
        return input;
    }

    while (ema->time_since_update >= ema->interval) {
        ema->time_since_update -= ema->interval;
        int64_t diff_num =
            ((int64_t)input << ema->k_log2_denom) - ema->_filtered_num;
        ema->_filtered_num += (diff_num * ema->k_num) >> ema->k_log2_denom;
    }
    ema->filtered = ema->_filtered_num >> ema->k_log2_denom;

    return ema->filtered;
}

#ifdef __cplusplus
}
#endif

#endif /* EXPONENTIAL_MOVING_AVERAGE_H */
