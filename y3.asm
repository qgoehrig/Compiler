			.text
			.globl			__start
__start:
			jal			_main
			li			$v0, 			10
			syscall
_main:
			li			$t0, 			62
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$v0, 			11
			li			$v0, 			5
			syscall
			move			$t0, 			$v0
			sw			$t0, 			_x
			li			$t1, 			62
			li			$v0, 			11
			move			$a0, 			$t1
			syscall
			li			$v0, 			11
			li			$v0, 			5
			syscall
			move			$t1, 			$v0
			sw			$t1, 			_y
			li			$t2, 			120
			li			$v0, 			11
			move			$a0, 			$t2
			syscall
			li			$t2, 			61
			li			$v0, 			11
			move			$a0, 			$t2
			syscall
			lw			$t2, 			_x
			li			$v0, 			1
			move			$a0, 			$t2
			syscall
			li			$t2, 			10
			li			$v0, 			11
			move			$a0, 			$t2
			syscall
			li			$t2, 			121
			li			$v0, 			11
			move			$a0, 			$t2
			syscall
			li			$t2, 			61
			li			$v0, 			11
			move			$a0, 			$t2
			syscall
			lw			$t2, 			_y
			li			$v0, 			1
			move			$a0, 			$t2
			syscall
			li			$t2, 			10
			li			$v0, 			11
			move			$a0, 			$t2
			syscall
L4:
			lw			$t2, 			_x
			lw			$t3, 			_y
			seq			$t4, 			$t2, 			$t3
			beq			$zero, 			$t4, 			L1
			li			$t5, 			10
			li			$v0, 			11
			move			$a0, 			$t5
			syscall
			lw			$t5, 			_x
			lw			$t6, 			_y
			slt			$t7, 			$t5, 			$t6
			beq			$zero, 			$t7, 			L2
			lw			$t8, 			_y
			lw			$t9, 			_x
			sub			$s0, 			$t8, 			$t9
			sw			$s0, 			_y
			b			L3
L2:
			lw			$t8, 			_x
			lw			$t9, 			_y
			sub			$s1, 			$t8, 			$t9
			sw			$s1, 			_x
L3:
			b			L4
L1:
			li			$t8, 			103
			li			$v0, 			11
			move			$a0, 			$t8
			syscall
			li			$t8, 			61
			li			$v0, 			11
			move			$a0, 			$t8
			syscall
			lw			$t8, 			_x
			li			$v0, 			1
			move			$a0, 			$t8
			syscall
			li			$t8, 			10
			li			$v0, 			11
			move			$a0, 			$t8
			syscall
			jr			$ra
			.data
_x:			.word			0
_y:			.word			0
