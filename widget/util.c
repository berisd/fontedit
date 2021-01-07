//
// Created by bernd on 07.01.21.
//
#include "util.h"

bool BRS_GUI_WIDGET_PointInRect(int32_t x, int32_t y, BRS_Point *position, BRS_Dimension *dimension) {
    bool intersect = (x >= position->x && x <= position->x + dimension->width &&
                      y >= position->y && y <= position->y + dimension->height);
    return intersect;
}
