@echo off
cd ..\.engine
git submodule init
git submodule update

git pull origin master
pause