/***************************************************************************/
/**                  Microsoft Windows                                    **/
/**            Copyright(c) Microsoft Corp., 1991, 1992                   **/
/***************************************************************************/

/****************************************************************************\

main.h

Main header for Microsoft Hearts Network: declares CMainWindow and the
global game state shared by all modules.

The original main.h/main.cpp from the Windows NT 5.1 mshearts drop was not
part of the repository import (issue #9).  This stub declares the CMainWindow
members and globals referenced by the rest of the tree so that all #include
directives resolve and the code is coherent.  The real implementations live
in the missing original sources; a faithful build must restore them.

****************************************************************************/

#ifndef MAIN_INC
#define MAIN_INC

#include "dlg.h"      // MAXPLAYER
#include "card.h"     // card, EMPTY
#include "player.h"   // player, remote_human, handinfotype, MOVE, PASS3, PASS12, GAMESTATUS, SLOT, MAXSLOT
#include "resource.h" // IDS_*, IDC_*, DLG_*, IDM_*, MAXNAMELENGTH, MAXCOMPUTERNAME
#include "dde.h"      // HSZ, HDDEDATA, DDE, DDEServer, DDEClient, ddeClient, ddeServer

// --------------------------------------------------------------------------
// CMainWindow -- the game's main window / gamemeister.
//
// Only members referenced elsewhere are declared; the original class has
// more (message map, dialog handling, painting, ...).
// --------------------------------------------------------------------------

class CMainWindow : public CFrameWnd
{
public:
    // DDEML callbacks (Win32 signature; issue #6)
    HDDEDATA DdeSrvCallBack(UINT wType, UINT wFmt, HCONV hConv,
                            HSZ hsz1, HSZ hsz2, HDDEDATA hData,
                            ULONG_PTR dwData1, ULONG_PTR dwData2);
    HDDEDATA DdeCliCallBack(UINT wType, UINT wFmt, HCONV hConv,
                            HSZ hsz1, HSZ hsz2, HDDEDATA hData,
                            ULONG_PTR dwData1, ULONG_PTR dwData2);

    // DDE item handlers
    HDDEDATA GetGameStatus(HCONV hConv);
    int      GetGameNumber();
    int      GetPlayerMode(int nPos);
    BOOL     IsFirstBloodEnforced();
    CString  GetPlayerName(int nPos);
    VOID     FatalError(UINT nStringID);
    VOID     OnNewGame();

    // Server helpers
    int      AddNewPlayer(HCONV hConv, HDDEDATA hData);
    VOID     GetPass12(PASS12 &pass12);
    VOID     ReceivePassFromClient(HDDEDATA hData);
    VOID     ReceiveMove(MOVE &rMove);
    VOID     HandleMove(MOVE &rMove);
    VOID     HandlePass(PASS3 &pass3);
    VOID     HandlePassing();
    VOID     PlayerQuit(int nId);
    int      CountClients();
    BOOL     IsCurrentPlayer(HCONV hConv, int *pnId);
    VOID     UpdateStatusNames(GAMESTATUS &gs);
    VOID     UpdatePassStatus(PASS12 &pass12);
};

// --------------------------------------------------------------------------
// Global state (definitions in globals.cpp)
// --------------------------------------------------------------------------

extern CMainWindow *pMainWnd;      // the main window (gamemeister when local)

extern card  cd[];                 // the card deck (52 cards)
extern player *p[MAXPLAYER];       // player objects; p[0] is the local human
extern handinfotype handinfo;      // current hand/trick state
extern MOVE   move;                // last move sent or received over DDE

extern int     nStatusHeight;      // height of the status bar
extern BOOL    bCheating;          // gamemeister cheating flag
extern BOOL    bAutostarted;       // current game was autostarted
extern BOOL    bTimerOn;           // move timer is active
extern int     passdir;            // current pass direction

extern HSZ hszStatus;           // game status (player names/ids)
extern HSZ hszMove;             // card plays
extern HSZ hszPass;             // pass selections
extern HSZ hszJoin;             // new player joining
extern HSZ hszGameNumber;       // game number
extern HSZ hszPassUpdate;       // pass-status update requests

extern TCHAR szShareName[MAXCOMPUTERNAME + 1];  // NetDDE share name

extern DDE *dde;                // DDE object used for status broadcasts

extern PASS3 passq[];           // queued passes
extern MOVE moveq[];            // queued moves
extern int cQdPasses;
extern int cQdMoves;

// --------------------------------------------------------------------------
// RegEntry -- registry access stub.
//
// The original RegEntry class (from the missing original sources) wraps the
// Win32 registry APIs.  This minimal stub keeps the tree compilable in
// structure; method bodies return safe defaults and are no-ops.  Replace
// with real registry implementations for a working build (issue #9).
// --------------------------------------------------------------------------

class RegEntry
{
public:
    RegEntry(LPCTSTR pszPath) { UNREFERENCED_PARAMETER(pszPath); }
    BOOL GetString(LPCTSTR pszValue, LPTSTR pszBuffer, DWORD nSize)
    {
        UNREFERENCED_PARAMETER(pszValue);
        if (pszBuffer != NULL && nSize > 0)
            *pszBuffer = _T('\0');
        return FALSE;
    }
    DWORD GetNumber(LPCTSTR pszValue, DWORD dwDefault)
    {
        UNREFERENCED_PARAMETER(pszValue);
        return dwDefault;
    }
    BOOL SetValue(LPCTSTR pszValue, DWORD dwValue)
    {
        UNREFERENCED_PARAMETER(pszValue);
        UNREFERENCED_PARAMETER(dwValue);
        return TRUE;
    }
    BOOL SetValue(LPCTSTR pszValue, LPCTSTR pszString)
    {
        UNREFERENCED_PARAMETER(pszValue);
        UNREFERENCED_PARAMETER(pszString);
        return TRUE;
    }
    BOOL DeleteValue(LPCTSTR pszValue)
    {
        UNREFERENCED_PARAMETER(pszValue);
        return TRUE;
    }
};

extern const TCHAR *szRegPath;
extern const TCHAR *regvalName;
extern const TCHAR *regvalPName[3];
extern const TCHAR *regvalSpeed;
extern const TCHAR *regvalServer;
extern const TCHAR *regvalRole;

#endif  // MAIN_INC
