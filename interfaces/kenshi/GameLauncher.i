%{
#include "kenshi/GameLauncher.h"
%}

%include "std_string.i"

// GameLauncher uses MFC/ATL types (CDialog, CCheckListBox, ATL::CString, etc.)
// which are Windows-specific and cannot be wrapped by SWIG for Python.
// Suppress the entire file; only expose enums.

%ignore CExCheckListBox;
%ignore GameLauncher;

enum GameLauncher_RendererType
{
    RENDERER_DX9  = 0,
    RENDERER_DX11 = 1
};
