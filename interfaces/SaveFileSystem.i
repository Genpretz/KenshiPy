%{
#include "SaveFileSystem.h"
%}

%include "std_string.i"

%ignore SaveFileSystem::fileSystem;
%ignore SaveFileSystem::deletedFiles;
%ignore SaveFileSystem::paths;
%ignore SaveFileSystem::oldCurrentFolders;
%ignore SaveFileSystem::currentMutex;
%ignore SaveFileSystem::saveLog;
%nocopyctor SaveFileSystem;

%include "SaveFileSystem.h"
