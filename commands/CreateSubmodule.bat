@echo off
cd..
rmdir .engine /s /q
rmdir .Externals\Easing /s /q

git submodule init
git submodule update

exit