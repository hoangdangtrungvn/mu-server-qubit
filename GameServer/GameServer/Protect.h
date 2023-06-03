// Protect.h: interface for the CProtect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

// Default value is 0x13 = 0x14 and ax36 = 0x37
#define PROTECT_START _asm _emit 0xEB _asm _emit 0x0E _asm _emit 0x13 _asm _emit 0x28 _asm _emit 0xCD _asm _emit 0xDB _asm _emit 0x50 _asm _emit 0x63 _asm _emit 0xAF _asm _emit 0x22 _asm _emit 0x75 _asm _emit 0xEE _asm _emit 0xB5 _asm _emit 0xD8 _asm _emit 0x43 _asm _emit 0x72
#define PROTECT_FINAL _asm _emit 0xEB _asm _emit 0x0E _asm _emit 0x36 _asm _emit 0xF2 _asm _emit 0xCD _asm _emit 0xDB _asm _emit 0x50 _asm _emit 0x63 _asm _emit 0xAF _asm _emit 0x22 _asm _emit 0x75 _asm _emit 0xEE _asm _emit 0xB5 _asm _emit 0xD8 _asm _emit 0x85 _asm _emit 0x61
// Default value is 0x30 = 0x25
#define CRASH_APPLICATION_MACRO _asm _emit 0xFF _asm _emit 0x30 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00

enum eAuthServerType
{
	AUTH_SERVER_TYPE_NONE = 0,
	AUTH_SERVER_TYPE_S6_CONNECT_SERVER = 1,
	AUTH_SERVER_TYPE_S6_JOIN_SERVER = 2,
	AUTH_SERVER_TYPE_S6_DATA_SERVER = 3,
	AUTH_SERVER_TYPE_S6_GAME_SERVER = 4,
	AUTH_SERVER_TYPE_S8_CONNECT_SERVER = 5,
	AUTH_SERVER_TYPE_S8_JOIN_SERVER = 6,
	AUTH_SERVER_TYPE_S8_DATA_SERVER = 7,
	AUTH_SERVER_TYPE_S8_GAME_SERVER = 8,
	AUTH_SERVER_TYPE_S4_CONNECT_SERVER = 9,
	AUTH_SERVER_TYPE_S4_JOIN_SERVER = 10,
	AUTH_SERVER_TYPE_S4_DATA_SERVER = 11,
	AUTH_SERVER_TYPE_S4_GAME_SERVER = 12,
	AUTH_SERVER_TYPE_S2_CONNECT_SERVER = 13,
	AUTH_SERVER_TYPE_S2_JOIN_SERVER = 14,
	AUTH_SERVER_TYPE_S2_DATA_SERVER = 15,
	AUTH_SERVER_TYPE_S2_GAME_SERVER = 16,
};

enum eAuthServerStatus
{
	AUTH_SERVER_STATUS_NONE = 1,
	AUTH_SERVER_STATUS_SUCCESS = 2,
};

struct SDHP_AUTH_SERVER_DATA_RECV
{
	BYTE type; // C1:00
	BYTE size;
	BYTE head;
	BYTE EncKey;
	BYTE ServerType;
	BYTE Status;
	BYTE PackageType;
	BYTE PlanType;
	char CustomerName[32];
	char CustomerHardwareId[36];
};

struct SDHP_AUTH_SERVER_DATA_SEND
{
	BYTE type; // C1:00
	BYTE size;
	BYTE head;
	BYTE EncKey;
	BYTE ServerType;
	char CustomerName[32];
	char CustomerHardwareId[36];
};

struct SDHP_AUTH_SERVER_ONLINE_SEND
{
	BYTE type; // C1:00
	BYTE size;
	BYTE head;
	BYTE EncKey;
	char CustomerName[32];
};

struct SDHP_AUTH_SERVER_ONLINE_RECV
{
	BYTE type; // C1:00
	BYTE size;
	BYTE head;
	BYTE EncKey;
	BYTE result;
};


