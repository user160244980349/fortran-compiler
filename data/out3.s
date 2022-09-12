.text

.global main
main:
push		$1
pop		%rax
mov		%rax, (var_a)
push		$10
pop		%rax
mov		%rax, (var_b)
dowhile_label_1:
mov		(var_a), %rax
push		%rax
mov		(var_b), %rax
push		%rax
pop		%rbx
pop		%rax
cmp		%rbx, %rax
jge		cmp_less_false_1
jl		cmp_less_true_1
cmp_less_true_1:
mov $1, %rax
jmp cmp_less_exit_1
cmp_less_false_1:
mov $0, %rax
jmp cmp_less_exit_1
cmp_less_exit_1:
push		%rax
pop		%rax
cmp		$0, %rax
jle		dowhile_exit_label_1
mov		(var_a), %rax
push		%rax
pop		%rax
lea		format(%rip), %rdi
mov		%rax, %rsi
call		printf
mov		(var_a), %rax
push		%rax
push		$0
push		$1
pop		%rbx
pop		%rax
sub		%rbx, %rax
push		%rax
pop		%rbx
pop		%rax
imul		%rbx
push		%rax
pop		%rax
mov		%rax, (var_a)
mov		(var_a), %rax
push		%rax
push		$0
pop		%rbx
pop		%rax
cmp		%rbx, %rax
jge		cmp_less_false_2
jl		cmp_less_true_2
cmp_less_true_2:
mov $1, %rax
jmp cmp_less_exit_2
cmp_less_false_2:
mov $0, %rax
jmp cmp_less_exit_2
cmp_less_exit_2:
push		%rax
pop		%rax
cmp		$0, %rax
je		label_false_1
mov		(var_a), %rax
push		%rax
push		$1
pop		%rbx
pop		%rax
sub		%rbx, %rax
push		%rax
pop		%rax
mov		%rax, (var_a)
jmp		if_exit_label_1
label_false_1:
mov		(var_a), %rax
push		%rax
push		$1
pop		%rbx
pop		%rax
add		%rbx, %rax
push		%rax
pop		%rax
mov		%rax, (var_a)
if_exit_label_1:
jmp		dowhile_label_1
dowhile_exit_label_1:
exit:
mov		$1, %rax
int		$0x80

.data
format:	.asciz "%d\n"
var_a:		.quad	0
var_b:		.quad	0
