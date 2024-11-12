@echo off
cd..
rmdir .engine /s /q
git submodule init
git submodule update
exit