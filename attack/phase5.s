popq %rax
movl %eax,%edx
movl %edx,%ecx
movl %ecx,%esi
movq %rsp,%rax
movq %rax,%rdi
callq add_xy
movq %rax,%rdi

