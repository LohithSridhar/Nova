section .init
	; gcc will nicely put the contents of crtend.o's .init section here.
	pop ebp
	ret

section .fini
	; gcc will nicely put the contents of crtend.o's .fini section here.
	pop ebp
	ret

; crtn.s - ending sections of kernel .init and .fini (basically only for C++ code)