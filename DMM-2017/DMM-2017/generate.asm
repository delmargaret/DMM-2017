.586
.model flat, stdcall
include kernel32.inc
include user32.inc
include masm32.inc
includelib kernel32.lib
includelib user32.lib
includelib masm32.lib
includelib mylibrary.lib
ExitProcess PROTO :DWORD
STD_OUTPUT_HANDLE EQU -11
GetStdHandle PROTO, nStdHandle: DWORD
sum PROTO :DWORD, :SDWORD
max PROTO :DWORD, :SDWORD
.stack 4096
.const
.data
consoleOutHandle1 dd ?
bytesWritten1 dd ?
lmessage1 dd ?
newline1 BYTE 13, 10, 0
buffer1 BYTE 128 dup(?)
retStr1 BYTE 256 dup(?)
intstr_10 BYTE "String /\", 0
integ_44 BYTE "hello", 0
main_140 BYTE "Hello ", 0
main_141 BYTE "World!", 0
.code
intstr PROC x:SDWORD,y:SDWORD,s:DWORD

LOCAL S1 [256]:BYTE
LOCAL S2 [256]:BYTE
LOCAL string [256]:BYTE
LOCAL res:SDWORD
LOCAL parm:SDWORD

MOV S1[0], 0
MOV S2[0], 0
push ecx
INVOKE GetStdHandle, STD_OUTPUT_HANDLE
mov consoleOutHandle1, eax
pushad
mov ebx, x
INVOKE dwtoa, ebx, addr buffer1
INVOKE lstrlen, addr buffer1
mov edx, offset buffer1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
INVOKE lstrlen, addr newline1
mov edx, offset newline1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
popad
pop ecx
PUSH ecx
invoke lstrcpy, addr S2, addr S1
invoke lstrcpy, addr S1, offset intstr_10
POP ecx
PUSH ecx
invoke lstrcpy, addr string, addr S1
POP ecx
push ecx
INVOKE GetStdHandle, STD_OUTPUT_HANDLE
mov consoleOutHandle1, eax
pushad
INVOKE lstrlen, addr string
INVOKE WriteConsoleA, consoleOutHandle1, addr string, eax, offset bytesWritten1, 0
INVOKE lstrlen, addr newline1
mov edx, offset newline1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
popad
pop ecx
PUSH ecx
invoke lstrcpy, addr S2, addr S1
invoke lstrcpy, addr S1, s
POP ecx
PUSH ecx
invoke lstrcpy, addr string, addr S1
POP ecx
push ecx
INVOKE GetStdHandle, STD_OUTPUT_HANDLE
mov consoleOutHandle1, eax
pushad
INVOKE lstrlen, addr string
INVOKE WriteConsoleA, consoleOutHandle1, addr string, eax, offset bytesWritten1, 0
INVOKE lstrlen, addr newline1
mov edx, offset newline1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
popad
pop ecx
PUSH 10
POP eax
MOV parm, eax
PUSH parm
PUSH x
PUSH y
POP ebx
POP eax
ADD eax, ebx
PUSH eax
POP ebx
POP eax
XOR EDX, EDX
IMUL ebx
PUSH eax
POP eax
MOV res, eax

PUSH res

POP eax
RET
intstr ENDP
integ PROC x:SDWORD

LOCAL S1 [256]:BYTE
LOCAL S2 [256]:BYTE
LOCAL zero:SDWORD
LOCAL dwx:SDWORD
LOCAL h [10]:SDWORD
LOCAL func:SDWORD
LOCAL P0[256]:BYTE

