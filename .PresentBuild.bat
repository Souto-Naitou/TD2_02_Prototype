@echo on
cd ".engine"
git stash
git pull origin master
xcopy ..\.cpyorgn\00_01.vcxproj .\ /e /y

pause