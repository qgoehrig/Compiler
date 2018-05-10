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
			ble			$t2, 			$t3, 			L4
			lw			$t4, 			_fac
			lw			$t5, 			_x
			mul			$t6, 			$t4, 			$t5
			sw			$t6, 			_fac
			lw			$t7, 			_x
			li			$t8, 			1
			sub			$t9, 			$t7, 			$t8
			sw			$t9, 			_x
			b			L5
L4:
			li			$v0, 			4
			la			$a0, 			L6
			syscall
			lw			$s0, 			_fac
			li			$v0, 			1
			move			$a0, 			$s0
			syscall
			li			$v0, 			4
			la			$a0, 			_"\n"
			syscall
			jr			$ra
			.data
_x:			.word			0
_fac:			.word			0
__iobuf:			.space			4
_"x = ":			.asciiz			"x = "
_"factorial = ":			.asciiz			"factorial = "
_"> ":			.asciiz			"> "
_"\n":			.asciiz			"\n"
