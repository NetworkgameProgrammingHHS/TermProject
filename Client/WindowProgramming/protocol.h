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
	char color;
	//char collide;
};

struct CS_PLAYER_RESET_PACKET {
	char type;
	char reset;
};

struct CS_NEXT_STAGE_PACKET
{
	char type;
};

//struct CS_GAMECLEAR_PACKET
//{
//	char type;
//};

//Server to Client packet
struct SC_READY_PACKET {
	char type;
};

struct SC_RANK_PACKET {
	char type;
	char winner_name[NAME_SIZE];
};

struct SC_LOGIN_INFO_PACKET {
	char type;
	short id;
	char online_p1;
	char online_p2;
	char online_p3;
};

struct SC_REMOVE_PACKET {
	char type;
	short id;
};

struct SC_GAMESTART_PACKET {
	char type;
};

struct SC_WORLD_UPDATE_PACKET {
	char type;

	char color_p1;
	short stage_p1;
	short x_p1, y_p1;
	char dir_p1;

	char color_p2;
	short stage_p2;
	short x_p2, y_p2;
	char dir_p2;

	char color_p3;
	short stage_p3;
	short x_p3, y_p3;
	char dir_p3;

	char bullet_enable;
	short x_bullet, y_bullet;
	char dir_bullet;
};
#pragma pack(pop)

//Packet Type
constexpr char PACKET_NONE = -1;

constexpr char CS_LOGIN = 1;
constexpr char CS_PLAYER_READY = 2;
constexpr char CS_INPUT = 3;
constexpr char CS_COLOR = 4;
constexpr char CS_PLAYER_RESET = 5;
//constexpr char CS_GAMECLEAR = 6;
constexpr char CS_NEXTSTAGE = 6;

constexpr char SC_LOGIN_INFO = 1;
constexpr char SC_READY = 2;
constexpr char SC_GAMESTART = 3;
constexpr char SC_WORLD_UPDATE = 4;
constexpr char SC_RANK = 5;
constexpr char SC_REMOVE = 6;

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

constexpr char POTION_COLLIDE_ON = 1;
constexpr char POTION_COLLIDE_OFF = 2;

constexpr char CLIENT_ONLINE = 1;
constexpr char CLIENT_OFFLINE = 2;

constexpr char KEY_PRESS = 1;
constexpr char KEY_RELEASE = 2;

constexpr char STAGE_1 = 1;
constexpr char STAGE_2 = 2;
constexpr char STAGE_3 = 3;
constexpr char STAGE_4 = 4;
constexpr char STAGE_5 = 5;
constexpr char STAGE_END = 6;