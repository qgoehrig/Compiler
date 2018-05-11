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
			sw			$t0, 			_p
L10:
			lw			$t1, 			_p
			li			$t2, 			1
			sgt			$t3, 			$t1, 			$t2
			beq			$zero, 			$t3, 			L2
			li			$v0, 			4
			la			$a0, 			L3
			syscall
			lw			$t4, 			_p
			li			$v0, 			1
			move			$a0, 			$t4
			syscall
			li			$v0, 			4
			la			$a0, 			L4
			syscall
			li			$t4, 			2
			sw			$t4, 			_d
L8:
			lw			$t5, 			_d
			lw			$t6, 			_p
			li			$t7, 			2
			div			$t8, 			$t6, 			$t7
			slt			$t6, 			$t5, 			$t8
			beq			$zero, 			$t6, 			L5
			lw			$t7, 			_p
			lw			$t9, 			_d
			div			$s0, 			$t7, 			$t9
			sw			$s0, 			_q
			lw			$t7, 			_q
			lw			$t9, 			_d
			mul			$s1, 			$t7, 			$t9
			lw			$t7, 			_p
			seq			$t9, 			$s1, 			$t7
			beq			$zero, 			$t9, 			L6
			lw			$s2, 			_d
			li			$v0, 			1
			move			$a0, 			$s2
			syscall
			li			$v0, 			4
			la			$a0, 			L7
			syscall
L6:
			lw			$s2, 			_d
			li			$s3, 			1
			add			$s4, 			$s2, 			$s3
			sw			$s4, 			_d
			b			L8
L5:
			li			$v0, 			4
			la			$a0, 			L9
			syscall
			li			$v0, 			4
			la			$a0, 			L1
			syscall
			li			$v0, 			11
			li			$v0, 			5
			syscall
			move			$s2, 			$v0
			sw			$s2, 			_p
			b			L10
L2:
			jr			$ra
			.data
_d:			.word			0
_p:			.word			0
_q:			.word			0
L7:			.asciiz			" "
L4:			.asciiz			" are "
L1:			.asciiz			"> "
L9:			.asciiz			"\n"
L3:			.asciiz			"factors of "
