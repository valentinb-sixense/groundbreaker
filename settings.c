#include <stdlib.h>
#include <stdio.h>

#include "menu.h"
#include "settings.h"
#include "dialog.h"
#include "config.h"

#define DEBUG true

void    editSettingCallback() {
    const t_dialog  *dialog;
    const char      *value;
    char            buffer[SIZE_DATA];
    unsigned int    vol;

    dialog = getEditBox();
    value = dialog->edit;

    if (!strlen(value)) {
        #ifdef DEBUG
            fprintf(stderr, "Error: empty value");
        #endif
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Empty value", g_window);
        return;
    }

    switch (dialog->arg)
    {
        case SETTING_VIDEO_FULLSCREEN:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }
            if (*value != '0' && *value != '1' && *value != '2') {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            int mode;
            switch (*value)
            {
                case 1:
                    mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
                    break;
                case 2:
                    mode = SDL_WINDOW_FULLSCREEN;
                    break;
                
                default:
                    mode = 0; // windowed
                    break;
            }

            gameConfig->video.fullscreen = mode;
            SDL_SetWindowFullscreen(g_window, gameConfig->video.fullscreen);

            saveSetting("fullscreen", value);
            break;
        case SETTING_VIDEO_WIDTH:
            if (strspn(value, "0123456789") != strlen(value)) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: value must be a number");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            gameConfig->video.width = atoi(value);
            SDL_SetWindowSize(g_window, atoi(value), gameConfig->video.height);

            saveSetting("width", value);
            break;
        case SETTING_VIDEO_HEIGHT:
            if (strspn(value, "0123456789") != strlen(value)) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: value must be a number");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            gameConfig->video.height = atoi(value);
            SDL_SetWindowSize(g_window, gameConfig->video.width, atoi(value));

            saveSetting("height", value);
            break;
        case SETTING_VIDEO_VSYNC:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }
            if (value[0] != '0' && value[0] != '1') {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            saveSetting("vsync", value);
            break;
        case SETTING_AUDIO_GLOBAL_VOL:
            vol = atoi(value);
            if (vol > 100) {
                vol = 100;
            }
            if (vol < 100) {
                vol = 0;
            }
            sprintf(buffer, "%d", vol);

            saveSetting("volume", buffer);

            gameConfig->audio.volume = vol;
            break;
        case SETTING_AUDIO_MUSICS_VOL:
            vol = atoi(value);
            if (vol > 100) {
                vol = 100;
            }
            if (vol < 100) {
                vol = 0;
            }
            sprintf(buffer, "%d", vol);

            saveSetting("music_vol", buffer);
            gameConfig->audio.music_volume = vol;
            break;
        case SETTING_AUDIO_SOUNDS_VOL:
            vol = atoi(value);
            if (vol > 100) {
                vol = 100;
            }
            if (vol < 100) {
                vol = 0;
            }
            sprintf(buffer, "%d", vol);

            saveSetting("sound_vol", buffer);
            gameConfig->audio.sound_volume = vol;
            break;
        case SETTING_CONTROLS_UP:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("up", buffer);
            break;
        case SETTING_CONTROLS_DOWN:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("down", buffer);
            break;
        case SETTING_CONTROLS_LEFT:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("left", buffer);
            break;
        case SETTING_CONTROLS_RIGHT:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("right", buffer);
            break;
        case SETTING_CONTROLS_USE_ITEM:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("use_item", buffer);
            break;
        case SETTING_CONTROLS_ITEM_1:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("item_1", buffer);
            break;
        case SETTING_CONTROLS_ITEM_2:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("item_2", buffer);
            break;
        case SETTING_CONTROLS_ITEM_3:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("item_3", buffer);
            break;
        case SETTING_CONTROLS_ITEM_4:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("item_4", buffer);
            break;
        case SETTING_CONTROLS_ITEM_5:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("item_5", buffer);
            break;
        case SETTING_CONTROLS_ITEM_6:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("item_6", buffer);
            break;
        case SETTING_CONTROLS_ITEM_7:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("item_7", buffer);
            break;
        case SETTING_CONTROLS_ITEM_8:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }

            sprintf(buffer, "%c", *value);
            saveSetting("item_8", buffer);
            break;
        case SETTING_CONTROLS_ITEM_9:
            if (strlen(value) != 1) {
                #ifdef DEBUG
                    fprintf(stderr, "Error: invalid value\n");
                #endif
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "invalid value", g_window);
                return;
            }
            sprintf(buffer, "%c", *value);
            saveSetting("item_9", buffer);
            break;
        
        default:
            break;
    }

    destroyEditBox();
}

void    editFullscreen() {
    t_dialog  *dialog;

    dialog = createEditBox("Fullscreen (0: windowed, 1 fullscreen desktop, 2 fullscreen)", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_VIDEO_FULLSCREEN;
}
void    editWidth() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter width:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_VIDEO_WIDTH;
}
void    editHeight() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter height:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_VIDEO_HEIGHT;
}
void    editVSync() {
    t_dialog  *dialog;

    dialog = createEditBox("Choose VSync (0: off, 1: on):", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_VIDEO_VSYNC;
}


void    editGlobalVol() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter global volume:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_AUDIO_GLOBAL_VOL;
}
void    editMusicsVol() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter musics volume:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_AUDIO_MUSICS_VOL;
}
void    editSoundsVol() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter sounds volume:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_AUDIO_SOUNDS_VOL;
}

void    editUp() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter UP key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_UP;
}
void    editDown() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter DOWN key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_DOWN;
}
void    editLeft() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter LEFT key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_LEFT;
}
void    editRight() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter RIGHT key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_RIGHT;
}

void    editUseItem() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter USE ITEM key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_USE_ITEM;
}

void    editItem1() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter item 1 key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_ITEM_1;
}
void    editItem2() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter item 2 key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_ITEM_2;
}
void    editItem3() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter item 3 key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_ITEM_3;
}
void    editItem4() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter item 4 key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_ITEM_4;
}
void    editItem5() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter item 5 key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_ITEM_5;
}
void    editItem6() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter item 6 key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_ITEM_6;
}
void    editItem7() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter item 7 key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_ITEM_7;
}
void    editItem8() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter item 8 key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_ITEM_8;
}
void    editItem9() {
    t_dialog  *dialog;

    dialog = createEditBox("Enter item 9 key:", 20, colorWhite, colorBlack);

    memset(dialog->edit, 0, sizeof(dialog->edit));
    dialog->callback = editSettingCallback;
    dialog->arg = SETTING_CONTROLS_ITEM_9;
}
