			.text
			.globl			__start
__start:
			jal			_main
			li			$v0, 			10
			syscall
_main:
			li			$t0, 			66
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			69
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			71
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			73
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			78
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			10
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$v0, 			4
			la			$a0, 			L1
			syscall
			li			$t0, 			103
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			61
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			lw			$t0, 			_x
			li			$v0, 			1
			move			$a0, 			$t0
			syscall
			li			$t0, 			10
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			jr			$ra
			.data
_x:			.word			0
_y:			.word			0
_z:			.word			0
__iobuf:			.space			4
L1:			.asciiz			"Hello\n"
