#include "tchat.h"
#include "game.h"
#include "utils.h"
#include "font.h"
#include "dialog.h"
#include "settings.h"
#include "server.h"
#include "client.h"

#define DEBUG true

// Declare global variables
char   **g_messages = NULL;
size_t  g_messages_nb = 0;

// TODO : player in green and other in white
/**
 * Draw the tchat
 * @return {void}
*/
void    drawTchat() {
    SDL_Rect rect;
    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
    SDL_Color color = {0, 0, 0, 128};

    pickColor(&color);

    rect.x = 0;
    rect.y = 0;
    rect.w = (gameConfig->video.width) * 0.25;
    rect.h = gameConfig->video.height * 0.4;
    SDL_RenderFillRect(g_renderer, &rect);
}

/**
 * Add a message to the tchat
 * @return {void}
 */
void    askTchatMessage() {
    t_dialog  *dialog;

    dialog = createEditBox("Send a message :", 20, (SDL_Color){255, 255, 255, 255}, (SDL_Color){0, 0, 0, 255});
    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = askTchatMessageCallback;
}

/**
 * Callback for askTchatMessage
 * @param  {t_dialog *} dialog
 * @return {void}
 */
void    askTchatMessageCallback(const char *msg) {
    t_player *player;
    player = getPlayer();

    if (msg) {
        putMessageInTchat(player, msg);
    }
    destroyEditBox();
}

/**
 * Add a message to the tchat
 * @param {t_player} player
 * @param {char *} message
 * @return {void}
*/
void    putMessageInTchat(t_player *player, const char *msg) {
    if(!inMultiplayer()) { exit (0); }
    char    *buffer;
    buffer = malloc(sizeof(char) * (strlen(msg) + 1 + strlen(player->name) + 1));
    if(buffer == NULL) {
        #ifdef DEBUG
            fprintf(stderr, "Error allocating memory for buffer");
        #endif
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game crashed", SDL_GetError(), g_window);
        exit(1);
    }
    sprintf(buffer, "TCHAT:%s: %s%c", player->name, msg, '\0');
    sendToAll(buffer, -1);
}

void    doSendTchatMessage(const char *msg) {


}

/**
 * @brief  Draw a messages in the tchat
 *
 * @param  msg  The message to send
 */
void    drawTchatMessages() {
    if(!inMultiplayer()) { exit (0); }
    int i, limit, op, textWidth, textHeight;
    SDL_Rect rect;
    SDL_Color color = {255, 255, 255, 255};
    SDL_Texture *tex;


    if(g_messages_nb == 0) return;
    rect.x = 0;
    rect.y = 0;
    rect.w = (gameConfig->video.width) * 0.25;
    i = g_messages_nb;
    limit = g_messages_nb - 5;

    // we start at the end of the array to display the 5 last messages
    while((i &&  g_messages[i - 1] != NULL && i > limit)) {
        drawText(&color, rect.x + (gameConfig->video.width * 0.03), rect.y + (gameConfig->video.height * 0.03), g_messages[i-1], false, rect.w);


        //TODO : fix margin when the text is too long
        // getTextureFromString contains 3 parameters : the text, the color and the width of the text
        tex = getTextureFromString(g_messages[i-1], &color, &textWidth);
        op = SDL_QueryTexture(tex, NULL, NULL, &textWidth, &textHeight);
        if(textHeight < 15) {
            rect.y = textHeight;
        }else if(textHeight > 15 && textHeight < 20) {
            rect.y += 25;
        }else if(textHeight > 20 && textHeight < 30) {
            rect.y += 35;
        }else {
            rect.y += (gameConfig->video.height * 0.03) + 15;
        }
        SDL_DestroyTexture(tex);

        i--;    
    }
}