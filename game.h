#ifndef GAME_H
 #define GAME_H

 #include <stdbool.h>

 #include "map.h"
 #include "bomb.h"
 #include "player.h"
 #include "assets.h"
 #include "menu.h"

 extern char            g_username[256];
 extern short           g_playersMultiIndex;

 #define PLAYER_WIDTH   50
 #define PLAYER_HEIGHT  73

 extern t_menu      menuPause;
 extern SDL_Rect    g_buttonsLocation[MAX_MENU_BUTTONS];

 typedef struct     s_game
 {
     t_map               *map;
     t_map               maps[10];
     t_player            **players;
     unsigned short      nbPlayers;
 }                  t_game;

 void    killBots(int xCell, int yCell);
 void    injectItems(const t_map *map);
 void    spawnPlayer(int x, int y, t_player *player);
 bool    inGame();
 bool    isMoving();
 bool    isGamePaused();
 void    resumeGame();
 void    pauseGame();
 void    game_quit();
 void    launchGame();
 t_game  *getGame();
 void    movePlayer(t_player *player);
 void    posToGrid(t_player *player);
 void    posToGridN(int x, int y, int *xCell, int *yCell);
 void    searchDirectionMap(int xCellBase, int yCellBase, t_direction directionMap, int scope, t_player *owner);
 void    handleDamage(t_player *player);
 void    explodeBomb(int xCell, int yCell, t_player *owner);
 void    handleMouseButtonUpPlaying(const SDL_Event *event);
 void    launchGame();
 void    putPlayerInFreeCell(t_player *player);
 Uint32  timedRespawn(Uint32 interval, char *param);
 void    receiveDamage(const char *content);
 void    receiveLife(const char *content);
 bool    checkIfEveryoneIsDead();
 void    receiveEndGame(const char* content);
 void    kickBomb();
 bool    searchPlayerToExplode(int xCell, int yCell);
 void    placeBomb(int xCell, int yCell, t_player *player);
 void    resetBots();

#endif
