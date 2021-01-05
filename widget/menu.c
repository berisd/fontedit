//
// Created by bernd on 05.01.21.
//

#include "menu.h"

BRS_LIST_DEFN(BRS_GUI_MenuList, BRS_GUI_Menu)

void BRS_GUI_Menu_render(BRS_VideoContext *context, BRS_GUI_Menu *menu, BRS_Point position) {
    BRS_setColor(context, menu->backColor);
    SDL_Rect r = {.x = position.x, .y = position.y, .w=50, .h=18};
    SDL_RenderFillRect(context->renderer, &r);

    BRS_drawString(context, menu->label, menu->font, &position,
                   menu->selected ? menu->selectedForeColor : menu->foreColor);
    if (menu->selected) {
        BRS_GUI_MenuItemListEntry *entry = menu->itemList->firstEntry;
        while (entry != NULL) {
            position.y += 18;
            BRS_GUI_MenuItem_render(context, entry->value, position);
            entry = entry->next;
        }
    }
}

BRS_GUI_Menu *BRS_GUI_Menu_create(const char *label, const BRS_Color *foreColor, const BRS_Color *backColor,
                                  const BRS_Color *selectedForeColor, BRS_Font *font, bool selected) {
    BRS_GUI_Menu *menu = malloc(sizeof(BRS_GUI_Menu));
    menu->label = label;
    menu->font = font;
    menu->itemList = BRS_GUI_MenuItemList_create();
    menu->foreColor = foreColor;
    menu->backColor = backColor;
    menu->selectedForeColor = selectedForeColor;
    menu->selected = selected;
    menu->itemList = BRS_GUI_MenuItemList_create();
    return menu;
}