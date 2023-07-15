call gccenv
g++ systray.cpp -lgdi32 --shared -o systray.dll
rem cc systray.cpp -lgdi32 -o systray.exe
