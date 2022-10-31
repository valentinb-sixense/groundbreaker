#ifndef LOOP_H
 #define LOOP_H

 #include <SDL.h>
 #include <SDL_ttf.h>

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>

 #include "menu.h"

 extern SDL_Window     *g_window;
 extern SDL_Renderer   *g_renderer;
 extern t_menu         *g_currentMenu;
 extern t_lobby        *g_lobby;
 extern short          g_currentOption;
 extern bool           g_serverRunning;


 bool   inMainMenu();
 void   makeSelection(unsigned short);
 void   handleMouseButtonUp(const SDL_Event *event);
 void   handleKeyDown(const SDL_Event *event);
 void   handleKeyUp(const SDL_Event *event);
 void   handleTextEditing(const SDL_Event *event);

#endif