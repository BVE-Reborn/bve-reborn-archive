@echo off
setlocal enableextensions

md "%1"
copy ".\docs\config\Doxyfile" "%1\Doxyfile"
echo OUTPUT_DIRECTORY ^= %1>>"%1\Doxyfile"
doxygen %1\Doxyfile

endlocal