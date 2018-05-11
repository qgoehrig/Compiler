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
			lw			$t2, 			_y
			add			$t3, 			$t4, 			$t2
			sw			$t3, 			_r
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
			li			$t2, 			114
			li			$v0, 			11
			move			$a0, 			$t2
			syscall
			li			$t2, 			61
			li			$v0, 			11
			move			$a0, 			$t2
			syscall
			lw			$t2, 			_r
			li			$v0, 			1
			move			$a0, 			$t2
			syscall
			li			$t2, 			10
			li			$v0, 			11
			move			$a0, 			$t2
			syscall
			jr			$ra
			.data
_r:			.word			0
_x:			.word			0
_y:			.word			0
