/*
 *  ファイル名  : client_common.h
 *  機能    : クライアント用のマクロ、構造体等の定義
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "../header/constants.h"
#include "client_define.h"
#include "client_map.hpp"
#include "client_system.hpp"
#include "client_WiiInput.h"
#include "client_KeybordInput.h"
#include "client_time.hpp"
#include "client_gamestate.hpp"
#include "graphic.h"
#include "client_smart.h"
#include "client_music.hpp"

/* net.c */
extern void SetupClient(char *, u_short);
extern void TerminateClient();
extern int ControlRequests();
extern int InCommand(char com);

// ----- system ----- //
System::ClientSystem& GetSystem();

/*control_setup*/
extern int ControlSetUp();

/*name_setup*/
extern void GetInitData(System::InitData initData);
extern void NameSetUp();


extern void KillGoServer();
extern void Goroutine();

int ShowResult();
