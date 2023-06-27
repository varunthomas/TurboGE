@echo off

REM Checking if Python is installed
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Python is not installed. Please install Python and try again. If installed, make sure to add the Python PATH to environment variable path
    PAUSE
    exit
)

REM Extracting Python version information
for /F "tokens=2" %%G in ('python --version 2^>^&1') do (
    set python_version=%%G
)
REM Comparing Python version
set min_version=3.7.0
setlocal EnableDelayedExpansion
python -c "import sys; sys.exit(1 if tuple(map(int, sys.version_info[:3])) < tuple(map(int, '!python_version:~0,3!'.split('.'))) else 0)" >nul 2>&1
if %errorlevel% neq 0 (
    echo Python version is below the required minimum version of %min_version%. Please update Python and try again.
    PAUSE
    exit
)

for /F "delims=" %%I in ('where python') do (
    set "python_path=%%~dpI"
    goto :found
)

:found
echo Python is installed at: %python_path%
setx PYTHON_LIBS "%python_path:~0,-1%"

REM Python is installed, running the script
python prerequisite.py
python setup_premake.py
PAUSE