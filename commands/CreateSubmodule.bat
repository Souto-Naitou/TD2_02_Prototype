@echo off
cd..
rmdir .engine /s /q
rmdir .Externals\Easing /s /q

git submodule init
git submodule update

cd .engine
git checkout TD2_02
git pull

exit