@echo off
winget install git.git
start /wait git clone https://github.com/Souto-Naitou/TD2_02_Prototype.git
cd TD2_02_Prototype\commands
start /wait CreateSubmodule.bat
start /wait Pull_Engine_Repository.bat
start /wait Pull_Easing_Repository.bat
pause