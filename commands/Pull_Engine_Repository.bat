@echo on

set originDir=%cd%

cd ..\.engine
git stash
git pull origin master

exit