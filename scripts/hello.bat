@echo off
set loopcount=1000
:loop
echo Hello World! %loopcount%
set /a loopcount=loopcount-1
if %loopcount%==0 goto exitloop
timeout /t 1 /nobreak > NUL
goto loop
:exitloop
   