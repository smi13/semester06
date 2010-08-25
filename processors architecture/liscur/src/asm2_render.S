	.text
        
	.global	asm2_put_pixel
	.type	asm2_put_pixel, %function
@ x, y, data, bytes_per_line
asm2_put_pixel:
	stmfd sp!, {r1 - r10, fp, ip, lr, pc}
	mul r4, r2, r3
        add r0, r4
	add r0, r1
        mov r6, #-1
	strb r6, [r0] 
	ldmfd sp, {r1 - r10, fp, sp, pc}	
	bx lr
	
	.global	asm2_put_line
	.type	asm2_put_line, %function
@ x0, y0, x1, y1, data, bytes_per_line - modified assembly (loop 3-unwinding & no pushes in cycles)
asm2_put_line:
	mov ip, sp
	stmfd sp!, {r1 - r10, fp, ip, lr, pc}
	
	sub r4, r3, r1		@ r4 = y1 - y0
	sub r5, r2, r0		@ r5 = x1 - x0

	mov r6, #-1

	cmp r4, #0
	mulmi r4, r6	
				@ r4 = abs(y1 - y0)
	cmp r5, #0
	mulmi r5, r6
				@ r5 = abs(x1 - x0)

	cmp r4, r5

	eorgt r0, r0, r1 
	eorgt r1, r1, r0
	eorgt r0, r0, r1
				@ swap x0, y0

	eorgt r2, r2, r3 
	eorgt r3, r3, r2
	eorgt r2, r2, r3
				@ swap x1, y1
	movgt r6, #1  
	movlt r6, #-1
				@ r6 = steep = 1 or -1

	cmp r0, r2
	movgt r7, #-1
	movlt r7, #1
	subgt r4, r0, r2
	sublt r4, r2, r0
				@ r4 = deltax, r7 = xstep
	cmp r3, r1
	movlt r8, #-1
	movgt r8, #1
	sublt r5, r1, r3
	subgt r5, r3, r1
				@ r5 = deltay, r8 = ystep

	mov r9, r0
	mov r10, r1
				@ r9 = x0, r10 = y0
				@ x0(r0), y1(r3), y0(r1) are no longer needed

	ldr r1, [ip]		@ r1 = data
	ldr r3, [ip, #4]	@ r3 = bytes_per_line

	mov r11, r4
				@ r11 = error

	mov r0, #-1

	cmp r6, #0
	bmi for_steep_negative
	bpl for_steep_positive

for_steep_positive:

	mul r6, r9, r3 
	add r6, r10

	strb r0, [r1, r6]

	sub r11, r5 		@ error -= deltay

	cmp r11, #0             @ if (error < 0)
	addmi r10, r8		@ y += ystep
	addmi r11, r4		@ error += deltax

	cmp r9, r2          	@ if (x != x1)
	addne r9, r7		@  x += xstep, continue
	
	mul r6, r9, r3 
	add r6, r10

	strb r0, [r1, r6]

	sub r11, r5 		@ error -= deltay

	cmp r11, #0             @ if (error < 0)
	addmi r10, r8		@ y += ystep
	addmi r11, r4		@ error += deltax

	cmp r9, r2          	@ if (x != x1)
	addne r9, r7		@  x += xstep, continue

	mul r6, r9, r3 
	add r6, r10

	strb r0, [r1, r6]

	sub r11, r5 		@ error -= deltay

	cmp r11, #0             @ if (error < 0)
	addmi r10, r8		@ y += ystep
	addmi r11, r4		@ error += deltax

	cmp r9, r2          	@ if (x != x1)
	addne r9, r7		@  x += xstep, continue
	
	bne for_steep_positive

for_steep_negative:

	mul r6, r10, r3 
	add r6, r9

	strb r0, [r1, r6]

	sub r11, r5 		@ error -= deltay

	cmp r11, #0             @ if (error < 0)
	addmi r10, r8		@ y += ystep
	addmi r11, r4		@ error += deltax

	cmp r9, r2          	@ if (x != x1)
	addne r9, r7		@  x += xstep, continue

	mul r6, r10, r3 
	add r6, r9

	strb r0, [r1, r6]

	sub r11, r5 		@ error -= deltay

	cmp r11, #0             @ if (error < 0)
	addmi r10, r8		@ y += ystep
	addmi r11, r4		@ error += deltax

	cmp r9, r2          	@ if (x != x1)
	addne r9, r7		@  x += xstep, continue
	
	mul r6, r10, r3 
	add r6, r9

	strb r0, [r1, r6]

	sub r11, r5 		@ error -= deltay

	cmp r11, #0             @ if (error < 0)
	addmi r10, r8		@ y += ystep
	addmi r11, r4		@ error += deltax

	cmp r9, r2          	@ if (x != x1)
	addne r9, r7		@  x += xstep, continue

	bne for_steep_negative
	
	ldmfd sp, {r1 - r10, fp, sp, pc}	
