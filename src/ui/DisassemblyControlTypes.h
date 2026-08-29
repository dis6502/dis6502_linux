#pragma once

/*
** notifications found in HIWORD(lParam) in WM_COMMAND message
*/
typedef enum {
    DIS_SCROLL_CHANGED,                     // user has scrolled the window
    DIS_LBUTTONDOWN,                        // left button pressed
    DIS_RBUTTONDOWN,                        // right button pressed
    DIS_XREF                                // cross reference can be updated
} DIS_NOTIFICATION;