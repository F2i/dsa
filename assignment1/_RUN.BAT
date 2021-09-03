@echo off
set "cmd=D:\work_space\Cpp\initial\testcase\main.exe"
for %%a in (*.txt) do ( echo ---------------------%%a------------------ && %cmd% %%a ) 
rem %cmd% buhbuh.txt
echo --------------------------------------------------
rem pause