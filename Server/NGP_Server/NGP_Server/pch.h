#pragma once

#include <string>
#include <array>
#include <fstream>
#include <memory>

#include "protocol.h"

using namespace std;

#define TILE_NUM_W 25
#define TILE_NUM_H 19

#define STAGE_NUM 5

#define BULLET_SPEED 300.f;

constexpr char DIR_LEFT = 1;
constexpr char DIR_RIGHT = 2;
constexpr char DIR_UP = 3;
constexpr char DIR_DOWN = 4;

struct Vec2 {
	float x = 0.f;
	float y = 0.f;
};

enum class PLAYER_COLOR {
	NORMAL, RED, GREEN, BLUE, YELLOW, PURPLE, GB, END
};