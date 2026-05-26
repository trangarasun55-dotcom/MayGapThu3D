#ifndef PHYSICS_H
#define PHYSICS_H

#include "globals.h"

void initPhysics();

void resetPhysics();

void updatePhysics(float deltaTime);

void moveClawManual(float dx, float dz);

void startGrabCycle();

#endif