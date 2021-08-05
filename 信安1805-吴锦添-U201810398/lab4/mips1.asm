.data
_Prompt: .asciiz "Enter an integer:  "
_ret: .asciiz "\n"
.globl main
.text
 li $t7,0x40
 jal main
 li $v0,10
 syscall
read:
  li $v0,4
  la $a0,_Prompt
  syscall
  li $v0,5
  syscall
  jr $ra
write:
  li $v0,1
  syscall
  li $v0,4
  la $a0,_ret
  syscall
  move $v0,$0
  jr $ra

fibo:
  li $t3, 1
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label2
  j label4
label4:
  li $t3, 2
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label2
  j label3
label2:
  li $t3, 1
  sw $t3, 16($sp)
  lw $v0, 16($sp)
  jr $ra
  j label1
label3:
  li $t3, 3
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label5
  j label6
label5:
  li $t3, 2
  sw $t3, 16($sp)
  lw $v0, 16($sp)
  jr $ra
  j label1
label6:
  li $t3, 4
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label7
  j label8
label7:
  li $t3, 3
  sw $t3, 16($sp)
  lw $v0, 16($sp)
  jr $ra
  j label1
label8:
  li $t3, 5
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label9
  j label10
label9:
  li $t3, 5
  sw $t3, 16($sp)
  lw $v0, 16($sp)
  jr $ra
  j label1
label10:
  li $t3, 6
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label11
  j label12
label11:
  li $t3, 8
  sw $t3, 16($sp)
  lw $v0, 16($sp)
  jr $ra
  j label1
label12:
  li $t3, 7
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label13
  j label14
label13:
  li $t3, 13
  sw $t3, 16($sp)
  lw $v0, 16($sp)
  jr $ra
  j label1
label14:
  li $t3, 8
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label15
  j label16
label15:
  li $t3, 21
  sw $t3, 16($sp)
  lw $v0, 16($sp)
  jr $ra
  j label1
label16:
  li $t3, 9
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label17
  j label18
label17:
  li $t3, 34
  sw $t3, 16($sp)
  lw $v0, 16($sp)
  jr $ra
  j label1
label18:
  li $t3, 10
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  beq $t1,$t2,label19
  j label20
label19:
  li $t3, 55
  sw $t3, 16($sp)
  lw $v0, 16($sp)
  jr $ra
  j label1
label20:
  li $t3, 2
  sw $t3, 16($sp)
  lw $t1, 12($sp)
  lw $t2, 16($sp)
  sub $t3,$t1,$t2
  sw $t3, 20($sp)
  move $t0,$sp
  addi $sp, $sp, -44
  sw $ra,0($sp)
  lw $t1, 20($t0)
  move $t3,$t1
  sw $t3,12($sp)
  jal fibo
  lw $ra,0($sp)
  addi $sp,$sp,44
  sw $v0,28($sp)
  li $t3, 1
  sw $t3, 28($sp)
  lw $t1, 12($sp)
  lw $t2, 28($sp)
  sub $t3,$t1,$t2
  sw $t3, 32($sp)
  move $t0,$sp
  addi $sp, $sp, -44
  sw $ra,0($sp)
  lw $t1, 32($t0)
  move $t3,$t1
  sw $t3,12($sp)
  jal fibo
  lw $ra,0($sp)
  addi $sp,$sp,44
  sw $v0,40($sp)
  lw $t1, 28($sp)
  lw $t2, 40($sp)
  add $t3,$t1,$t2
  sw $t3, 40($sp)
  lw $v0, 40($sp)
  jr $ra
label1:

main:
  addi $sp, $sp, -28
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal read
  lw $ra,0($sp)
  addi $sp, $sp, 4
  sw $v0, 28($sp)
  lw $t1, 28($sp)
  move $t3, $t1
  sw $t3, 12($sp)
  li $t3, 1
  sw $t3, 24($sp)
  lw $t1, 24($sp)
  move $t3, $t1
  sw $t3, 20($sp)
label26:
  lw $t1, 20($sp)
  lw $t2, 12($sp)
  blt $t1,$t2,label25
  j label24
label25:
  move $t0,$sp
  addi $sp, $sp, -44
  sw $ra,0($sp)
  lw $t1, 20($t0)
  move $t3,$t1
  sw $t3,12($sp)
  jal fibo
  lw $ra,0($sp)
  addi $sp,$sp,44
  sw $v0,28($sp)
  lw $t1, 28($sp)
  move $t3, $t1
  sw $t3, 16($sp)
  lw $a0, 16($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
  lw $t1,20($sp)
  add $t1,$t1,1
  sw $t1,20($sp)
  j label26
label24:
  li $t3, 1
  sw $t3, 24($sp)
  lw $v0, 24($sp)
  jr $ra
label21: