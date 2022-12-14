#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#include "client.h"
#include "server.h"
#include "utils.h"
#include "config.h"
#include "dialog.h"
#include "player.h"
#include "game.h"
#include "tchat.h"
#include "effects.h"

#define DEBUG true

#ifdef _WIN32
HANDLE              g_clientThread = 0;
HANDLE              g_clientThreadUDP = 0;
#elif __unix__
pthread_t           g_clientThread = 0;
pthread_t           g_clientThreadUDP = 0;
#endif
int                 g_serverSocket = 0;
int                 g_serverSocketUDP = 0;
struct sockaddr_in  *g_serverAddrUDP = NULL;


t_serverConfig g_serverConfig;

void    broadcastMsg(const char *msg, int except) {
    char    buffer[1024];

    sprintf(buffer, "BROADCAST:%d %s", except, msg);
    sendMsg(buffer, g_serverSocket);
}

void    broadcastMsgUDP(const char *msg, short except) {
    char    buffer[1024];

    sprintf(buffer, "BROADCAST:%d %s", except, msg);
    sendMsgUDP(buffer, g_serverSocketUDP, g_serverAddrUDP);
}

/**
 * @brief Handle the message received from the server
 * if sockaddr is not null, it's a UDP message, else it's a TCP message
 * 
 * @param buffer 
 * @param server 
 * @param sockaddr 
 */
void    handleMessageClient(const char  *buffer, int server, const struct sockaddr_in  *sockaddr) {
    char        *pos;
    char        type[128];
    char        *content;

    pos = strchr(buffer, ':');
    if (pos == NULL) {
        #if DEBUG
            puts("Invalid message (: client)");
            exit(1);
        #endif
        return;
    }

    *pos = '\0';
    pos++;
    strcpy(type, buffer);

    content = pos;
    #if DEBUG
        printf("type: %s, msg: [%s]\n", type, content);
    #endif

    handleMessageClient2(type, content);
}

void    handleMessageClient2(char *type, char *content) {
    t_player    *player;
    t_game      *game;
    t_message   action;

    if (stringIsEqual(type, "JOIN")) {
        action = JOIN;
    } else if (stringIsEqual(type, "MOVE")) {
        action = MOVE;
    } else if (stringIsEqual(type, "CHAT")) {
        action = CHAT;
    } else if (stringIsEqual(type, "READY")) {
        action = READY;
    } else if (stringIsEqual(type, "START")) {
        action = START;
    } else if (stringIsEqual(type, "QUIT")) {
        action = QUIT;
    } else if (stringIsEqual(type, "PLAYERDAT")) {
        action = PLAYERDAT;
    } else if (stringIsEqual(type, "TCHAT")) {
        action = TCHAT;
    } else if (stringIsEqual(type, "CELL")) {
        action = CELL;
    } else if (stringIsEqual(type, "EFFECT")) {
        action = EFFECT;
    } else if (stringIsEqual(type, "DAMAGE")) {
        action = DAMAGE;
    } else if (stringIsEqual(type, "RESPAWN")) {
        action = RESPAWN;
    } else if (stringIsEqual(type, "LIFE")) {
        action = LIFE;
    } else if (stringIsEqual(type, "END")) {
        action = END;
    } else if (stringIsEqual(type, "NAME_TAKEN")) {
        action = NAME_TAKEN;
    } else {
        #if DEBUG
            puts("Invalid message type");
        #endif
    }

    game = getGame();
    switch (action)
    {
        case NAME_TAKEN:
            #if DEBUG
                fprintf(stderr, "The name is already taken");
            #endif
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Can't connect", "The name is already used", g_window);
            exit(0);
            break;
        case END:
            receiveEndGame(content);
            break;
        case CELL:
            cellUpdate(content);
            break;
        case LIFE:
            receiveLife(content);
            break;
        case EFFECT:
            receiveEffect(content);
            break;
        case MOVE:
            receiveMove(content);
            break;
        case RESPAWN: {
            short       id;
            int         xCell;
            int         yCell;

            sscanf(content, "%hd %d %d", &id, &xCell, &yCell);
            player = game->players[id];

            player->health = 100;    
        }
            break;
        case DAMAGE: {
            receiveDamage(content);
        }
            break;
        case START: {
            //launch game
            unsigned short  h;
            unsigned short  w;
            unsigned short  players;
            t_map           *map;
            const char      *ptr;
            sscanf(content, "%hu %hu %hu $", &h, &w, &players);
            printf("h: %hu, w: %hu, nb: %hu\n", h, w, players);

            if (!h || !w) {
                puts("Invalid map size");
                return;
            }

            ptr = strchr(content, '$');
            if (ptr == NULL) {
                #if DEBUG
                    puts("Invalid message ($ client)");
                    exit(1);
                #endif
            }
            ptr++;
            if (game->map != NULL) {
                map_destroy(game->map);
            }

            map = map_create(w, h);
            // note: players

            for (int i = 0; i < h; i++) {
                strncpy(map->map[i], ptr + w * i, w);
            }

            game->map = map;
            // amount of players
            game->map->players = players; //TODO

            game->nbPlayers = players;

            for (size_t i = 0; i < players; i++)
            {
                player = game->players[i];
                printf("(init) spawned player %s at %d, %d\n", player->name, player->xCell, player->yCell);
                spawnPlayer(player->xCell, player->yCell, player);
            }
            
            g_currentState = GAME_PLAY_PLAYING;
        }
            break;
        
        case PLAYERDAT: {
            // n, index in the players list is shared by the server
            short           n;
            int             xCell;
            int             yCell;
            char            name[256];
            sscanf(content, "%s %d %d %hu", name, &xCell, &yCell, &n);

            if (*name == 0) {
                puts("Name empty");
                exit(1);
            }

            printf("Params received: %s %d %d %hu\n", name, xCell, yCell, n);

            player = game->players[n];

            if (stringIsEqual(g_username, name)) {
                printf("set g_playersMultiIndex to %hu\n", n);
                g_playersMultiIndex = n;
                strcpy(game->players[n]->name, g_username);
            }

            if (player->id != n) {
                game->nbPlayers++;
            }

            strcpy(player->name, name);
            player->xCell = xCell;
            player->yCell = yCell;
            player->direction = DIR_IDLE;
            player->id = n;

            printf("player[%hu] %s at %d, %d\n", n, player->name, player->xCell, player->yCell);

            // if the player is the current player, spawn it
            printf("PLAYER name: %s\n", player->name);
        }
            break;

        case TCHAT:
            // we update the tchat to display the message for all players
            updateTchatMessages(content);
            break;
        default:
            break;
    }
}

void    askUsernameCallback() {
    //TODO: check if host is valid

    if (strlen(getEditBox()->edit) == 0) {
        #if DEBUG
            puts("No username entered");
        #endif
        return;
    }

    printf("put username [%s] in g_username\n", getEditBox()->edit);
    strcpy(getUsername(), getEditBox()->edit);

    strcpy(getGame()->players[0]->name, getUsername());
    printf("SET PLAYER 0 NAME TO %s\n", g_username);

    printf("g_username: %s\n", getUsername());
    destroyEditBox();
}

void    askServerHost() {
    t_dialog  *dialog;

    dialog = getEditBox();

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->text = "Enter server IP/host:";
    dialog->callback = askServerHostCallback;
}

void    askServerHostCallback() {
    //TODO: check if host is valid
    strcpy(g_serverConfig.host, getEditBox()->edit);
    askServerPort();
}

void    askServerPort() {
    t_dialog  *dialog;

    dialog = getEditBox();

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->text = "Enter server PORT:";
    dialog->callback = askServerPortCallback;
}

void    askServerPortCallback() {
    int     port;

    port = atoi(getEditBox()->edit);
    if (port == 0) {
        #if DEBUG
            puts("Invalid port");
        #endif
        getEditBox()->edit[0] = 0;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game crashed", "Invalid port", g_window);
        return;
    }

    if (port < 0 || port > 65535) {
        #if DEBUG
            puts("Invalid port");
        #endif
        getEditBox()->edit[0] = 0;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game crashed", "Invalid port", g_window);
        return;
    }

    strcpy(g_serverConfig.port, getEditBox()->edit);

    destroyEditBox();

    #ifdef _WIN32
    g_clientThread = CreateThread(NULL, 0, connectToServer, NULL, 0, NULL);
    g_clientThreadUDP = CreateThread(NULL, 0, connectToServerUDP, NULL, 0, NULL);
    #elif __unix__
    pthread_create(&g_clientThread, NULL, &connectToServer, "client");
    pthread_create(&g_clientThreadUDP, NULL, &connectToServerUDP, "client");
    #endif

    //Put client in the lobby
    g_currentState = GAME_MAINMENU_PLAY;
}




void    joinServer() {
    bool    valid = false;
    if (g_clientThread != 0) {
        #if DEBUG
            puts("Client already running");
        #endif
        return;
    }

    valid = checkUsername();
    if (!valid) return;

    createEditBox("", 20, colorGreen, colorWhite);
    SDL_StartTextInput();
    askServerHost();
}

/**
 * @brief Connect to the server
 * This function should be used in a separate thread
 * 
 */
