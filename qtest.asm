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
			lw			$t3, 			_b
			lw			$t4, 			_c
			slt			$t6, 			$t3, 			$t4
			lw			$t3, 			_a
			lw			$t4, 			_b
			add			$t7, 			$t3, 			$t4
			lw			$t3, 			_c
			seq			$t4, 			$t7, 			$t3
			and			$t3, 			$t6, 			$t6
			and			$t4, 			$t5, 			$t5
			beq			$zero, 			$t4, 			L1
			li			$v0, 			4
			la			$a0, 			L2
			syscall
L1:
			lw			$t3, 			_a
			lw			$t5, 			_b
			sgt			$t6, 			$t3, 			$t5
			lw			$t3, 			_b
			lw			$t5, 			_c
			slt			$t7, 			$t3, 			$t5
			lw			$t3, 			_a
			lw			$t5, 			_b
			add			$t8, 			$t3, 			$t5
			lw			$t3, 			_c
			seq			$t5, 			$t8, 			$t3
			and			$t3, 			$t7, 			$t7
			or			$t5, 			$t6, 			$t6
			beq			$zero, 			$t5, 			L3
			li			$v0, 			4
			la			$a0, 			L4
			syscall
L3:
			lw			$t3, 			_a
			lw			$t6, 			_b
			sgt			$t7, 			$t3, 			$t6
			lw			$t3, 			_b
			lw			$t6, 			_c
			sgt			$t8, 			$t3, 			$t6
			or			$t3, 			$t7, 			$t7
			beq			$zero, 			$t3, 			L5
			li			$v0, 			4
			la			$a0, 			L6
			syscall
L5:
			lw			$t6, 			_a
			lw			$t7, 			_b
			sgt			$t8, 			$t6, 			$t7
			lw			$t6, 			_b
			lw			$t7, 			_c
			sgt			$t9, 			$t6, 			$t7
			or			$t6, 			$t8, 			$t8
			nor			$t6, 			$t6, 			$t6
			beq			$zero, 			$t6, 			L7
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
