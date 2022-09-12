.text

.global main

factorial:
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	$0
	movq	16(%rbp), %rax
	pushq	%rax
	pushq	$1
	popq	%rbx
	popq	%rax
	subq	%rbx, %rax
	pushq	%rax
	popq	%rax
	movq	%rax, 16(%rbp)
	movq	16(%rbp), %rax
	pushq	%rax
	pushq	$0
	popq	%rbx
	popq	%rax
	cmpq	%rbx, %rax
	jg		cmp_more_true_1
	jle		cmp_more_false_1
cmp_more_true_1:
	movq	$1, %rax
	jmp		cmp_more_exit_1
cmp_more_false_1:
	movq	$0, %rax
	jmp		cmp_more_exit_1
cmp_more_exit_1:
	pushq	%rax
	popq	%rax
	cmpq	$0, %rax
	je		label_false_1
	movq	24(%rbp), %rax
	pushq	%rax
	movq	24(%rbp), %rax
	pushq	%rax
	pushq	$1
	popq	%rbx
	popq	%rax
	addq	%rbx, %rax
	pushq	%rax
	movq	16(%rbp), %rax
	pushq	%rax
	call	factorial
	addq	$16, %rsp
	pushq	%rax
	popq	%rbx
	popq	%rax
	mulq	%rbx
	pushq	%rax
	popq	%rax
	movq	%rax, 24(%rbp)
label_false_1:
if_exit_label_1:
	movq	24(%rbp), %rax
	movq	%rbp, %rsp
	popq	%rbp
	ret

program:
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	$0
	pushq	$3
	pushq	$1
	pushq	$1
	pushq	$2
	popq	%rbx
	popq	%rax
	mulq	%rbx
	pushq	%rax
	pushq	$2
	popq	%rbx
	popq	%rax
	mulq	%rbx
	pushq	%rax
	movq	-16(%rbp), %rax
	pushq	%rax
	popq	%rbx
	popq	%rax
	addq	%rbx, %rax
	pushq	%rax
	call	factorial
	addq	$16, %rsp
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
	movq	%rbp, %rsp
	popq	%rbp
	ret

main:
	call	program
	movq	$1, %rax
	int		$0x80

.data
	format:	.asciz	"%d\n"
