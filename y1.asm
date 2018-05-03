			.text
			.globl			__start
__start:
			jal			_main
			li			$v0, 			10
			syscall
_main:			.word			0
			.data
_x:			.word			0
_y:			.word			0
_z:			.word			0
_ab:			.word			0
_bcd:			.word			0
