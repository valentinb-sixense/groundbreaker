#include "config.h"
#include "loop.h"
#include "game.h"
#include "map.h"
#include "moves.h"
#include "utils.h"

bool    inGame() {
    return (g_currentState >= GAME_PLAY_PLAYING);
}

t_game* getGame() {
    static t_game* game;

    if (game == NULL) {
        game = malloc(sizeof(t_game));
        if (game == NULL) {
            #ifdef DEBUG
                fprintf(stderr, "Error allocating memory for game");
            #endif
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game crashed", SDL_GetError(), g_window);
            exit(1);
        }
        game->x = 0;
        game->y = 0;

        game->vx = 0;
        game->vy = 0;

        game->xCell = 0;
        game->yCell = 0;
    }
    return game;
}

void    posToGrid() {
    unsigned int   cellSizeX;
    unsigned int   cellSizeY;
    t_game         *game;

    game = getGame();

    // can't divide by 0
    if (game->x) {
        cellSizeX = gameConfig->video.width / game->map->width; // ex: 166 (width of 1000 divided by 6 (nb of cols))
        // avoid xCell to be equal to map->width (segfault, col 6 doesn't exist for a map of 6 cols)
        if (game->x != gameConfig->video.width) {
            game->xCell = (game->x / cellSizeX);
        }
    }

    if (game->y) {
        cellSizeY = gameConfig->video.height / game->map->height;
        if (game->y != gameConfig->video.height) {
            game->yCell = (game->y / cellSizeY);
        }
    }

    // find the nearest multiple of cellSize(s)
    // ex: 768 / 166 = 4.6 => 4
}


void    movePlayer() {
    t_game *game;
    const t_map  *map;
    short        oldCellX;
    short        oldCellY;

    game = getGame();
    map = game->map;

    // save the position on the grid before moving
    oldCellX = game->xCell;
    oldCellY = game->yCell;

    if (map == NULL) {
        #ifdef DEBUG
            fprintf(stderr, "Error: map is NULL in movePlayer()\n");
        #endif
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game crashed", SDL_GetError(), g_window);
        exit(1);
    }

    checkBorders();

    // make the move
    game->x += game->vx;
    game->y += game->vy;

    // clamp the player position to the map
    if (game->x < 0) {
        game->x = 0;
    }
    if (game->y < 0) {
        game->y = 0;
    }

    if (game->x > gameConfig->video.width) {
        game->x = gameConfig->video.width;
    }
    if (game->y > gameConfig->video.height) {
        game->y = gameConfig->video.height;
    }
    posToGrid();

    // printf("x = %d, y = %d , vx = %d, vy = %d, cellPos: (cx:%hu cy:%hu)\n", game->x, game->y, game->vx, game->vy, game->xCell, game->yCell);

    // if (game->x == game->lastX && game->y == game->lastY) return;

    // the old position is now empty
    // map->map[game->lastY][game->lastX] = EMPTY;

    // if player want to go out of the map then we move him at the other side

    switch (map->map[game->yCell][game->xCell]) {
        // case EMPTY:
        //     map->map[game->yCell][game->xCell] = PLAYER;
        //     break;
        case WALL:
            // if the player is on a wall then we move him back to the old position
            game->x -= game->vx;
            game->y -= game->vy;
            break;
        case UNBREAKABLE_WALL:
            // if the player is on a wall then we move him back to the old position
            game->x -= game->vx;
            game->y -= game->vy;
            break;
        // case BOMB:
        //     map->map[game->yCell][game->xCell] = PLAYER;
        //     // TODO : player is on a bomb so the player must die
        //     break;
        // case ITEM:
        //     map->map[game->yCell][game->xCell] = PLAYER;
        //     // TODO : Remove the item from the map and add it to the inventory
        //     break;
        default:
            break;
    }
    posToGrid();
}


