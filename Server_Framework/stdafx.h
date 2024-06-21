#pragma once

#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <MSWSock.h>

#include <tchar.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <array>
#include <unordered_map>
#include <windows.h>  
#include <clocale>
#include <sqlext.h> 
#include <wchar.h>
#include <new>
#include <DirectXMath.h>

#include "protocol.h"
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "MSWSock.lib")

using namespace DirectX;

#define DIR_FORWARD  0x01
#define DIR_BACKWARD 0x02
#define DIR_LEFT     0x04
#define DIR_RIGHT    0x08
#define DIR_UP       0x10
#define DIR_DOWN     0x20