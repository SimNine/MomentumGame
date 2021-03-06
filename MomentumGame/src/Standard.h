#pragma once

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <assert.h>
#include <fstream>
#include <cmath>
#include <thread>
#include <mutex>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "22626"

#include "Enums.h"
#include "Coord.h"
#include "Logger.h"