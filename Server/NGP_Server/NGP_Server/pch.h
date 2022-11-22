#pragma once

#include <string>
#include <iostream>
#include <array>
#include <fstream>
#include <memory>
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32")


#include "protocol.h"

using namespace std;

#define TILE_NUM_W 25
#define TILE_NUM_H 19

#define PLAYER_NUM 3
#define STAGE_NUM 5

#define BULLET_SPEED 300.f
#define PLAYER_SPEED 25.0f

struct Vec2 {
	float x = 0.f;
	float y = 0.f;
};

enum class PLAYER_COLOR {
	NORMAL, RED, GREEN, BLUE, YELLOW, PURPLE, GB, END
};

enum DIR {
	LEFT = -1, RIGHT = 1, UP = 2, DOWN = 3
};