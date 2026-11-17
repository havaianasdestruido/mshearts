#ifndef RESOURCE_INC
#define RESOURCE_INC

// Dialog template IDs (the dialogs themselves live in hearts.dlg, a Win16
// .dlg text file; a modern build must convert them to a hearts.rc).
#define DLG_SCORE     100
#define DLG_QUOTE     101
#define DLG_WELCOME   102
#define DLG_OPTIONS   103
#define DLG_LOCATE    104

// Control IDs
#define IDC_NAME1     200
#define IDC_NAME2     201
#define IDC_NAME3     202
#define IDC_YOURNAME  203
#define IDC_SERVERNAME 204
#define IDC_MEISTER   205
#define IDC_JOIN      206
#define IDC_GROUP     207
#define IDC_FAST      208
#define IDC_SLOW      209
#define IDC_NORMAL    210
#define IDC_AUTO      211
#define IDC_AUTOGROUP 212
#define IDC_WELCOMEHELP 213
#define IDC_BROWSE    214
#define IDC_LOCATEHELP 215

// Command IDs
#define IDM_REF       300
#define IDM_NEWGAME   301

// String IDs
#define IDS_P1NAME    400
#define IDS_P2NAME    401
#define IDS_P3NAME    402
#define IDS_FONTFACE  403
#define IDS_CHARSET   404
#define IDS_FONTSIZE  405
#define IDS_SCORESHEET 406
#define IDS_PLACE1    407
#define IDS_GAMEOVER  408
#define IDS_GAMEOVERWIN 409
#define IDS_MEMORY    410
#define IDS_VERSION   411
#define IDS_DISCONNECT 412

// Help context IDs (kept in sync with helpnum.h -- identical redefinitions
// are permitted by the preprocessor).
#define IDH_START_HRTS 500
#define IDH_LOCATE     501

// Maximum length of a player name in TCHARs.  Fixed-size buffers (the
// welcome/options edit controls and GAMESTATUS.name[]) are sized to
// MAXNAMELENGTH + 1.  Remote names are truncated to this length before use
// (issue #13).
#define MAXNAMELENGTH 64

#define MAXCOMPUTERNAME 31

#endif  // RESOURCE_INC
