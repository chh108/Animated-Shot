constexpr int PORT_NUM = 9000;	//portnum
constexpr int BUF_SIZE = 256;
constexpr int NAME_SIZE = 20;	//id, ¿Ã∏ß

constexpr int MAX_USER = 10;
// Packet ID
constexpr char CS_LOGIN = 0;
constexpr char CS_MOVE = 1;
constexpr char CS_CAMERA = 2;
constexpr char CS_ANIMATION = 3;

constexpr char SC_LOGIN_FAIL = 4;
constexpr char SC_LOGIN_SUCESS_INFO = 5;
constexpr char SC_ADD_PLAYER = 6;
constexpr char SC_REMOVE_PLAYER = 7;
constexpr char SC_MOVE_PLAYER = 8;
constexpr char SC_CAMERA_PLAYER = 9;
constexpr char SC_ANIMATION_PLAYER = 10;

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
	float x;
	float y;
	float z;
};

struct CS_CAMERA_PACKET
{
	unsigned char size;
	char	type;

	DirectX::XMFLOAT3 f3Right;
	DirectX::XMFLOAT3 f3Look;
	DirectX::XMFLOAT3 f3Up;
};

struct CS_ANIMATION_PACKET
{
	unsigned char size;
	char	type;
	char	animation;
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

struct SC_CAMERA_PLAYER_PACKET
{
	unsigned char size;
	char	type;
	short	id;

	DirectX::XMFLOAT3 f3Right;
	DirectX::XMFLOAT3 f3Look;
	DirectX::XMFLOAT3 f3Up;
};

struct SC_ANIMATION_PLAYER_PACKET
{
	unsigned char size;
	char	type;
	short	id;
	char	animation;
};
#pragma pack (pop)