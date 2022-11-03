#ifndef PLAYER_H
 #define PLAYER_H

 #include <stdbool.h>

 #include <SDL.h>

 extern SDL_Window*     g_window;
 extern SDL_Renderer*   g_renderer;
 extern bool            g_serverRunning;

 extern pthread_t       g_clientThread;

 typedef enum       e_direction
 {
            DIR_IDLE,
            DIR_UP,
            DIR_DOWN,
            DIR_LEFT,
            DIR_RIGHT,
            DIR_UP_LEFT,
            DIR_UP_RIGHT,
            DIR_DOWN_LEFT,
            DIR_DOWN_RIGHT
 }                  t_direction;

 typedef struct     s_player
 {
     char            name[256];
     int             x;
     int             y;
     short           vx;
     short           vy;
     int             xCell;
     int             yCell;
     int             health;
     int             score;
     t_direction     direction;
 }                  t_player;

 char            *getUsername();
 t_player        *initPlayer();
 t_player        *getPlayer();
 t_direction     getDirection(const t_player *player);
 bool            inMultiplayer();
 bool            isMoving(const t_player *player);
 void            sendPos();
 void            doSendPos(const t_player *player);

#endif