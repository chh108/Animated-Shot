constexpr int PORT_NUM = 9000;	//portnum
constexpr int BUF_SIZE = 256;
constexpr int NAME_SIZE = 20;	//id, ¿Ã∏ß

constexpr int MAX_USER = 10;
// Packet ID
constexpr char CS_LOGIN = 0;
constexpr char CS_MOVE = 1;

constexpr char SC_LOGIN_FAIL = 2;
constexpr char SC_LOGIN_SUCESS_INFO = 3;
constexpr char SC_ADD_PLAYER = 4;
constexpr char SC_REMOVE_PLAYER = 5;
constexpr char SC_MOVE_PLAYER = 6;

#pragma pack (push, 1)
struct CS_LOGIN_PACKET {
	unsigned char size;
	char	type;
	TCHAR	name[NAME_SIZE];
};

struct CS_MOVE_PACKET {
	unsigned char size;
	char	type;
	DWORD	direction;



	float f3Right[3];
	float f3Look[3];
	float f3Up[3];
};

struct SC_LOGIN_SUCESS_INFO_PACKET {
	unsigned char size;
	char	type;
	short	id;
	char	chartype;		//1,2,3
	short	x, y, z;
	short	level;
	short	hp;
	short	exp;
};

struct SC_LOGIN_FAIL_PACKET {
	unsigned char size;
	char	type;
};

struct SC_ADD_PLAYER_PACKET {
	unsigned char size;
	char	type;
	short	id;
	float	x, y, z;
	char	chartype;
	short	level;
	short	hp;
	char	name[NAME_SIZE];
};

struct SC_REMOVE_PLAYER_PACKET {
	unsigned char size;
	char	type;
	short	id;
};

struct SC_MOVE_PLAYER_PACKET {
	unsigned char size;
	char	type;
	short	id;
	char	chartype;
	float	x, y, z;
};

#pragma pack (pop)