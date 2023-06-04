        # qemu -kernel会将内核加载到0x80000000地址且使每个hart(即CPU)跳转到该地址
        # kernel.ld使以下代码放置在0x80000000
.section .text
.global _entry
_entry:
        # 设置一个栈
        # stack0在start.c中做了声明
        # 每个CPU有4096字节栈
        # sp = stack0 + (hartid * 4096)

        # load address
        la sp, stack0

        # load immediate
        li a0, 1024*4

        # csrr（Control and Status Register）指令：读出(Read)机器特权寄存器的值
        csrr a1, mhartid

        # hart id的编号方式是从1开始编号的，而不是从0开始
        addi a1, a1, 1
        mul a0, a0, a1
        add sp, sp, a0
        # 跳转到start.c中的start()
        call start
spin:
        j spin
