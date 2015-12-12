		li 		$t5, 7
		li 		$t6, 3
		mult 	$t5, $t6
		mflo 	$s0
		addi	$s0, $s0, 3

		move 	$a0, $s0
		li 		$v0, 1
		syscall

		li		$v0, 10
		syscall