@echo off
pushd "%~dp0"
..\build\src\bin\main < ..\test.stric
popd
