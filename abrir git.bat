@echo off
setlocal

set "CURRENT_DIR=%CD%"

set "GIT_BASH=C:\Program Files\Git\git-bash.exe"

start "" "%GIT_BASH%" --cd="%CURRENT_DIR%"