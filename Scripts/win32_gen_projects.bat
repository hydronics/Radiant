@echo off
pushd ..\
call thirdparty\bin\premake\premake5.exe vs2019
popd

PAUSE
