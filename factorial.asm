n_0:
	
	li t0 0
	sw t0 (sp)
	addi sp, sp, -4
result_4:
	
	li t0, 1
	sw t0 (sp)
	addi, sp, sp, -4

	li a7, 5
	ecall
	mv t0, a0
	addi, sp, sp, 8
	sw, t0, (sp)
	addi, sp, sp, -8

while_condition:
	RELATION:
	addi, sp, sp, 8
	lw, t0, (sp)
	addi, sp, sp, -8
	li t1, 0
	ble t0, t1, while_end
while_body:
	
	addi, sp, sp, 4
	lw, t0, (sp)
	addi, sp, sp, -4
	sw t0, (sp)
	addi sp, sp, -4
	addi, sp, sp, 4
	lw t0, (sp)
	
	addi, sp, sp, 8
	lw, t1, (sp)
	addi, sp, sp, -8
	mul t0, t0, t1
	sw t0, (sp)
	addi, sp, sp, -4

	addi, sp, sp, 4
	lw t0, (sp)
	sw, t6, (sp)
	addi, sp, sp, 4
	sw, t0, (sp)
	addi, sp, sp, -4
	
	addi, sp, sp, 8
	lw, t0, (sp)
	addi, sp, sp, -8
	sw t0, (sp)
	addi sp, sp, -4
	addi, sp, sp, 4
	lw t0, (sp)
	
	li t1, 1
	sub t0, t0, t1
	sw t0, (sp)
	addi, sp, sp, -4

	addi, sp, sp, 4
	lw t0, (sp)
	sw, t6, (sp)
	addi, sp, sp, 8
	sw, t0, (sp)
	addi, sp, sp, -8
	
	j while_condition
while_end:
	li a7, 1
	addi, sp, sp, 4
	lw, t0, (sp)
	addi, sp, sp, -4
	mv a0, t0
	ecall