MOV S1[0], 0
MOV S2[0], 0
PUSH 0
POP eax
MOV zero, eax
PUSH 0
POP eax
MOV dwx, eax
mov ecx, 10
.while ecx > 0
PUSH 0
XOR edx, edx
MOV eax, zero
PUSH ebx
MOV ebx, 4
MUL ebx
POP ebx
MOV edx, eax
POP eax
MOV h[edx], eax
PUSH zero
PUSH 1
POP ebx
POP eax
ADD eax, ebx
PUSH eax
POP eax
MOV zero, eax
dec ecx
.endw
PUSH 0
POP eax
MOV zero, eax
PUSH ecx
invoke lstrcpy, addr P0, offset integ_44
POP ecx
INVOKE intstr,2,3,addr P0
PUSH eax
POP eax
MOV func, eax
PUSH func
XOR edx, edx
MOV eax, dwx
PUSH ebx
MOV ebx, 4
MUL ebx
POP ebx
MOV edx, eax
POP eax
MOV h[edx], eax
mov ecx, x
.while ecx > 0
XOR edx, edx
MOV eax, dwx
PUSH ebx
MOV ebx, 4
MUL ebx
POP ebx
PUSH h[eax]
PUSH 1
POP ebx
POP eax
ADD eax, ebx
PUSH eax
XOR edx, edx
MOV eax, dwx
PUSH ebx
MOV ebx, 4
MUL ebx
POP ebx
MOV edx, eax
POP eax
MOV h[edx], eax
XOR edx, edx
MOV eax, dwx
PUSH ebx
MOV ebx, 4
MUL ebx
POP ebx
PUSH h[eax]
POP eax
MOV zero, eax
push ecx
INVOKE GetStdHandle, STD_OUTPUT_HANDLE
mov consoleOutHandle1, eax
pushad
mov ebx, zero
INVOKE dwtoa, ebx, addr buffer1
INVOKE lstrlen, addr buffer1
mov edx, offset buffer1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
INVOKE lstrlen, addr newline1
mov edx, offset newline1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
popad
pop ecx
PUSH dwx
PUSH 1
POP ebx
POP eax
ADD eax, ebx
PUSH eax
POP eax
MOV dwx, eax
dec ecx
.endw

PUSH zero

POP eax
RET
integ ENDP
ab PROC x:DWORD,y:DWORD

LOCAL S1 [256]:BYTE
LOCAL S2 [256]:BYTE
LOCAL one [256]:BYTE
LOCAL two [256]:BYTE
LOCAL three [256]:BYTE

MOV S1[0], 0
MOV S2[0], 0
PUSH ecx
invoke lstrcpy, addr S2, addr S1
invoke lstrcpy, addr S1, x
POP ecx
PUSH ecx
invoke lstrcpy, addr one, addr S1
POP ecx
PUSH ecx
invoke lstrcpy, addr S2, addr S1
invoke lstrcpy, addr S1, y
POP ecx
PUSH ecx
invoke lstrcpy, addr two, addr S1
POP ecx
PUSH ecx
invoke lstrcpy, addr S2, addr S1
invoke lstrcpy, addr S1, addr one
POP ecx
PUSH ecx
invoke lstrcpy, addr S2, addr S1
invoke lstrcpy, addr S1, addr two
POP ecx
PUSH ecx
invoke lstrcat, addr S2, addr S1
invoke lstrcpy, addr S1, addr S2
POP ecx
PUSH ecx
invoke lstrcpy, addr three, addr S1
POP ecx

PUSH ecx
invoke lstrcpy, addr S2, addr S1
invoke lstrcpy, addr S1, addr three
POP ecx

invoke lstrcpy, addr retStr1, addr S1
LEA eax, retStr1
RET
ab ENDP
main PROC 
LOCAL S1 [256]:BYTE
LOCAL S2 [256]:BYTE
LOCAL x:SDWORD
LOCAL z:SDWORD
LOCAL res:SDWORD
LOCAL a:SDWORD
LOCAL t:SDWORD
LOCAL h [5]:SDWORD
LOCAL libfunctio:SDWORD
LOCAL y:SDWORD
LOCAL ar [10]:SDWORD
LOCAL pam:SDWORD
LOCAL forstr [256]:BYTE
LOCAL P0[256]:BYTE
LOCAL P1[256]:BYTE

