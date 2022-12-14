#pragma once

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 608;

constexpr int TILE_NUM_W = 25;
constexpr int TILE_NUM_H = 19;

constexpr int TILE_SIZE = 32;

#define NAME_SIZE 20
#define BUF_SIZE 256
#define SERVER_PORT 9000

#pragma pack(push, 1)
//Client to Server packet
struct CS_LOGIN_PACKET {
	char type;
	char name[NAME_SIZE];
};

struct CS_INPUT_PACKET {
	char type;
	char key;
	char state;
};

struct CS_PLAYER_READY_PACKET {
	char type;
	char ready;
};

struct CS_PLAYER_COLOR_PACKET {
	char type;
	short color;
};

struct CS_PLAYER_RESET_PACKET {
	char type;
	char reset;
};

struct CS_NEXT_STAGE_PACKET 
{
	char type;
	int n;
};

//Server to Client packet
struct SC_READY_PACKET {
	char type;
	char ready;
	short id;
};

struct SC_RANK_PACKET {
	char type;
	char winner_name[NAME_SIZE];
};

struct SC_LOGIN_INFO_PACKET {
	char type;
	short id;
	bool online_p1;
	bool online_p2;
	bool online_p3;
	char name_p1[NAME_SIZE];
	char name_p2[NAME_SIZE];
	char name_p3[NAME_SIZE];
	bool ready_p1;
	bool ready_p2;
	bool ready_p3;
};

struct SC_RESET_PACKET {
	char type;
	short id;
};

struct SC_REMOVE_PACKET {
	char type;
	short id;
};

struct SC_GAMESTART_PACKET {
	char type;
	int n;
};

struct SC_WORLD_UPDATE_PACKET {
	char type = ' ';

	short stage_p1 = 0;
	float x_p1 = TILE_SIZE, y_p1 = WINDOW_HEIGHT - 2 * TILE_SIZE;
	short dir_p1 = 0;

	short stage_p2 = 0;
	float x_p2 = TILE_SIZE, y_p2 = WINDOW_HEIGHT - 2 * TILE_SIZE;
	short dir_p2 = 0;

	short stage_p3 = 0;
	float x_p3 = TILE_SIZE, y_p3 = WINDOW_HEIGHT - 2 * TILE_SIZE;
	short dir_p3 = 0;

	char bullet_enable = ' ';
	char stage_bullet = ' ';
	float x_bullet = 0.f, y_bullet = WINDOW_HEIGHT - 2 * TILE_SIZE;
	short dir_bullet = 0;
};

struct SC_COLOR_PACKET {
	char type;
	short id;
	short color;
};
#pragma pack(pop)

//Packet Type
constexpr char PACKET_NONE = -1;

constexpr char CS_LOGIN = 1;
constexpr char CS_PLAYER_READY = 2;
constexpr char CS_INPUT = 3;
constexpr char CS_COLOR = 4;
constexpr char CS_PLAYER_RESET = 5;
constexpr char CS_NEXTSTAGE = 6;

constexpr char SC_LOGIN_INFO = 1;
constexpr char SC_READY = 2;
constexpr char SC_GAMESTART = 3;
constexpr char SC_WORLD_UPDATE = 4;
constexpr char SC_RANK = 5;
constexpr char SC_REMOVE = 6;
constexpr char SC_COLOR = 7;
constexpr char SC_RESET = 8;

//Packet Value
constexpr char KEY_DIR_LEFT = 1;
constexpr char KEY_DIR_RIGHT = 2;
constexpr char KEY_DIR_UP = 3;
constexpr char KEY_DIR_DOWN = 4;
constexpr char KEY_FIREGUN = 5;

constexpr char READY_ON = 1;
constexpr char READY_OFF = 2;

constexpr char RESET_ON = 1;
constexpr char RESET_OFF = 2;

constexpr char BULLET_ON = 1;
constexpr char BULLET_OFF = 2;
constexpr char GUN_OBJECT = 3;

constexpr bool CLIENT_ONLINE = true;
constexpr bool CLIENT_OFFLINE = false;

constexpr char KEY_PRESS = 1;
constexpr char KEY_RELEASE = 2;

constexpr char STAGE_1 = 1;
constexpr char STAGE_2 = 2;
constexpr char STAGE_3 = 3;
constexpr char STAGE_4 = 4;
constexpr char STAGE_5 = 5;
constexpr char STAGE_END = 6;