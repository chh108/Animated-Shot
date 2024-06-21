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

#include "../Server_Framework/protocol.h"
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "MSWSock.lib")