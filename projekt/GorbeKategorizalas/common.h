#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <time.h>
#include <math.h>
#include <random>

#include <thread>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <windows.h>

#include <tchar.h>
#include <cctype> // is_alpha
#include <algorithm>

#include <list>
#include <utility>
#include <mutex>
#include <memory>

#include <stdlib.h>
#include <sstream>
#include <curl/curl.h>
#include <ws2tcpip.h>

#include <fcntl.h>
#include <unistd.h>

#include <chrono>
#include <functional>





#define SZELES 640
#define MAGAS 520


std::string toLower(const std::string& str);

std::string toUpper(const std::string& str);

void main2( SDL_Window &window, SDL_Renderer &renderer);

#endif // COMMON_H_INCLUDED
