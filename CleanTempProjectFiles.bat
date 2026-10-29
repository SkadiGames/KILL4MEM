@echo off
title Cleanup Project Folders

echo Removing folders: .vs, Binaries, DerivedDataCache, Intermediate, Saved
echo and file TILEWARDEN.sln from %cd% and subfolders...
echo.

:: Удаление папок (рекурсивно, включая скрытые)
set targets=.vs Binaries DerivedDataCache Intermediate Saved

for /f "delims=" %%i in ('dir /b /s /ad /a "%cd%" 2^>nul') do (
    for %%t in (%targets%) do (
        if /i "%%~nxi"=="%%t" (
            echo Deleting folder "%%i"
            rd /s /q "%%i"
        )
    )
)

:: Удаление файла .sln (только в текущей папке)
if exist "TILEWARDEN.sln" (
    echo Deleting file "TILEWARDEN.sln"
    del /f /q "TILEWARDEN.sln"
) else (
    echo File TILEWARDEN.sln not found in current folder.
)

echo Done.