.text

.global main
main:
mov		(var_a), %rax
push		%rax
pop		%rax
lea		format(%rip), %rdi
mov		%rax, %rsi
call		printf
push		$0
push		$2
push		$5
pop		%rbx
pop		%rax
imul		%rbx
push		%rax
pop		%rbx
pop		%rax
sub		%rbx, %rax
push		%rax
pop		%rax
mov		%rax, (var_b)
mov		(var_b), %rax
push		%rax
pop		%rax
lea		format(%rip), %rdi
mov		%rax, %rsi
call		printf
dowhile_label_1:
mov		(var_a), %rax
push		%rax
mov		(var_b), %rax
push		%rax
push		$5
pop		%rbx
pop		%rax
add		%rbx, %rax
push		%rax
pop		%rbx
pop		%rax
cmp		%rbx, %rax
jg		cmp_more_true_1
jle		cmp_more_false_1
cmp_more_true_1:
mov $1, %rax
jmp cmp_more_exit_1
cmp_more_false_1:
mov $0, %rax
jmp cmp_more_exit_1
cmp_more_exit_1:
push		%rax
pop		%rax
cmp		$0, %rax
jle		dowhile_exit_label_1
mov		(var_a), %rax
push		%rax
push		$1
pop		%rbx
pop		%rax
sub		%rbx, %rax
push		%rax
pop		%rax
mov		%rax, (var_a)
mov		(var_a), %rax
push		%rax
push		$0
pop		%rbx
pop		%rax
cmp		%rbx, %rax
jg		cmp_more_true_2
jle		cmp_more_false_2
cmp_more_true_2:
mov $1, %rax
jmp cmp_more_exit_2
cmp_more_false_2:
mov $0, %rax
jmp cmp_more_exit_2
cmp_more_exit_2:
push		%rax
pop		%rax
cmp		$0, %rax
je		label_false_1
jmp		dowhile_label_1
label_false_1:
if_exit_label_1:
mov		(var_a), %rax
push		%rax
pop		%rax
lea		format(%rip), %rdi
mov		%rax, %rsi
call		printf
jmp		dowhile_label_1
dowhile_exit_label_1:
exit:
mov		$1, %rax
int		$0x80

.data
format:	.asciz "%d\n"
var_b:		.quad	0
var_a:		.quad	5