void    *connectToServer(void *arg) {
    struct sockaddr_in  cl;
    char                buffer[1024];
    const char          *ptr;
    size_t              total;
    size_t              len;

    ptr = NULL;
    // thread does not need to be joined
    pthread_detach(pthread_self());

    printf("Connecting to server %s:%s\n", g_serverConfig.host, g_serverConfig.port);

    puts("Connecting to server...");
    g_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    cl.sin_family = AF_INET;

    if (isdigit(*g_serverConfig.host)) {
        cl.sin_addr.s_addr = inet_addr(g_serverConfig.host);
    } else {
        if (!hostToAddr(g_serverConfig.host, &cl.sin_addr.s_addr)) {
            #if DEBUG
                puts("Error converting host to address");
            #endif
            return NULL;
        }
    }

    cl.sin_port = htons((uint16_t) atoi(g_serverConfig.port));

    // setsockopt(g_serverSocket, IPPROTO_TCP, TCP_NODELAY, &(int){0}, sizeof(int));

    int res = connect(g_serverSocket, (struct sockaddr *)&cl, sizeof(cl));
    if (res < 0) {
        #if DEBUG
            perror("Error connecting to server");
            fprintf(stderr, "Error connecting to server: %s", strerror(res));
        #endif
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Can't connect", "Can't connect to server", g_window);
        g_clientThread = 0;
        return NULL;
    }

    puts("Connected to server");

    sprintf(buffer, "JOIN:%s", g_username);
    sendMsg(buffer, g_serverSocket);
    do
    {
        // receive message from client, wait if no message
        #if DEBUG
            puts("Waiting for message from server");
        #endif

        total = receiveMsg(buffer, g_serverSocket);
        // check if there is a single message in the buffer
        // if there is more than one message, we need to split the buffer
        // and process each message separately
        
        ptr = buffer;
        if (ptr == NULL) {
            printf("ptr is NULL, total: %lu\n", total);
        }
        len = 0;
        do
        {
            ptr = buffer + len;
            len += (strlen(ptr) + 1);
            // printf("ptr: char:%d %s\n", *ptr, ptr);
            // printf("len: %lu, total: %lu\n", len, total);

            handleMessageClient(ptr, g_serverSocket, NULL);

        } while (len != total);
        // printf("len == total, %lu  %lu\n", len, total);

        memset(buffer, 0, 1024);
    } while (true);
    

}


void    *connectToServerUDP(void *arg) {
    struct sockaddr_in  cl;
    unsigned            short  port;
    char                buffer[1024];
    char                *ptr = NULL;
    size_t              total;
    size_t              len;

    port = (unsigned short) atoi(g_serverConfig.port);

    // thread does not need to be joined
    pthread_detach(pthread_self());

    printf("(UDP) Connecting to server %s:%d\n", g_serverConfig.host, port);

    puts("(UDP) Connecting to server...");
    g_serverSocketUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    cl.sin_family = AF_INET;
    if (isdigit(*g_serverConfig.host)) {
        cl.sin_addr.s_addr = inet_addr(g_serverConfig.host);
    } else {
        if (!hostToAddr(g_serverConfig.host, &cl.sin_addr.s_addr)) {
            #if DEBUG
                puts("Error converting host to address");
            #endif
            return NULL;
        }
    }
    cl.sin_port = htons(port);

    g_serverAddrUDP = &cl;

    puts("(UDP) Connected to server");

    sprintf(buffer, "MYNAME:%s", g_username);
    sendMsgUDP(buffer, g_serverSocketUDP, &cl);
    memset(buffer, 0, 1024);

    do
    {
        struct sockaddr_in  sockaddr;

        // receive message from client, wait if no message
        #if DEBUG
            puts("Waiting for message from server");
        #endif

        total = receiveMsgUDP(buffer, g_serverSocketUDP, &sockaddr);
        // check if there is a single message in the buffer
        // if there is more than one message, we need to split the buffer
        // and process each message separately
        
        ptr = buffer;
        if (ptr == NULL) {
            printf("ptr is NULL, total: %lu\n", total);
        }
        len = 0;
        do
        {
            ptr = buffer + len;
            len += (strlen(ptr) + 1);

            printf("len: %lu, total: %lu\n\n", len, total);
            printf("ptr: %s\n", ptr);
            handleMessageClient(ptr, g_serverSocketUDP, &sockaddr);

        } while (len != total);

        memset(buffer, 0, 1024);
    } while (true);
}


void    updateCell(unsigned short xCell, unsigned short yCell, t_type type) {
    if (!inMultiplayer()) return;
    char    buffer[1024];

    sprintf(buffer, "CELL:%hu %hu %u", xCell, yCell, type);
    sendToAll(buffer, getPlayer()->id);
}

void    sendEffect(const t_effect *effect) {
    if (!inMultiplayer()) return;
    char    buffer[1024];
    
    sprintf(buffer, "EFFECT:%d %d %d", effect->xCell, effect->yCell, effect->type);
    sendToAll(buffer, getPlayer()->id);
}
