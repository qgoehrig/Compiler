			.text
			.globl			__start
__start:
			jal			_main
			li			$v0, 			10
			syscall
_main:
			li			$v0, 			4
			la			$a0, 			L1
			syscall
			li			$v0, 			11
			li			$v0, 			5
			syscall
			move			$t0, 			$v0
			sw			$t0, 			_x
			li			$v0, 			4
			la			$a0, 			L2
			syscall
			lw			$t1, 			_x
			li			$v0, 			1
			move			$a0, 			$t1
			syscall
			li			$v0, 			4
			la			$a0, 			L3
			syscall
			li			$t1, 			1
			sw			$t1, 			_fac
L5:
			lw			$t2, 			_x
			li			$t3, 			0
			sub			$t5, 			$t2, 			$t3
			slt			$t4, 			$zero, 			$t5
			beq			$zero, 			$t4, 			L4
			lw			$t5, 			_fac
			lw			$t6, 			_x
			mul			$t7, 			$t5, 			$t6
			sw			$t7, 			_fac
			lw			$t5, 			_x
			li			$t6, 			1
			sub			$t8, 			$t5, 			$t6
			sw			$t8, 			_x
			b			L5
L4:
			li			$v0, 			4
			la			$a0, 			L6
			syscall
			lw			$t5, 			_fac
			li			$v0, 			1
			move			$a0, 			$t5
			syscall
			li			$v0, 			4
			la			$a0, 			L3
			syscall
			jr			$ra
			.data
_x:			.word			0
_fac:			.word			0
L2:			.asciiz			"x = "
L6:			.asciiz			"factorial = "
L1:			.asciiz			"> "
L3:			.asciiz			"\n"
