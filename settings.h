#ifndef SETTINGS_H
 #define SETTINGS_H
 
 #include "colors.h"
 #include "loop.h"

 extern t_menu                  *g_currentMenu;
 extern SDL_Window*             g_window;
 extern SDL_Renderer*           g_renderer;

 typedef enum    e_settings {
     SETTING_VIDEO_FULLSCREEN,
     SETTING_VIDEO_WIDTH,
     SETTING_VIDEO_HEIGHT,
     SETTING_VIDEO_VSYNC,
     SETTING_AUDIO_GLOBAL_VOL,
     SETTING_AUDIO_MUSICS_VOL,
     SETTING_AUDIO_SOUNDS_VOL,
     SETTING_CONTROLS_UP,
     SETTING_CONTROLS_DOWN,
     SETTING_CONTROLS_LEFT,
     SETTING_CONTROLS_RIGHT,
     SETTING_CONTROLS_TCHAT,
     SETTING_CONTROLS_USE_ITEM,
     SETTING_CONTROLS_ITEM_1,
     SETTING_CONTROLS_ITEM_2,
     SETTING_CONTROLS_ITEM_3,
     SETTING_CONTROLS_ITEM_4,
     SETTING_CONTROLS_ITEM_5,
 }               t_settings;

 typedef enum    e_keys
 {
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_TCHAT,
        KEY_USE_ITEM,
        KEY_ITEM_1,
        KEY_ITEM_2,
        KEY_ITEM_3,
        KEY_ITEM_4,
        KEY_ITEM_5,
 }               t_keys;
 
 t_command    getActionFor(SDL_KeyCode key);
 bool         isKeyAvailable(char key);
 void         editFullscreen();
 void         editWidth();
 void         editHeight();
 void         editVSync();

 void         editGlobalVol();
 void         editMusicsVol();
 void         editSoundsVol();

 void         editUp();
 void         editDown();
 void         editLeft();
 void         editRight();
 void         editTchat();
 void         editUseItem();
 void         editItem1();
 void         editItem2();
 void         editItem3();
 void         editItem4();
 void         editItem5();
 void         editItem6();
 void         editItem7();
 void         editItem8();
 void         editItem9();

 void         editSettingCallback();

#endif