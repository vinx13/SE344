/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SPEED_H
#define SPEED_H

const double kGravityCoff = -0.5;

struct Speed {
    double x, y, z;
    double yaw, roll, pitch;
};

#endif //SPEED_H
