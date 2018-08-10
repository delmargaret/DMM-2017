ml /c /Cp /coff %~n1.asm
link /SUBSYSTEM:CONSOLE /STACK:1000000,1000000 %~n1.obj