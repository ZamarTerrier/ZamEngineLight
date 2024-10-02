#include "GUI/e_widget_entry.h"

#include "Core/e_window.h"

#include "TigorEngine.h"
#include "TigorGUI.h"

#include "Tools/e_math.h"

#include "Data/e_resource_data.h"

extern TEngine engine;

bool e_ctrl_press = false, e_c_press = false, e_v_press = false, e_pasted = false, e_copied = false;

int EntryWidgetCharInput(EWidget* widget, uint32_t codepoint, void *arg){

    EWidgetEntry *temp = (EWidgetEntry *)widget;

    if(temp->currPos >= MAX_ENTERY_LENGTH - 1)
        return 1;

    if(codepoint > 256)
    {
        for(int i=0;i < 66; i++)
        {
            if(fontIndexes[i].FindLetter == codepoint)
            {
                uint16_t t = fontIndexes[i].IndexLetter & 0xFF;
                temp->text[temp->currPos + 1] = t;
                t = fontIndexes[i].IndexLetter >> 8;
                temp->text[temp->currPos] = t;
                temp->currPos+=2;
            }
        }
    }else{
        temp->text[temp->currPos] = codepoint;
        temp->currPos++;
        temp->text[temp->currPos] = '|';
    }

    return 0;
}

int EntryWidgetKeyPressInput(EWidget* widget, int key, void *arg){

    TWindow *window = (TWindow *)engine.window;

    EWidgetEntry *temp = (EWidgetEntry *)widget;

    if(key == TIGOR_KEY_BACKSPACE)
    {
        EntryWidgetMakeBackspace(temp);
    }

    
    if(temp->currPos >= MAX_ENTERY_LENGTH - 1)
        return 1;


    if(e_ctrl_press == true && e_v_press == true && !e_pasted)
    {
        const char *clipboard = TEngineGetClipBoardString();

        if(clipboard == NULL)
            return 1;

        uint32_t size = strlen(clipboard);

        char *point = &temp->text[temp->currPos];

        int diff = MAX_ENTERY_LENGTH - temp->currPos - size;

        if(diff > 0){
            memcpy(point, clipboard, size);
            
            point[size] = 0;
            temp->currPos += size;
        }else{
            diff = size + diff;

            if(diff > 0){
                memcpy(point, clipboard, diff);

                point[diff] = 0;
                temp->currPos += diff;
            }                
        }

        e_pasted = true;
    }

    if(e_ctrl_press == true && e_c_press && !e_copied){

        temp->text[temp->currPos] = 0;

        TEngineSetClipBoardString(temp->text);

        e_copied = true;
    }
    
    temp->text[temp->currPos] = '|';

    return 0;
}

void EntryWidgetMakeBackspace(EWidgetEntry *temp){
    temp->text[temp->currPos] = 0;

    temp->currPos--;

    if(temp->currPos < 0)
    {
        temp->currPos = 0;
    }

        
    if(e_ctrl_press){
        while(temp->text[temp->currPos] != ' ' && temp->currPos > 0){
            temp->text[temp->currPos] = 0;

            temp->currPos--;
        }
    }else if(temp->text[temp->currPos] < 0)
    {
        temp->text[temp->currPos] = 0;

        temp->currPos--;
    }
        
    temp->text[temp->currPos] = '|';
}

int EntryWidgetKeyRepeatInput(EWidget* widget, int key, void *arg){
    EWidgetEntry *temp = (EWidgetEntry *)widget;

    if(key == TIGOR_KEY_BACKSPACE)
    {
        EntryWidgetMakeBackspace(temp);
    }

    return 0;
}

void EntryWidgetKeyCallback(void* arg,  int key, int scancode, int action, int mods)
{
    if(engine.e_var_current_entry == NULL)
            return;

    if(key == TIGOR_KEY_LEFT_CONTROL && action == TIGOR_PRESS)
        e_ctrl_press = true;
    else if(key == TIGOR_KEY_LEFT_CONTROL && action == TIGOR_RELEASE){
        e_ctrl_press = false;

        if(e_v_press == false)
            e_pasted = false;

        if(e_c_press == false)
            e_copied = false;
    }

    if(key == TIGOR_KEY_V && action == TIGOR_PRESS)
        e_v_press = true;
    else if(key == TIGOR_KEY_V && action == TIGOR_RELEASE){
        e_v_press = false;
        e_pasted = false;
    }

    if(key == TIGOR_KEY_C && action == TIGOR_PRESS)
        e_c_press = true;
    else if(key == TIGOR_KEY_C && action == TIGOR_RELEASE){
        e_c_press = false;
        e_copied = false;
    }

    if(action == TIGOR_PRESS)
        WidgetConfirmTrigger(engine.e_var_current_entry, TIGOR_WIDGET_TRIGGER_ENTRY_KEY_PRESS_INPUT, key);
    else if(action == TIGOR_REPEAT)
        WidgetConfirmTrigger(engine.e_var_current_entry, TIGOR_WIDGET_TRIGGER_ENTRY_KEY_REPEAT_INPUT, key);
    else if(action == TIGOR_RELEASE)
        WidgetConfirmTrigger(engine.e_var_current_entry, TIGOR_WIDGET_TRIGGER_ENTRY_KEY_RELEASE_INPUT, key);
}

