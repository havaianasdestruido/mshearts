# mshearts code audit — final report

**Date:** 2026-10-16
**Repository:** `havaianasdestruido/mshearts` (branch `arena/e13b9dd1-mshearts`)
**Scope:** Full-repository static audit of all tracked source files.
**Audit method:** Manual code review of every `.cpp`/`.h`/`.dlg` file, cross-reference with Windows SDK/MFC documentation, grep-driven defect search for common C/C++/Win32 bug patterns, and build-feasibility analysis.

## Summary of issues created

12 distinct, actionable GitHub issues were filed. All were verified as real defects via static analysis; none were speculative.

| # | Title | Severity | Subsystem |
|---|---|---|---|
| [#2](https://github.com/havaianasdestruido/mshearts/issues/2) | `build: __builtin_popcount is GCC/Clang-specific and will not compile with MSVC` | High (build) | AI/player logic |
| [#3](https://github.com/havaianasdestruido/mshearts/issues/3) | `bug: card constructor/destructor do not safely handle cards.dll load/proc-address failures (null-deref crash, unbalanced count)` | High (crash) | Card rendering |
| [#4](https://github.com/havaianasdestruido/mshearts/issues/4) | `bug: DDEClient destructor calls DdeDisconnect on potentially NULL/uninitialized m_hConv` | Medium-High (crash) | Networking/DDE |
| [#5](https://github.com/havaianasdestruido/mshearts/issues/5) | `build: Obsolete Win16 APIs (MakeProcInstance/FreeProcInstance/WinExec) removed from modern Windows SDKs` | High (build) | DDE / Options dialog |
| [#6](https://github.com/havaianasdestruido/mshearts/issues/6) | `bug: DDECALLBACK uses Win16 WORD/DWORD signature — wrong calling convention for Win32/Win64` | Medium (x64 crash) | Networking/DDE |
| [#7](https://github.com/havaianasdestruido/mshearts/issues/7) | `bug: DDE string helpers mix up byte counts and TCHAR counts — garbled/corrupt traffic in Unicode builds` | Medium (data corruption) | Networking/DDE |
| [#8](https://github.com/havaianasdestruido/mshearts/issues/8) | `bug: card::Glide clobbers the DC clip region and mutates a CRgn while selected into the DC` | Medium (visual/GDI) | Card rendering/animation |
| [#9](https://github.com/havaianasdestruido/mshearts/issues/9) | `build: Repository is missing multiple source headers and any build system — cannot compile` | Critical (build) | Repository structure |
| [#10](https://github.com/havaianasdestruido/mshearts/issues/10) | `bug: Empty _setargv/_setenvp stubs disable CRT command-line/environment parsing and break modern linkers` | Low-Medium | CRT/startup |
| [#11](https://github.com/havaianasdestruido/mshearts/issues/11) | `bug: computer::Setup calls c->IsValid() on potentially-NULL card pointers (UB / AV crash)` | Medium (crash) | AI/player logic |
| [#12](https://github.com/havaianasdestruido/mshearts/issues/12) | `bug: COptionsDlg::OnInitDialog uses stale CString buffer pointer after ReleaseBuffer/LoadString` | Medium (UB/garbled UI) | Options dialog |
| [#13](https://github.com/havaianasdestruido/mshearts/issues/13) | `security: Unbounded lstrcpy in GetGameStatus allows remotely-triggered stack buffer overflow via long player name` | High (security) | Networking/DDE |

Total issues: **12**.

## Categories of defects found

- **Build / portability regressions (3):** #2 (GCC intrinsic in MSVC project), #5 (removed Win16 APIs), #9 (missing headers / no build system).
- **Runtime crashes / undefined behaviour (5):** #3 (null-deref on DLL load failure), #4 (DdeDisconnect on NULL conv), #8 (GDI clip clobber / selected-region mutation), #10 (CRT argv/envp stubs), #11 (null `this` call via c->IsValid()), #12 (stale CString pointer).
- **Network / data corruption (2):** #6 (wrong DDE callback signature, breaks x64), #7 (byte-vs-TCHAR bug in Unicode DDE).
- **Security (1):** #13 (remotely-triggered stack buffer overflow via long player name).

## Notable findings from the audit

1. **The repository is incomplete.** Five local headers (`main.h`, `player.h`, `resource.h`, `helpnum.h`) and at least the `hearts.rc` resource script and several `.cpp` files (`main.cpp`, `player.cpp`) referenced by existing files are not present. There is also no `Makefile`/`CMakeLists.txt`/`.vcxproj`, so the project cannot be compiled from a clean clone. This is filed as #9 and is the single biggest blocker to any further automated testing.
2. **The recent "bolt" performance PR introduced one confirmed regression**: `__builtin_popcount` (see `.jules/bolt.md`) is GCC-specific and breaks MSVC compilation (#2). The `std::sqrt` change in `IntSqrt` is functionally fine on its own.
3. **Several Win16-era artefacts remain** and prevent compilation/linking on modern Windows: `MakeProcInstance`/`FreeProcInstance`, `WinExec`, the 16-bit DDE callback typedef, and `_setargv`/`_setenvp` CRT hook overrides.
4. **The DDE code has multiple correctness issues** — NULL-handle derefs, wrong byte/character sizes, a buffer-copy vulnerability — which is unsurprising for a 1992 network layer, but they are real crashes and one real security bug.
5. **The defensive `ASSERT(this != NULL)` / `IsValid()` idiom in `card.h` is a C++ anti-pattern** — calling a member function through a null pointer is UB, so the guards don't protect anything; the NULL check needs to be performed *before* the member call, as documented in #11.

## Verification methods actually executed

- Full recursive enumeration of all tracked files.
- Line-by-line reading of every `.cpp`, `.h`, `.dlg`, and `.md` file.
- Targeted `grep` searches for: unsafe C string APIs (`strcpy`, `lstrcpy`, `sprintf`, `wsprintf`), obsolete Win16 APIs (`MakeProcInstance`, `FreeProcInstance`, `WinExec`), unchecked pointer dereferences after NULL-able GetProcAddress / DdeConnect calls, incorrect DDEML callback signatures, byte/char count mixups (`lstrlen`/`GetBuffer`/`cbData`), missing initializers, post-`ReleaseBuffer` pointer use, CRT hooks, and toolchain-intrinsic portability.
- `git log` review (one merge commit from a perf PR) to identify regressions.
- `gh issue list` / `gh label list` to review existing state (no pre-existing issues; labels were limited; I added the `security` label).
- Verified the GitHub CLI created each issue successfully and that URLs resolve.

## Verification limitations

- **No Windows / MSVC / MFC / DDEML runtime** is available in the Linux sandbox, so runtime reproduction of crashes, DDE traffic, or UI glitches was not possible. Every defect filed is supported by static reasoning, documented Windows SDK contracts, and/or code-path logic — no fabricated execution output appears in any issue.
- **Several header files are missing** (`main.h`, `player.h`, `resource.h`, `helpnum.h`), so the exact dimensions of some fixed-size buffers (e.g. `GAMESTATUS.name[]`, `MAXNAMELENGTH`) could not be quoted. Issue #13 (lstrcpy overflow) was filed on the basis that `lstrcpy` into a fixed-size buffer fed by network input is unsafe regardless of the buffer size; the same issue text calls out the missing-header limitation explicitly.
- **No static-analysis tools (MSVC `/analyze`, clang-tidy, Cppcheck)** were run because there is no compilable unit — without the missing headers and build system the translation units will not parse.
- **No dynamic analysis (ASan, AppVerifier)** was possible for the same reason.
- **Tests:** There are no unit tests, no test framework, and no CI configuration in the repository. That is itself a structural deficiency, though not distinct from the missing-build-system issue (#9).

## Areas not covered / suspicions not confirmed

- **Hearts game rules / AI correctness**: the computer-player logic (`comp2.cpp`, `computer.cpp`) could not be executed, so strategy bugs (e.g. incorrect card-priority choices, moon-shoot detection edge cases) were not investigated beyond code inspection of control flow. None rose to the level of a confirmed, actionable defect from static reading alone.
- **Sound (glass.wav) / icon / resource rendering**: binary resource files were not analysed.
- **HTMLHelp integration**: `htmlhelp.h` is included but the help calls are commented out in `dlg.cpp`; not a defect.
- **Registry (`RegEntry`) helper**: the `RegEntry` class is declared in the missing `main.h`, so its error handling / buffer management could not be audited.
- **Score sheet rendering math**: inspected and found to be correct for MAXPLAYER=4, with the possible exception of integer overflow if someone plays thousands of hands (not realistic).
- **Minor style issues** (duplicate `#include <afxwin.h>` in `hearts.h`, missing include guard on `debug.h`, typo "source doe" in `README.md`) were noted but judged too cosmetic/non-actionable to file issues for given the project's legacy nature.

## Duplicate consolidation

- The various "null pointer" crash sites (`lpcdtTerm`, `lpcdtDraw`, `hCardsDLL`, unbalanced `count`) were consolidated into issue #3 because they all originate in the same constructor/destructor failure-handling defect and would be fixed together.
- The DDE byte-vs-TCHAR bugs in `GetDataString` and `Poke(string)` were consolidated into issue #7.
- Obsolete Win16 API removals (`MakeProcInstance`/`FreeProcInstance`/`WinExec`) were consolidated into issue #5.
- No other duplicates were found.
