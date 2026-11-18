# mshearts

mshearts taken from Windows NT 5.1's source tree, now patched to add new
stuff.

## Build status

The source cannot be compiled out of the box as imported: the original
repository drop lost several headers (`main.h`, `player.h`, `resource.h`,
`helpnum.h`) and there was no build system (issue #9).  This tree now
contains:

- stub replacements for the missing headers (`mshearts/main.h`,
  `mshearts/player.h`, `mshearts/resource.h`, `mshearts/helpnum.h`) so every
  `#include` resolves in-repo, plus `mshearts/globals.cpp` with stub
  definitions of the global game state;
- `CMakeLists.txt` describing a Windows/MSVC/MFC build;
- `tools/check_includes.py` (run by CI) verifying that all quoted includes
  resolve;
- fixes for all open issues (see below).

A faithful build still requires restoring the original Windows NT 5.1
sources (`main.cpp`, `player.cpp`, `hearts.rc` compiled from the Win16
`hearts.dlg` dialog text) and building with Visual Studio + MFC + the
Windows SDK (see "Build requirements" below).

## Build requirements

- Windows 10/11 SDK
- Visual Studio 2019 or 2022 with the "Desktop development with C++
  workload" including the **MFC** component (afxwin.h etc.)
- `cards.dll` (the legacy Windows card DLL) on the PATH at runtime

Open `CMakeLists.txt` in Visual Studio (it generates a .vcxproj) or run
`cmake -S . -B build` followed by `cmake --build build`.  MFC must be
enabled for the target (see `CMakeLists.txt`).  Link against `ddeml.lib`
and `htmlhelp.lib`.

## Open issues fixed in this branch

| # | Issue | Fix |
|---|-------|-----|
| #2 | `__builtin_popcount` won't compile with MSVC | Already fixed by PR #15: `computer::CountBits` uses portable Kernighan bit counting |
| #3 | `card` ctor/dtor crash on `cards.dll` load/proc failure | Guard all `GetProcAddress` results, free library on failure paths, only count successfully constructed cards, NULL-guard every `lpcdtDraw`/`lpcdtTerm` call site |
| #4 | `DDEClient` destructor disconnects NULL/uninitialized `m_hConv` | Initialize `m_hConv` to NULL in the ctor, guard the destructor call |
| #5 | Obsolete Win16 APIs (`MakeProcInstance`/`FreeProcInstance`/`WinExec`) | Removed thunks (callback stored directly); `WinExec` replaced with `ShellExecute` |
| #6 | `DDECALLBACK` uses Win16 WORD/DWORD signature | Win32 signature (`UINT`/`UINT`/.../`ULONG_PTR`/`ULONG_PTR`, `__stdcall`) matching `<ddeml.h>` |
| #7 | DDE helpers mix byte and TCHAR counts (Unicode corruption) | `GetDataString` sizes/copies in bytes; `Poke(string)` sends `(_tcslen+1)*sizeof(TCHAR)` bytes with `CF_UNICODETEXT`/`CF_TEXT` as appropriate |
| #8 | `card::Glide` clobbers the caller's DC clip region | `Glide` wrapped in `SaveDC`/`RestoreDC`; screen-sized clip reset removed; `GlideStep` never modifies a region while selected into a DC |
| #9 | Missing headers/build system — cannot compile | Stub `main.h`/`player.h`/`resource.h`/`helpnum.h`, `globals.cpp`, `CMakeLists.txt`, CI include check, build docs |
| #10 | Empty `_setargv()`/`_setenvp()` stubs break CRT parsing | Removed both stubs from `hearts.cpp` |
| #11 | `computer::Setup` calls `c->IsValid()` on NULL card pointers | NULL check before `IsValid()`; removed the `ASSERT(this != NULL)` anti-pattern from `card.h` |
| #12 | `COptionsDlg::OnInitDialog` uses stale CString buffer pointer | `SetWindowText(sName[i])` after all `CString` mutations instead of the invalidated `p` pointer |
| #13 | Unbounded `lstrcpy` in `GetGameStatus` — remote stack buffer overflow | `lstrcpyn` with forced NUL termination in `GetGameStatus`; remote names truncated to `MAXNAMELENGTH` at receipt in `AddNewPlayer` |

## Verification

This sandbox has no Windows/MSVC toolchain, so the fixes were validated by
static analysis and by `tools/check_includes.py` (all quoted `#include`
directives now resolve to files in the repository).  A real build and
functional test require Windows with MFC and `cards.dll`.

## License

See LICENSE.
