#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "server.h"
#include "game.h"

#define DEBUG true

char                g_username[256] = {0};
short               g_playersMultiIndex = 0;


char            *getUsername() {
    const t_game      *game;

    game = getGame();

    if (g_clientThread) {
        return game->players[g_playersMultiIndex]->name;
    } else {
        return g_username;
    }
}

t_player        *initPlayer() {
    t_player    *player;

    player = malloc(sizeof(t_player));
    if (player == NULL) {
        #ifdef DEBUG
            fprintf(stderr, "Error allocating memory for player");
        #endif
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game crashed", "Error while creating player", g_window);
        exit(1);
    }
    *player->name = '\0';
    player->score = 0;
    player->x = 0;
    player->y = 0;
    player->vx = 0;
    player->vy = 0;
    player->xCell = 0;
    player->yCell = 0;
    player->health = 100;
    player->scope = 2;
    player->isBot = false;

    initInventory(player);
    return player;
}

t_player        *getPlayer() {
    const t_game      *game;

    game = getGame();

    // if player is a client, player != [0]
    if (g_clientThread) {
        return game->players[g_playersMultiIndex];
    } else {
        return game->players[0];
    }
}

t_direction     getDirection(const t_player *player) {
    if (player->vx == 0 && player->vy == 0) {
        return DIR_IDLE;
    } else if (player->vx == 0 && player->vy < 0) {
        return DIR_UP;
    } else if (player->vx == 0 && player->vy > 0) {
        return DIR_DOWN;
    } else if (player->vx < 0 && player->vy == 0) {
        return DIR_LEFT;
    } else if (player->vx > 0 && player->vy == 0) {
        return DIR_RIGHT;
    } else if (player->vx < 0 && player->vy < 0) {
        return DIR_UP_LEFT;
    } else if (player->vx > 0 && player->vy < 0) {
        return DIR_UP_RIGHT;
    } else if (player->vx < 0 && player->vy > 0) {
        return DIR_DOWN_LEFT;
    } else if (player->vx > 0 && player->vy > 0) {
        return DIR_DOWN_RIGHT;
    }
    return DIR_IDLE;
}

bool    inMultiplayer() {
    return (g_clientThread || g_serverRunning);
}

bool    isMoving(const t_player *player) {
    return (player->vx != 0 || player->vy != 0);
}

void    sendPos() {
    const t_player    *player;

    player = getPlayer();
    if (inMultiplayer() && isMoving(player)) {
        doSendPos(player);
    }
}

void    doSendPos(const t_player *player) {
    char    buffer[256] = {0};
    // update the grid position for other players
    sprintf(buffer, "MOVE:%d %d %u %hu%c", player->x, player->y, player->direction, g_playersMultiIndex, '\0');
    sendToAllUDP(buffer, NULL);
}

void        printInventory() {
    t_player    *player;
    player = getPlayer();
    /*
    for(int k = 0; k < NB_ITEMS; k++) {
        if (player->inventory[k] != NULL) {
            printf("Item %d: %d", k, player->inventory[k]->quantity);
        }
    }
     */
}

void       initInventory(t_player *player) {
    // TODO : init inventory, example : player->inventory[ITEM_BOMB] = getItem(ITEM_BOMB);
   player->inventory[0] = getItem(ITEM_BOMB);

        /*
        for (int k = ITEM_BOMB; k < NB_ITEMS; k++) {
            //malloc(sizeof(t_item));
            printf("p");
            //player->inventory[k] = malloc(sizeof(t_item));
            //player->inventory[k] = getItem(k);
        }
         */

}

/**
 * @brief      Player has the item in his inventory
 * @param item
 * @return
 */
bool        hasItemInInventory(t_player *player, t_item *item) {
    if (player->inventory[item->type] != NULL && player->inventory[item->type]->quantity > 0) {
        return true;
    }
    return false;
}

/**
 * @brief      Verify if the player is alive
 * @param item
 * @return
 */
bool        isAlive(t_player *player) {
    return (player->health > 0);
}