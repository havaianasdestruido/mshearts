/***************************************************************************/
/**                  Microsoft Windows                                    **/
/**            Copyright(c) Microsoft Corp., 1991, 1992                   **/
/***************************************************************************/

/****************************************************************************

hearts.cpp

Main file for Microsoft Hearts Network.

Aug 92, JimH
May 93, JimH    chico port

****************************************************************************/

#include "hearts.h"
#include "debug.h"

// The obsolete _setargv()/_setenvp() CRT hooks were removed (issue #10):
// they disabled command-line/environment parsing and are absent from modern
// SDKs, so the CRT's own startup code must handle argv/envp.

TCHAR   suitid[] = TEXT("CDHS");
TCHAR   cardid[] = TEXT("JQKA");
