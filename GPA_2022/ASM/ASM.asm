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
	L2 dd 12
	L3 dd 2
	L4 dd 20
	L5 dd 16
	L6 db 'My string1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq' , 0
	L7 db 'My string2' , 0
	L8 dd 10
	L9 dd 1

.DATA

	buffer BYTE 256 dup(0)
	newvalue_inc2 DD 0
	newvalue_inc3 DD 0
	y_start DD 0
	e_start DD 0
	x_start DD 0
	str1_start DD ?
	str2_start DD ?
	str3_start DD ?
	z_start DD 0
	le_start DD 0
	len_start DD 0

.CODE

inc2 PROC value_inc2 : DWORD 

push value_inc2
push value_inc2
pop EAX
pop EBX
add EAX, EBX
push EAX
pop newvalue_inc2

mov eax, newvalue_inc2
ret
inc2 ENDP

inc3 PROC value_inc3 : DWORD 

push value_inc3
push value_inc3
call inc2
push EAX
pop EAX
pop EBX
add EAX, EBX
push EAX
pop newvalue_inc3

mov eax, newvalue_inc3
ret
inc3 ENDP


main PROC
START:

push L1
pop y_start

push L2
push L3
pop EAX
pop EBX
OR EAX, EBX
push EAX
pop e_start

push L4
call inc3
push EAX
push L3
pop EAX
pop EBX
add EAX, EBX
push EAX
push L1
push L5
pop EAX
NOT EAX
push EAX
pop EBX
pop EAX
sub EAX, EBX
push EAX
pop x_start

mov EAX, x_start
push EAX
call ConsoleInt

push offset L6
pop str1_start

push offset L7
pop str2_start

mov EAX, str3_start
push EAX
call ConsoleStr

push L8
pop le_start

push L9
pop z_start

mov EAX, z_start
push EAX
call ConsoleBool


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
mov EBX, L8
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