MOV S1[0], 0
MOV S2[0], 0
PUSH 5
POP eax
MOV x, eax
PUSH 0
POP eax
MOV z, eax
PUSH 0
POP eax
MOV res, eax
mov ecx, x
.while ecx > 0
PUSH z
PUSH 100
POP ebx
POP eax
ADD eax, ebx
PUSH eax
PUSH z
PUSH 2
POP ebx
POP eax
ADD eax, ebx
PUSH eax
POP ebx
POP eax
XOR EDX, EDX
IDIV ebx
PUSH eax
POP eax
MOV res, eax
PUSH z
PUSH 1
POP ebx
POP eax
ADD eax, ebx
PUSH eax
POP eax
MOV z, eax
push ecx
INVOKE GetStdHandle, STD_OUTPUT_HANDLE
mov consoleOutHandle1, eax
pushad
mov ebx, res
INVOKE dwtoa, ebx, addr buffer1
INVOKE lstrlen, addr buffer1
mov edx, offset buffer1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
INVOKE lstrlen, addr newline1
mov edx, offset newline1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
popad
pop ecx
dec ecx
.endw
INVOKE integ,4
PUSH eax
POP eax
MOV a, eax
PUSH 0
POP eax
MOV t, eax
mov ecx, 5
.while ecx > 0
PUSH t
PUSH 3
POP ebx
POP eax
XOR EDX, EDX
IMUL ebx
PUSH eax
PUSH 2
POP ebx
POP eax
XOR EDX, EDX
IDIV ebx
PUSH eax
XOR edx, edx
MOV eax, t
PUSH ebx
MOV ebx, 4
MUL ebx
POP ebx
MOV edx, eax
POP eax
MOV h[edx], eax
XOR edx, edx
MOV eax, t
PUSH ebx
MOV ebx, 4
MUL ebx
POP ebx
PUSH h[eax]
POP eax
MOV x, eax
push ecx
INVOKE GetStdHandle, STD_OUTPUT_HANDLE
mov consoleOutHandle1, eax
pushad
mov ebx, x
INVOKE dwtoa, ebx, addr buffer1
INVOKE lstrlen, addr buffer1
mov edx, offset buffer1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
INVOKE lstrlen, addr newline1
mov edx, offset newline1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
popad
pop ecx
PUSH t
PUSH 1
POP ebx
POP eax
ADD eax, ebx
PUSH eax
POP eax
MOV t, eax
dec ecx
.endw
INVOKE sum,addr h,5
PUSH eax
POP eax
MOV libfunctio, eax
push ecx
INVOKE GetStdHandle, STD_OUTPUT_HANDLE
mov consoleOutHandle1, eax
pushad
mov ebx, libfunctio
INVOKE dwtoa, ebx, addr buffer1
INVOKE lstrlen, addr buffer1
mov edx, offset buffer1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
INVOKE lstrlen, addr newline1
mov edx, offset newline1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
popad
pop ecx
INVOKE max,addr h,5
PUSH eax
POP eax
MOV libfunctio, eax
push ecx
INVOKE GetStdHandle, STD_OUTPUT_HANDLE
mov consoleOutHandle1, eax
pushad
mov ebx, libfunctio
INVOKE dwtoa, ebx, addr buffer1
INVOKE lstrlen, addr buffer1
mov edx, offset buffer1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
INVOKE lstrlen, addr newline1
mov edx, offset newline1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
popad
pop ecx
PUSH 3
POP eax
MOV y, eax
PUSH 15
POP eax
MOV ar[0*4], eax
PUSH 2
POP eax
MOV pam, eax
PUSH 6
POP eax
NEG eax
PUSH eax
PUSH 4
POP ebx
POP eax
XOR EDX, EDX
IMUL ebx
PUSH eax
XOR edx, edx
MOV eax, pam
PUSH ebx
MOV ebx, 4
MUL ebx
POP ebx
MOV edx, eax
POP eax
MOV ar[edx], eax
PUSH y
PUSH ar[2*4]
POP ebx
POP eax
ADD eax, ebx
PUSH eax
POP eax
MOV y, eax
push ecx
INVOKE GetStdHandle, STD_OUTPUT_HANDLE
mov consoleOutHandle1, eax
pushad
mov ebx, y
INVOKE dwtoa, ebx, addr buffer1
INVOKE lstrlen, addr buffer1
mov edx, offset buffer1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
INVOKE lstrlen, addr newline1
mov edx, offset newline1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
popad
pop ecx
PUSH ecx
invoke lstrcpy, addr P0, offset main_140
POP ecx
PUSH ecx
invoke lstrcpy, addr P1, offset main_141
POP ecx
INVOKE ab,addr P0,addr P1
PUSH ecx
PUSH eax
invoke lstrcpy, addr S2, addr S1
POP eax
POP ecx
PUSH ecx
invoke lstrcpy, addr S1, eax
POP ecx
PUSH ecx
invoke lstrcpy, addr forstr, addr S1
POP ecx
push ecx
INVOKE GetStdHandle, STD_OUTPUT_HANDLE
mov consoleOutHandle1, eax
pushad
INVOKE lstrlen, addr forstr
INVOKE WriteConsoleA, consoleOutHandle1, addr forstr, eax, offset bytesWritten1, 0
INVOKE lstrlen, addr newline1
mov edx, offset newline1
INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0
popad
pop ecx

PUSH 0

POP eax
RET
main ENDP
end main
