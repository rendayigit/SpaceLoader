echo off

tasklist | find /i "SERVER.exe" && taskkill /im notepad.exe /F || echo process "SERVER.exe" not running.
tasklist | find /i "CONSOLE.exe" && taskkill /im notepad.exe /F || echo process "CONSOLE.exe" not running.
tasklist | find /i "tests.exe" && taskkill /im notepad.exe /F || echo process "tests.exe" not running.