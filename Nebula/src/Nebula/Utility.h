#pragma once

// Limits the framerate to targetFPS by calculating the required time to sleep this thread and thus reaching the targetFPS.
void LimitFPS(int targetFPS);