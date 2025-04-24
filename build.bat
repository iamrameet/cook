@echo off
echo Building Cook Programming Language...

REM Create bin directory if it doesn't exist
if not exist bin mkdir bin

REM Compile source files
g++ -std=c++14 -I include -o bin/cook.exe src/main.cpp src/lexer.cpp src/parser.cpp src/ast.cpp src/interpreter.cpp

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Executable created at bin/cook.exe
) else (
    echo Build failed with error code %ERRORLEVEL%
)
