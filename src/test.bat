@echo off
setlocal EnableDelayedExpansion
cd /d "%~dp0"
set "SAMPLES_DIR=test\resources\samples"
set "PROGRAM=TemperatureAnalyzer.exe"


for %%f in ("%SAMPLES_DIR%\measurements-*.txt") do (
    set "INPUT_FILE=%%f"
    set "BASENAME=%%~nf"
    set "EXPECTED_OUTPUT=%SAMPLES_DIR%\%%~nf.out"
    set "ACTUAL_OUTPUT=%%~nf_actual.out"

    echo Testing with "!INPUT_FILE!"...

    "%PROGRAM%" "!INPUT_FILE!" > "!ACTUAL_OUTPUT!"

    fc "!ACTUAL_OUTPUT!" "!EXPECTED_OUTPUT!" > nul
    if errorlevel 1 (
        echo Test !BASENAME! failed.
        echo Differences:
        fc "!ACTUAL_OUTPUT!" "!EXPECTED_OUTPUT!"
    ) else (
        echo Test !BASENAME! passed.
    )

    rem del "!ACTUAL_OUTPUT!"
)
