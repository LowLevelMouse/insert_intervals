@echo off
setlocal

del /q *.obj *.pdb *.exe *.ilk *.lib *.exp 2>nul

set CFLAGS=/nologo /Zi /Od /W4 /Fe:test.exe
set SOURCE=insert_interval.cpp
set LFLAGS=/DEBUG /SUBSYSTEM:CONSOLE 

cl %CFLAGS% %SOURCE% /link %LFLAGS%

endlocal