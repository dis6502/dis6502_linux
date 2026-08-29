#pragma once


/*
** WM_DUMP_SET_BEGIN_SELECTION, WM_DUMP_SET_END_SELECTION, WM_DUMP_GET_BEGIN_SELECTION and
** WM_DUMP_GET_END_SELECTION messages can use this define for no selection.
*/
#define   DUMP_NO_SELECTION        0xFFFF

/*
** notifications found in HIWORD(lParam) in WM_COMMAND message
*/
enum class MemoryInspectorNotification {
	SCROLL_CHANGED,                    // user has scrolled the window
	SELECTION_CHANGED,                 // user has changed the selection
	RBUTTONDOWN,                       // user has right clicked in the window
	LBUTTONDBLCLK,                     // user has double clicked in the window
	QUIT_EDIT_MODE                     // user has quit the edit mode
};
