@echo off
SET DIR=.
SET OUTPUT_FILE=output.txt

REM Check if directory exists
if not exist "%DIR%" (
    echo Directory does not exist: %DIR% > %OUTPUT_FILE%
    exit /b
)

REM Clear the output file
type nul > %OUTPUT_FILE%

REM Loop through all .cpp files in the directory
for %%f in ("%DIR%\*.cpp") do (
    echo File: %%f >> %OUTPUT_FILE%
    echo -------------- >> %OUTPUT_FILE%
    type "%%f" >> %OUTPUT_FILE%
    echo ============== >> %OUTPUT_FILE%
)

REM Copy the entire content of the output file to the clipboard
type %OUTPUT_FILE% | clip

echo Output written to %OUTPUT_FILE% and its contents copied to clipboard.
