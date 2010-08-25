/** @file
 * This file declares functions for hooking mouse messages.\n
 * Hooking mouse is used for comfortable moving borders in CRuler 
 */

#pragma once

#include <windows.h>
#include <tchar.h>

#ifndef _DEFINED_44E531B1_14D3_11d5_A025_006067718D04
#define _DEFINED_44E531B1_14D3_11d5_A025_006067718D04

#ifdef __cplusplus

extern "C" {

#endif // __cplusplus

#ifdef _COMPILING_44E531B1_14D3_11d5_A025_006067718D04
#define LIBSPEC __declspec(dllexport)
#else
#define LIBSPEC __declspec(dllimport)
#endif // _COMPILING_44E531B1_14D3_11d5_A025_006067718D04

/**
 * Function, which hooks @e WM_MOUSEMOVE and @e WM_LBUTTONUP messages.
 * @param hWnd window to forward hooked messages
 * @return true if success, false otherwise
 */
LIBSPEC bool setMouseHook( HWND hWnd );

/**
 * Unhook mouse function
 * @param hWnd window, where mouse was hooked
 * @return true if success, false otherwise
 * @see setMouseHook
 */
LIBSPEC bool clearMouseHook( HWND hWnd );

#undef LIBSPEC

#ifdef __cplusplus
}
#endif // __cplusplus

/**
 * Macro for hooked @e WM_MOUSEMOVE message
 */
#define UWM_MOUSEMOVE_MSG _T("UWM_MOUSEMOVE_MSG-44E531B1_14D3_11d5_A025_006067718D04")

/**
 * Macro for hooked @e WM_LBUTTONUP message
 */
#define UWM_LBUTTONUP_MSG _T("UWM_LBUTTONUP_MSG-44E531B1_14D3_11d5_A025_006067718D05")

#endif // _DEFINED_44E531B1_14D3_11d5_A025_006067718D04

