cmake_minimum_required(VERSION 3.18)
project(mshearts CXX)

# ---------------------------------------------------------------------------
# mshearts -- Microsoft Hearts Network (Windows NT 5.1 port)
#
# This is a Win32/MFC application.  It requires:
#   * Visual Studio with the MFC component (afxwin.h, afxext.h, afxcmn.h)
#   * the Windows SDK (ddeml.lib, htmlhelp.lib)
#   * cards.dll at runtime (legacy card-drawing DLL from the Windows NT era)
#
# The repository import lost several original source files (main.h/.cpp,
# player.h/.cpp, resource.h, helpnum.h and the .rc script -- see issue #9).
# Stub headers and globals.cpp are provided so that every #include resolves
# and the source tree is coherent, but a faithful build must restore the
# original files and compile hearts.dlg (Win16 dialog format) into hearts.rc.
# ---------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
    message(FATAL_ERROR "mshearts targets MSVC and the Windows SDK (MFC/Win32).")
endif()

set(MSHEARTS_SRCS
    mshearts/card.cpp
    mshearts/comp2.cpp
    mshearts/computer.cpp
    mshearts/dde.cpp
    mshearts/ddecb.cpp
    mshearts/dlg.cpp
    mshearts/globals.cpp
    mshearts/hearts.cpp
)

add_executable(mshearts WIN32 ${MSHEARTS_SRCS})

target_compile_options(mshearts PRIVATE /W4 /GS /sdl)

# MFC must be enabled for this target (Use of MFC = Static/Shared in the
# generated .vcxproj, or /clr or afxwin.h availability).  With raw CMake on
# MSVC, consume the SDK MFC libs by setting the target properties manually:
set_target_properties(mshearts PROPERTIES
    VS_GLOBAL_UseOfMFC static
    VS_GLOBAL_CLR false
)

target_link_libraries(mshearts PRIVATE ddeml.lib htmlhelp.lib user32.lib advapi32.lib shell32.lib gdi32.lib comdlg32.lib winmm.lib)

# The original build produced hearts.rc from hearts.dlg (Win16 dialog text
# format).  Add hearts.rc here once it has been converted:
# target_sources(mshearts PRIVATE mshearts/hearts.rc)
