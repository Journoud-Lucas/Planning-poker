@echo off

:: Checks that you have doxygen 
where doxygen >nul 2>&1
if %errorlevel% neq 0 (
    echo "The doxygen command is not recognized. Make sure Doxygen is installed and configured in your PATH."
    pause
    exit /b 1
)

:: Run doxygen
doxygen

:: Pause before closing
pause