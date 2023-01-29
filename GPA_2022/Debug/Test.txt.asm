.586P
.MODEL FLAT, STDCALL
includelib kernel32.lib
includelib libucrt.lib
includelib ..\Debug\Static_Library.lib
EXTRN Strcmp: PROC
EXTRN Strcat: PROC
EXTRN Strcpy: PROC
EXTRN Strlen: PROC
EXTRN ConsoleInt: PROC
EXTRN ConsoleStr: PROC
EXTRN ConsoleBool: PROC
EXTRN consolpause: proc
ExitProcess PROTO : DWORD

.STACK 4096
.CONST

	L1 dd 15
	L2 dd 20
	L3 dd 2
	L4 dd 16
	L5 db 'This my string' , 0
	L6 db 'This string' , 0
	L7 dd 10

.DATA

	buffer BYTE 256 dup(0)
	newvalue_pow2 DD 0
	newvalue_pow3 DD 0
	y_start DD 0
	x_start DD 0
	str1_start DD ?
	str2_start DD ?
	z_start DD 0
	len_start DD 0

.CODE

pow2 PROC value_pow2 : DWORD 

push value_pow2
push value_pow2
pop EAX
pop EBX
add EAX, EBX
push EAX
pop newvalue_pow2

mov eax, newvalue_pow2
ret
pow2 ENDP

pow3 PROC value_pow3 : DWORD 

push value_pow3
push value_pow3
call pow2
push EAX
pop EAX
pop EBX
add EAX, EBX
push EAX
pop newvalue_pow3

mov eax, newvalue_pow3
ret
pow3 ENDP


main PROC
START:

push L1
pop y_start

push L2
call pow3
push EAX
push L3
pop EAX
pop EBX
add EAX, EBX
push EAX
push L1
push L4
pop EAX
pop EBX
OR EAX, EBX
push EAX
pop EBX
pop EAX
sub EAX, EBX
push EAX
pop x_start

mov EAX, x_start
push EAX
call ConsoleInt

push offset L5
pop str1_start

push offset L6
pop str2_start

push str1_start
push str2_start
call strcmp
push EAX
pop z_start


mov EAX,z_start
mov EBX, 1
sub EAX, EBX
je true1
jmp exit1

true1:
push str1_start
call strlen
push EAX
pop len_start


mov EAX,len_start
mov EBX, L7
sub EAX, EBX
je true2
jmp exit2

true2:
mov EAX, len_start
push EAX
call ConsoleInt


exit2:
mov EAX, str2_start
push EAX
call ConsoleStr


exit1:
push 0
call consolpause
call ExitProcess
main ENDP
end main