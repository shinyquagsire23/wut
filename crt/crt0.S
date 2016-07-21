.extern main
.extern exit
.global _start
_start:
	bl main
	b exit
	
.global __init
__init:
	blr
