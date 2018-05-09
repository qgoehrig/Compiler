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
			beq			$t2, 			$t3, 			L1
			lw			$t4, 			_x
			lw			$t5, 			_y
			bge			$t4, 			$t5, 			L2
			lw			$t6, 			_y
			lw			$t7, 			_x
			sub			$t8, 			$t6, 			$t7
			sw			$t8, 			_y
			b			L3
L2:
			lw			$t9, 			_x
			lw			$s0, 			_y
			sub			$s1, 			$t9, 			$s0
			sw			$s1, 			_x
L3:
			b			L4
L1:
			li			$s2, 			103
			li			$v0, 			11
			move			$a0, 			$s2
			syscall
			li			$s2, 			61
			li			$v0, 			11
			move			$a0, 			$s2
			syscall
			lw			$s2, 			_x
			li			$v0, 			1
			move			$a0, 			$s2
			syscall
			li			$s2, 			10
			li			$v0, 			11
			move			$a0, 			$s2
			syscall
			jr			$ra
			.data
_x:			.word			0
_y:			.word			0
