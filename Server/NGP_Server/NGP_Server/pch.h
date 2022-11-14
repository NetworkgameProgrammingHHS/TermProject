#pragma once

#include <string>
#include <iostream>
#include <array>
#include <fstream>
#include <memory>
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32")


#include "protocol.h"

using namespace std;

#define TILE_NUM_W 25
#define TILE_NUM_H 19

#define PLAYER_NUM 3
#define STAGE_NUM 5

#define BULLET_SPEED 300.f;

struct Vec2 {
	float x = 0.f;
	float y = 0.f;
};

struct SESSION {
	SOCKET client_sock = NULL;
	int id = 0;
};

enum class PLAYER_COLOR {
	NORMAL, RED, GREEN, BLUE, YELLOW, PURPLE, GB, END
};