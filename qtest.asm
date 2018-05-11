			.text
			.globl			__start
__start:
			jal			_main
			li			$v0, 			10
			syscall
_main:
			li			$t0, 			1
			sw			$t0, 			_a
			li			$t1, 			2
			sw			$t1, 			_b
			li			$t2, 			3
			sw			$t2, 			_c
			lw			$t3, 			_a
			lw			$t4, 			_b
			slt			$t5, 			$t3, 			$t4
			lw			$t6, 			_b
			lw			$t7, 			_c
			slt			$t8, 			$t6, 			$t7
			lw			$t9, 			_a
			lw			$s0, 			_b
			add			$s1, 			$t9, 			$s0
			lw			$t9, 			_c
			sub			$s2, 			$s1, 			$t9
			slt			$s2, 			$s2, 			$zero
			sub			$s3, 			$t9, 			$s1
			slt			$s3, 			$s3, 			$zero
			nor			$s0, 			$s2, 			$s3
			and			$s2, 			$t8, 			$t8
			and			$s3, 			$t5, 			$t5
			beq			$zero, 			$s3, 			L1
			li			$v0, 			4
			la			$a0, 			L2
			syscall
L1:
			lw			$s4, 			_a
			lw, 			_b
			sub, 			$s4
			slt, 			$zero
			lw, 			_b
			lw, 			_c
			slt
			lw, 			_a
			lw, 			_b
			add
			lw, 			_c
			sub
			slt, 			$zero
			sub
			slt, 			$zero
			nor
			and
			or
			beq			$zero, 			L3
			li			$v0, 			4
			la			$a0, 			L4
			syscall
L3:
			lw, 			_a
			lw, 			_b
			sub
			slt, 			$zero
			lw, 			_b
			lw, 			_c
			sub
			slt, 			$zero
			or
			beq			$zero, 			L5
			li			$v0, 			4
			la			$a0, 			L6
			syscall
L5:
			lw, 			_a
			lw, 			_b
			sub
			slt, 			$zero
			lw, 			_b
			lw, 			_c
			sub
			slt, 			$zero
			or
			nor
			beq			$zero, 			L7
			li			$v0, 			4
			la			$a0, 			L8
			syscall
L7:
			jr			$ra
			.data
_a:			.word			0
_b:			.word			0
_c:			.word			0
L2:			.asciiz			"first true\n"
L4:			.asciiz			"second true\n"
L6:			.asciiz			"third true\n"
L8:			.asciiz			"forth true\n"
