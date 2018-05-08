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
			lw			$t2, 			_x
			lw			$t3, 			_x
			mul			$t4, 			$t2, 			$t3
			lw			$t5, 			_y
			add			$t6, 			$t2, 			$t5
			sw			$t2, 			_r
			li			$t7, 			120
			li			$v0, 			11
			move			$a0, 			$t7
			syscall
			li			$t7, 			61
			li			$v0, 			11
			move			$a0, 			$t7
			syscall
			lw			$t7, 			_x
			li			$v0, 			1
			move			$a0, 			$t7
			syscall
			li			$t7, 			10
			li			$v0, 			11
			move			$a0, 			$t7
			syscall
			li			$t7, 			121
			li			$v0, 			11
			move			$a0, 			$t7
			syscall
			li			$t7, 			61
			li			$v0, 			11
			move			$a0, 			$t7
			syscall
			lw			$t7, 			_y
			li			$v0, 			1
			move			$a0, 			$t7
			syscall
			li			$t7, 			10
			li			$v0, 			11
			move			$a0, 			$t7
			syscall
			li			$t7, 			114
			li			$v0, 			11
			move			$a0, 			$t7
			syscall
			li			$t7, 			61
			li			$v0, 			11
			move			$a0, 			$t7
			syscall
			lw			$t7, 			_r
			li			$v0, 			1
			move			$a0, 			$t7
			syscall
			li			$t7, 			10
			li			$v0, 			11
			move			$a0, 			$t7
			syscall
			jr			$ra
			.data
_r:			.word			0
_x:			.word			0
_y:			.word			0