class CProtect
{
public:
	CProtect();
	virtual ~CProtect();
	bool GetComputerHardwareId();
	bool ConnectToAuthServer(char* IpAddress,WORD port);
	bool AuthServerDataSend(eAuthServerType ServerType);
	bool AuthServerOnlineSend();
	void StartAuth(eAuthServerType ServerType);
	void OnlineAuth();
	inline void ErrorMessageBox(int Erro);
	inline void SafeExitProcess();
	void ProtectBlock(DWORD size);
	void ReleaseBlock(DWORD size);
	void DecryptBlock(BYTE* data,int size);
	void EncryptBlock(BYTE* data,int size);
	inline void InsertMemoryMacro(BYTE* data,BYTE* info,int size);
	inline void RemoveMemoryMacro(BYTE* data,BYTE* info,int size);
	inline bool VerifyMemoryMacro(BYTE* data,BYTE* info,int size);
	bool GetInsertStartBlock(DWORD start_offset,DWORD final_offset,DWORD* start_block);
	bool GetInsertFinalBlock(DWORD start_offset,DWORD final_offset,DWORD* final_block);
	bool GetRemoveStartBlock(DWORD start_offset,DWORD final_offset,DWORD* start_block);
	bool GetRemoveFinalBlock(DWORD start_offset,DWORD final_offset,DWORD* final_block);
	char* GetEncryptedString(BYTE* string,int size);
public:
	SOCKET m_socket;
	SOCKET m_socket2;
	char m_ComputerHardwareId[36];
	BYTE m_EncKey;
	BYTE m_EncKeyNew;
	SDHP_AUTH_SERVER_DATA_RECV m_AuthInfo;
	SDHP_AUTH_SERVER_ONLINE_RECV m_AuthOnlineInfo;
	int Logged;
};

extern CProtect gProtect;

static const BYTE gProtectTable[16] = {0xC8,0x9D,0x24,0xC2,0x10,0xDD,0xE1,0x66,0xF3,0x5D,0x35,0xD2,0xA8,0x96,0xC5,0x48};
static const BYTE gProtectInsertStart[16] = {0x74,0x91,0x8B,0xB7,0x52,0x44,0xCF,0xFC,0x30,0xBD,0xEA,0x71,0x2A,0x47,0xDC,0xED}; //{0xEB,0x0E,0x14,0x28,0xCD,0xDB,0x50,0x63,0xAF,0x22,0x75,0xEE,0xB5,0xD8,0x43,0x72}
static const BYTE gProtectInsertFinal[16] = {0x74,0x91,0xA8,0x6D,0x52,0x44,0xCF,0xFC,0x30,0xBD,0xEA,0x71,0x2A,0x47,0x1A,0xFE}; //{0xEB,0x0E,0x37,0xF2,0xCD,0xDB,0x50,0x63,0xAF,0x22,0x75,0xEE,0xB5,0xD8,0x85,0x61}
static const BYTE gProtectRemoveStart[16] = {0x5C,0x74,0x92,0xB7,0x52,0x44,0xCF,0xFC,0x30,0xBD,0xEA,0x71,0x2A,0x47,0xDC,0xED}; //{0xC3,0xEB,0x0D,0x28,0xCD,0xDB,0x50,0x63,0xAF,0x22,0x75,0xEE,0xB5,0xD8,0x43,0x72}
static const BYTE gProtectRemoveFinal[16] = {0x5C,0x74,0x92,0x6D,0x52,0x44,0xCF,0xFC,0x30,0xBD,0xEA,0x71,0x2A,0x47,0x1A,0xFE}; //{0xC3,0xEB,0x0D,0xF2,0xCD,0xDB,0x50,0x63,0xAF,0x22,0x75,0xEE,0xB5,0xD8,0x85,0x61}
static BYTE gProtectString1[04] = {0xF3,0x8A,0xEC,0xB0}; //"C:\\"
static BYTE gProtectString2[20] = {0x95,0x80,0x88,0xE8,0x9D,0x95,0x80,0x88,0xE8,0x9D,0x95,0x80,0x88,0xE8,0x9D,0x95,0x80,0x88,0xE8,0xB0}; //"%08X-%08X-%08X-%08X"
static BYTE gProtectString3[12] = {0x81,0x82,0x87,0x9E,0x80,0x9E,0x80,0x9E,0x80,0x81,0xB0}; //"127.0.0.1"
static BYTE gProtectString4[35] = {0xF3,0xDF,0xC5,0xDC,0xD4,0x90,0xDE,0xDF,0xC4,0x90,0xD1,0xC5,0xC4,0xD8,0xD5,0xDE,0xC4,0xD9,0xD3,0xD1,0xC4,0xD5,0x90,0xC4,0xD8,0xD5,0x90,0xDD,0xD1,0xD3,0xD8,0xD9,0xDE,0xD5,0xB0}; //"Could not authenticate the machine"
static BYTE gProtectString5[06] = {0xF5,0xC2,0xC2,0xDF,0xC2,0xB0}; //"Error"
