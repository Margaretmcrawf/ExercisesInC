	.file	"cards.c"
	.section	.rodata
.LC0:
	.string	"Enter the card name: "
.LC1:
	.string	"%2s"
.LC2:
	.string	"That value doesn't work"
.LC3:
	.string	"Current count: %i\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -28(%rbp)
	movl	$5, -20(%rbp)
.L12:
	movl	$.LC0, %edi
	call	puts
	leaq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	movl	$0, -24(%rbp)
	movzbl	-16(%rbp), %eax
	movsbl	%al, %eax
	subl	$65, %eax
	cmpl	$23, %eax
	ja	.L2
	movl	%eax, %eax
	movq	.L4(,%rax,8), %rax
	jmp	*%rax
	.section	.rodata
	.align 8
	.align 4
.L4:
	.quad	.L3
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L5
	.quad	.L5
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L5
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L2
	.quad	.L15
	.text
.L5:
	movl	$10, -24(%rbp)
	jmp	.L7
.L3:
	movl	$11, -24(%rbp)
	jmp	.L7
.L2:
	leaq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	atoi
	movl	%eax, -24(%rbp)
	cmpl	$0, -24(%rbp)
	jle	.L9
	cmpl	$10, -24(%rbp)
	jle	.L7
.L9:
	movl	$.LC2, %edi
	call	puts
	jmp	.L8
.L7:
	cmpl	$2, -24(%rbp)
	jle	.L10
	cmpl	$6, -24(%rbp)
	jg	.L10
	addl	$1, -28(%rbp)
	jmp	.L11
.L10:
	cmpl	$10, -24(%rbp)
	jne	.L11
	subl	$1, -28(%rbp)
.L11:
	movl	-28(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	jmp	.L8
.L15:
	nop
.L8:
	movzbl	-16(%rbp), %eax
	cmpb	$88, %al
	jne	.L12
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L14
	call	__stack_chk_fail
.L14:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
