@echo off
echo Downloading FreeGLUT for Windows...
powershell -Command "Invoke-WebRequest -Uri 'https://www.transmissionzero.co.uk/files/software/development/GLUT/freeglut-MinGW.zip' -OutFile 'freeglut.zip'"
echo Extracting...
powershell -Command "Expand-Archive -Path 'freeglut.zip' -DestinationPath 'freeglut_temp'"
echo Copying files...
copy "freeglut_temp\freeglut\bin\freeglut.dll" "lib\"
copy "freeglut_temp\freeglut\lib\libfreeglut.a" "lib\"
copy "freeglut_temp\freeglut\include\GL\*.h" "include\GL\"
echo Cleaning up...
rmdir /S /Q freeglut_temp
del freeglut.zip
echo Done! Libraries installed.
pause