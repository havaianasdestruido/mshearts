/***************************************************************************/
/**                  Microsoft Windows                                    **/
/**            Copyright(c) Microsoft Corp., 1991, 1992                   **/
/***************************************************************************/

/****************************************************************************\

globals.cpp

Definitions of the global game state declared extern in main.h.

The original main.cpp/player.cpp from the Windows NT 5.1 mshearts drop was
not part of the repository import (issue #9), so this file provides stub
definitions.  They link-check the declarations and give the globals safe
initial values; the real implementations must restore the original sources.

****************************************************************************/

#include "hearts.h"

#include "main.h"

// The main window (gamemeister when playing locally).
CMainWindow *pMainWnd = NULL;

// Player objects: p[0] is the local human, p[1..3] are computer/remote players.
player *p[MAXPLAYER] = { NULL };

// The card deck.  card's default constructor gives each card id == EMPTY, so
// no cards.dll initialization is triggered by static initialization.
card cd[52];

// Current hand/trick state.
handinfotype handinfo = { 0 };

// Last move sent or received over DDE.
MOVE move = { 0 };

// Miscellaneous game state flags.
int     nStatusHeight = 0;
BOOL    bCheating = FALSE;
BOOL    bAutostarted = FALSE;
BOOL    bTimerOn = FALSE;
int     passdir = 0;

// DDE string handles for the shared topics/items.
HSZ hszStatus = NULL;
HSZ hszMove = NULL;
HSZ hszPass = NULL;
HSZ hszJoin = NULL;
HSZ hszGameNumber = NULL;
HSZ hszPassUpdate = NULL;

// NetDDE share name.
TCHAR szShareName[MAXCOMPUTERNAME + 1] = TEXT("");

// DDE object used for status broadcasts.
DDE *dde = NULL;

// Queued passes and moves (used when the gamemeister is busy).
PASS3 passq[16] = { { 0 } };
MOVE moveq[16] = { { 0 } };
int cQdPasses = 0;
int cQdMoves = 0;

// Registry value names (RegEntry is a stub; see main.h).
const TCHAR *szRegPath = TEXT("Software\\Microsoft\\Hearts");
const TCHAR *regvalName = TEXT("Name");
const TCHAR *regvalPName[3] = { TEXT("P1Name"), TEXT("P2Name"), TEXT("P3Name") };
const TCHAR *regvalSpeed = TEXT("Speed");
const TCHAR *regvalServer = TEXT("Server");
const TCHAR *regvalRole = TEXT("Role");
