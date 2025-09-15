@echo off
setlocal EnableExtensions EnableDelayedExpansion

REM -- Work inside the folder where this script lives
pushd "%~dp0" || (echo [ERROR] Cannot access script folder. & exit /b 1)

REM -- Folder name (just FYI / messages)
for %%I in ("%CD%") do set "FOLDER=%%~nxI"

REM -- Find newest CREATED .h file (excluding Universe.h)
set "SRC="
for /f "delims=" %%F in ('dir /b /a:-d /tc /o:-d *.h 2^>nul') do (
  if /i not "%%~nxF"=="Universe.h" (
    set "SRC=%%~fF"
    goto :found_src
  )
)

REM -- If nothing found, seed a minimal header
echo [WARN] No .h files found (except Universe.h). Seeding "Universe_seed.h".
set "SRC=%CD%\Universe_seed.h"
> "Universe_seed.h" (
  echo #pragma once
  echo // Auto-created seed header
  echo struct Seed { int dummy = 0; };
)

:found_src

REM -- Timestamp: Universe_<day>_<month>_<year>_<hour>_<minutes>.h
for /f "usebackq delims=" %%T in (`
  powershell -NoProfile -Command "(Get-Date).ToString('dd_MM_yyyy_HH_mm')"
`) do set "STAMP=%%T"

set "DST=Universe_%STAMP%.h"

REM -- Copy newest header to timestamped file (in the same folder)
copy /Y "%SRC%" "%DST%" >nul || (echo [ERROR] Copy failed. & popd & exit /b 1)

REM -- Overwrite Universe.h to include the fresh header (no folder prefix!)
> "Universe.h" (
  echo // Auto-generated in "%FOLDER%" on %DATE% %TIME%
  echo #pragma once
  echo #include "Universe_%STAMP%.h"
)

echo [OK] Source : "%SRC%"
echo [OK] Created: "%CD%\%DST%"
echo [OK] Wrote  : "%CD%\Universe.h"

popd
endlocal
