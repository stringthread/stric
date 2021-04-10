@echo off
pushd "%~dp0"
..\build\src\bin\parse ..\test.stric > ..\out.json
popd
