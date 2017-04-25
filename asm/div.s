.data
.balign 4
number:
	.word -12

.text
.balign 4

.global _start
.global _int_output
.global _int_div

b	_start


_int_output:
	push	{r4, r5, r7, lr}

	cmp	r0, #0
	bgt	.iout_not_negative
	blt	.iout_is_negative

	mov	r0, #48
	str	r0, [sp, #-6]
	mov	r0, #10
	str	r0, [sp, #-5]
	sub	r1, sp, #5

	mov	r7, #4
	mov	r0, #1
	mov	r2, #2

	swi	0

	b	.iout_print_exit

	.iout_is_negative:
	mov	r3, r0

	mov	r0, #45
	str	r0, [sp, #-5]
	sub	r1, r0, #5

	mov	r7, #4
	mov	r0, #1
	mov	r2, #1

	swi	0

	sub	r0, #0, r3

	.iout_not_negative:

	mov	r4, #0

	sub	r3, sp, #30

	b	.ioutconv_test

	.ioutconv_loop:
		mov	r1, #10
		bl	_int_div

		add	r1, r1, #48

		str	r1, [r3]

		add	r3, r3, #1
		add	r4, r4, #1

	.ioutconv_test:
	cmp	r0, #0
	bgt	.ioutconv_loop


	
	mov	r1, r3
	sub	r0, r3, #1

	add	r7, r3, r4

	.ioutinvert_loop:
		ldr	r2, [r0]
		str	r2, [r1]

		add	r1, r1, #1
		sub	r0, r0, #1

	cmp	r7, r1
	bgt	.ioutinvert_loop

	@append a newline character to the end
	add	r4, r4, #1
	mov	r0, #10
	str	r0, [r7]

	mov	r7, #4		@ print to stdout
	mov	r0, #1		@ start index
	mov	r2, r4		@ stop index

	mov	r1, r3		@ call to print
	swi	0

	.iout_print_exit:

	pop	{r4, r5, r7, pc}	@ exit function



_int_div:
	push	{r3, lr}

	mov	r2, #0

	b	.idivloop_test

	.idivloop:
		sub	r0, r0, r1
		add	r2, r2, #1

	.idivloop_test:
	cmp	r0, r1
	bge	.idivloop

	mov	r1, r0
	mov	r0, r2

	pop	{r3, pc}


/*
ENTRY POINT
*/
_start:
	ldr	r0, number_addr
	ldr	r0, [r0]
	
	bl	_int_output

	mov	r7, #1		@ call to exit
	swi	0


number_addr :	.word	number

	.end

