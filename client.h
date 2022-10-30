#ifndef CLIENT_H
 #define CLIENT_H

 #include "game.h"
 extern t_gameConfig    *gameConfig;

 void    askUsernameCallback();
 void    askServerHost();
 void    askServerHostCallback();
 void    askServerPort();
 void    askServerPortCallback();
 void    joinServer();
 void    *connectToServer();

#endif