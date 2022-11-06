#pragma once
#define SFML_STATIC 1
#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "protocol.h"

using namespace std;

#ifdef _DEBUG
#pragma comment (lib, "lib/sfml-audio-s-d.lib")
#pragma comment (lib, "lib/sfml-graphics-s-d.lib")
#pragma comment (lib, "lib/sfml-window-s-d.lib")
#pragma comment (lib, "lib/sfml-system-s-d.lib")
#pragma comment (lib, "lib/sfml-network-s-d.lib")
#else
#pragma comment (lib, "lib/sfml-audio-s.lib")
#pragma comment (lib, "lib/sfml-graphics-s.lib")
#pragma comment (lib, "lib/sfml-window-s.lib")
#pragma comment (lib, "lib/sfml-system-s.lib")
#pragma comment (lib, "lib/sfml-network-s.lib")
#endif
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "winmm.lib")

enum class PLAYER_STATE { IDLE, MOVE_L, MOVE_R, JUMP };

enum class TILE_TYPE {NONE, WALL, 
					  RED_P, GREEN_P, BLUE_P, BLACK_P,
					  RED_T, GREEN_T, BLUE_T, YELLOW_T, PURPLE_T, GB_T, IDLE_T,
					  RED_J, GREEN_J, BLUE_J, YELLOW_J, PURPLE_J, GB_J,
					  RED_G, GREEN_G, BLUE_G, YELLOW_G, PURPLE_G, GB_G,
					  SPOID,
					  END
};

enum class PLAYER_COLOR {
	NORMAL, RED, GREEN, BLUE, YELLOW, PURPLE, GB, END
};

enum class SCENE_NUM {
	NONE, TITLE, STAGE1, STAGE2, STAGE3, STAGE4, STAGE5
};