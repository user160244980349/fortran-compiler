.text

.global main

program:
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	$0
	pushq	$5
	movq	-16(%rbp), %rax
	pushq	%rax
	popq	%rsi
	movq	$format, %rdi
	movq	$0, %rax
	call	printf
	movq	$0, %rax
	pushq	$0
	pushq	$2
	pushq	$0
	pushq	$5
	popq	%rbx
	popq	%rax
	subq	%rbx, %rax
	pushq	%rax
	popq	%rbx
	popq	%rax
	mulq	%rbx
	pushq	%rax
	popq	%rbx
	popq	%rax
	subq	%rbx, %rax
	pushq	%rax
	popq	%rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	pushq	%rax
	popq	%rsi
	movq	$format, %rdi
	movq	$0, %rax
	call	printf
	movq	$0, %rax
dowhile_label_1:
	movq	-16(%rbp), %rax
	pushq	%rax
	movq	-8(%rbp), %rax
	pushq	%rax
	popq	%rbx
	popq	%rax
	cmpq	%rbx, %rax
	jg		cmp_less_false_1
	jl		cmp_less_true_1
cmp_less_true_1:
	movq	$1, %rax
	jmp		cmp_less_exit_1
cmp_less_false_1:
	movq	$0, %rax
	jmp		cmp_less_exit_1
cmp_less_exit_1:
	pushq	%rax
	popq	%rax
	cmpq	$0, %rax
	jle		dowhile_exit_label_1
	movq	-16(%rbp), %rax
	pushq	%rax
	pushq	$1
	popq	%rbx
	popq	%rax
	addq	%rbx, %rax
	pushq	%rax
	popq	%rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	pushq	%rax
	pushq	$6
	popq	%rbx
	popq	%rax
	cmpq	%rbx, %rax
	jg		cmp_more_true_2
	jle		cmp_more_false_2
cmp_more_true_2:
	movq	$1, %rax
	jmp		cmp_more_exit_2
cmp_more_false_2:
	movq	$0, %rax
	jmp		cmp_more_exit_2
cmp_more_exit_2:
	pushq	%rax
	popq	%rax
	cmpq	$0, %rax
	je		label_false_1
	jmp		dowhile_exit_label_1
label_false_1:
if_exit_label_1:
	movq	-16(%rbp), %rax
	pushq	%rax
	popq	%rsi
	movq	$format, %rdi
	movq	$0, %rax
	call	printf
	movq	$0, %rax
	jmp		dowhile_label_1
dowhile_exit_label_1:
	movq	%rbp, %rsp
	popq	%rbp
	ret

main:
	call	program
	movq	$1, %rax
	int		$0x80

.data
	format:	.asciz	"%d\n"
