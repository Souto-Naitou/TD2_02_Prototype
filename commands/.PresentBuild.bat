@echo on

set originDir=%cd%

cd ..\.engine
git stash
git pull origin master

setlocal enabledelayedexpansion
cd !originDir!

xcopy .cpyorgn\00_01.vcxproj ..\.engine /e /y

pause