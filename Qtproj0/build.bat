@echo off
REM 设置Qt环境变量
set PATH=D:\Qt\6.9.2\mingw_64\bin;D:\Qt\Tools\mingw1310_64\bin;%PATH%
set QT_DIR=D:\Qt\6.9.2\mingw_64

REM 进入项目目录
cd /d "%~dp0"

REM 清理旧的构建文件
if exist Makefile del Makefile
if exist Makefile.Debug del Makefile.Debug
if exist Makefile.Release del Makefile.Release

REM 生成Makefile
qmake.exe Qtproj0.pro

REM 编译项目
mingw32-make.exe

REM 暂停以查看结果
pause
