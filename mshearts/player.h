/***************************************************************************/
/**                  Microsoft Windows                                    **/
/**            Copyright(c) Microsoft Corp., 1991, 1992                   **/
/***************************************************************************/

/****************************************************************************\

player.h

This file contains the class declarations and manifest constants necessary
for implementing the 'player' object and the shared game structures.

The original player.h/player.cpp from the Windows NT 5.1 mshearts drop was
not part of the repository import (issue #9).  This stub declares the
types referenced by the rest of the tree so that all #include directives
resolve.  It is deliberately minimal; a real build should restore the
original sources.

****************************************************************************/

#ifndef PLAYER_INC
#define PLAYER_INC

#include "hearts.h"
#include "card.h"

// Number of cards in a hand, i.e. the number of card slots per player.
const int MAXSLOT = 13;

// A SLOT indexes a card within one player's hand (0 .. MAXSLOT-1).
typedef int SLOT;

// --------------------------------------------------------------------------
// handinfotype -- snapshot of the current trick/hand, passed to computer
// players so they can decide what to play.
// --------------------------------------------------------------------------
struct handinfotype
{
    card *cardplayed[MAXPLAYER];  // NULL for players who have not played yet
    int   playerled;              // player id who led the current trick
    int   turn;                   // position within the turn order
    BOOL  bQSPlayed;              // queen of spades has been played
    BOOL  bShootingRisk;          // someone may be shooting the moon
    BOOL  bHumanShooter;          // the shooter is a human player
    int   nMoonShooter;           // player id of the suspected shooter
};

// --------------------------------------------------------------------------
// MOVE -- a card play, exchanged between players over DDE.
// --------------------------------------------------------------------------
struct MOVE
{
    int playerid;     // player id who played the card
    int cardid;       // id of the card played
    int playerled;    // player id who led the current trick
    int turn;         // turn number
};

// --------------------------------------------------------------------------
// PASS3 -- three cards passed by a single player.
// --------------------------------------------------------------------------
struct PASS3
{
    int id;           // player id
    int cardid[3];    // card ids passed
};

// --------------------------------------------------------------------------
// PASS12 -- pass selections of all four players.
// --------------------------------------------------------------------------
struct PASS12
{
    int passdir;                  // pass direction for this hand
    int cardid[MAXPLAYER][3];     // cardid[player][i]
};

// --------------------------------------------------------------------------
// GAMESTATUS -- player names and ids broadcast by the gamemeister over DDE.
// The name buffer is what issue #13 protects against overflowing.
// --------------------------------------------------------------------------
struct GAMESTATUS
{
    int  id;                          // player id of this conversation (-1 none)
    TCHAR name[MAXPLAYER][MAXNAMELENGTH + 1];
};

// --------------------------------------------------------------------------
// player -- base class for local human, computer and remote human players.
// --------------------------------------------------------------------------
class player
{
public:
    player(int nID, int nSeat);
    virtual ~player() {}

    LPCTSTR GetName() const { return m_name; }
    VOID SetName(const CString &rName, CDC &dc);
    VOID DisplayName(CDC &dc);
    HCONV GetConv() const { return m_hConv; }
    int GetMode() const { return m_mode; }
    VOID SetMode(int nMode) { m_mode = nMode; }
    VOID Select(SLOT nSlot, BOOL bSelect);
    VOID Quit() { m_bQuitting = TRUE; }
    card *Card(SLOT nSlot);
    SLOT GetSlot(int nCardId);
    VOID MarkSelectedCards(CDC &dc);
    VOID MarkCardPlayed(SLOT nSlot);
    VOID SelectCardsToPass();
    VOID ReturnSelectedCards(int &nCardId);

protected:
    int     m_nID;
    int     m_nSeat;
    CString m_name;
    HCONV   m_hConv;
    int     m_mode;
    BOOL    m_bQuitting;
};

// --------------------------------------------------------------------------
// remote_human -- a player connected to the gamemeister over DDE.
// --------------------------------------------------------------------------
class remote_human : public player
{
public:
    remote_human(int nID, int nSeat, HCONV hConv);
};

#endif  // PLAYER_INC
