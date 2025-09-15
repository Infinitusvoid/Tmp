@echo off
setlocal EnableExtensions EnableDelayedExpansion

REM === Paths (relative to this script) ===
set "ROOT=%~dp0"
set "UNIVERSES=%ROOT%universes"

if not exist "%UNIVERSES%" (
  mkdir "%UNIVERSES%" || (echo [ERROR] Could not create "%UNIVERSES%". & exit /b 1)
)

REM === Find newest-by-creation-time universe_*.h inside universes ===
set "SRC="
for /f "usebackq delims=" %%I in (`
  powershell -NoProfile -Command ^
    "$ErrorActionPreference='Stop';" ^
    "$d = Get-ChildItem -File -LiteralPath '%UNIVERSES%' -Filter 'universe_*.h' |" ^
    "Sort-Object CreationTime -Descending | Select-Object -First 1;" ^
    "if($d){$d.FullName}"
`) do set "SRC=%%I"

REM === Seed if none found ===
if not defined SRC (
  echo [WARN] No existing "universe_*.h" in "%UNIVERSES%".
  echo        Creating a minimal seed header: universe_seed.h
  set "SRC=%UNIVERSES%\universe_seed.h"
  (
    echo #pragma once
    echo // Seed universe header auto-created
    echo struct RNGSeed { int dummy; };
  ) > "%SRC%"
)

REM === Timestamp: universe_<day>_<month>_<year>_<hour>_<minutes>.h ===
for /f "usebackq delims=" %%T in (`
  powershell -NoProfile -Command "(Get-Date).ToString('dd_MM_yyyy_HH_mm')"
`) do set "STAMP=%%T"

set "DST=%UNIVERSES%\universe_%STAMP%.h"

REM === Copy the newest file to the timestamped name ===
copy /Y "%SRC%" "%DST%" >nul || (echo [ERROR] Copy failed. & exit /b 1)

REM === Overwrite univere.h in ROOT to include the fresh file ===
set "INCLUDE_FILE=%ROOT%univere.h"
(
  echo // Auto-generated on %DATE% %TIME%
  echo #pragma once
  echo #include "universes/universe_%STAMP%.h"  // Flux from scratch on CPU example
) > "%INCLUDE_FILE%"

echo [OK] New universe: "%DST%"
echo [OK] Wrote include: "%INCLUDE_FILE%"
endlocal