void EntryWidgetCharacterCallback(void* arg, uint32_t codepoint)
{
    if(engine.e_var_current_entry == NULL)
            return;

    WidgetConfirmTrigger(engine.e_var_current_entry, TIGOR_WIDGET_TRIGGER_ENTRY_CHAR_INPUT, codepoint);

}

int EntryWidgetPress(EWidget *widget, void *entry, void *arg){

    engine.e_var_current_entry = (EWidget *)widget;

    EWidgetEntry *temp = (EWidgetEntry *)widget;

    temp->text[temp->currPos] = '|';

    return 0;
}

int EntryWidgetUnfocus(EWidget *widget, void *entry, void *arg){

    EWidgetEntry *temp = (EWidgetEntry *)widget;

    temp->text[temp->currPos] = 0;

    return 0;
}

char *EntryWidgetGetText(EWidgetEntry *entry)
{
    return entry->text;
}

void EntryWidgetSetText(EWidgetEntry *entry, char *text)
{

    memset(entry->text, 0, MAX_ENTERY_LENGTH);

    int len = strlen(text);
    
    if(len > MAX_ENTERY_LENGTH)
    {
        memcpy(entry->text, text, MAX_ENTERY_LENGTH);
    }else{
        memcpy(entry->text, text, len);
    }

    entry->text[len] = 0;

    entry->currPos = len + 1;
}

void EntryWidgetCleartext(EWidgetEntry *entry)
{
    memset(entry->text, 0, MAX_ENTERY_LENGTH);
}

void EntryWidgetDraw(EWidgetEntry *entry){
            
    if(entry->widget.widget_flags & TIGOR_FLAG_WIDGET_VISIBLE){   

        vec2 pos = v2_add(entry->widget.position, entry->widget.base);

        GUIAddRectFilled(pos, v2_add(pos, entry->widget.scale), entry->widget.color, entry->widget.rounding, GUIDrawFlags_RoundCornersAll);
        
        int len = strlen(entry->text);
        int max_len = entry->widget.scale.x / engine.width * 8 * 10;
        if(len > max_len){
            char buff[max_len + 1];

            int t_pos = len - max_len;

            memcpy(buff, entry->text + t_pos, max_len);
            buff[max_len] = 0;

            GUIAddText(pos.x, pos.y + entry->widget.scale.y / 2, vec3_f(0,0,0), 7, buff);
        }
        else
            GUIAddText(pos.x, pos.y + entry->widget.scale.y / 2, vec3_f(0,0,0), 7, entry->text);
    }            
}


void EntryWidgetInit(EWidgetEntry *entry, vec2 scale, EWidget *parent){

    if(!GUIManagerIsInit())
        return;
        
    memset(entry, 0, sizeof(EWidgetEntry));

    WidgetInit((EWidget *)entry, parent);

    GameObjectSetDrawFunc((GameObject *)entry, (void *)EntryWidgetDraw);

    entry->widget.type = TIGOR_WIDGET_TYPE_BUTTON;
    entry->widget.rounding = 5.0f;    

    memset(entry->text, 0, MAX_ENTERY_LENGTH);

    WidgetSetColor((EWidget *)&entry->widget, vec3_f(1, 1, 1));
    WidgetSetScale((EWidget *)entry, scale.x, scale.y);

    WidgetConnect(entry, TIGOR_WIDGET_TRIGGER_MOUSE_PRESS, EntryWidgetPress, NULL);
    WidgetConnect(entry, TIGOR_WIDGET_TRIGGER_WIDGET_UNFOCUS, EntryWidgetUnfocus, NULL);

    WidgetConnect(entry, TIGOR_WIDGET_TRIGGER_ENTRY_CHAR_INPUT, EntryWidgetCharInput, NULL);
    WidgetConnect(entry, TIGOR_WIDGET_TRIGGER_ENTRY_KEY_PRESS_INPUT, EntryWidgetKeyPressInput, NULL);
    WidgetConnect(entry, TIGOR_WIDGET_TRIGGER_ENTRY_KEY_REPEAT_INPUT, EntryWidgetKeyRepeatInput, NULL);

}