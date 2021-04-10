@echo off
pushd "%~dp0"
..\build\src\bin\lex  ..\test.stric
popd
