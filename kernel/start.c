#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

void main();
void timerinit();

// entry.S需要每个CPU一个栈
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

// 每个CPU有一个存储空间用于机器模式定时器中断
uint64 timer_scratch[NCPU][5];

// kernelvec.S中的机器模式定时器中断汇编代码
extern void timervec();

// entry.S在stack0上使用机器模式跳转到这里
void
start()
{
  // 写控制与状态寄存器，为了在mret时切换到管理模式
  unsigned long x = r_mstatus();
  x &= ~MSTATUS_MPP_MASK;
  x |= MSTATUS_MPP_S;
  w_mstatus(x);

  // 将异常指令的PC保存为main函数地址，为了mret跳转
  // requires gcc -mcmodel=medany
  w_mepc((uint64)main);

  // 现在禁用分页.
  w_satp(0);

  // 将所有中断和例外委托给管理模式（设置1就是将对应中断或异常委托给S模式的处理程序）
  w_medeleg(0xffff); // Machine Exception Delegation
  w_mideleg(0xffff); // Machine Interrupt Delegation
  // Supervisor Interrupt Enable S模式下的中断使能（是否屏蔽）
  w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);

  // 配置外存保护，让管理模式可以访问所有的物理内存
  w_pmpaddr0(0x3fffffffffffffull);
  w_pmpcfg0(0xf); // 指定范围的顶部为全部物理空间

  // ask for clock interrupts.
  timerinit();

  // 让每个CPU的hartid保存在它的tp寄存器中，用于cpuid()
  int id = r_mhartid();
  w_tp(id);

  // 切换到管理模式，跳转到main()
  asm volatile("mret");
}

// arrange to receive timer interrupts.
// they will arrive in machine mode at
// at timervec in kernelvec.S,
// which turns them into software interrupts for
// devintr() in trap.c.
void
timerinit()
{
  // each CPU has a separate source of timer interrupts.
  int id = r_mhartid();

  // ask the CLINT for a timer interrupt.
  int interval = 1000000; // cycles; about 1/10th second in qemu.
  *(uint64*)CLINT_MTIMECMP(id) = *(uint64*)CLINT_MTIME + interval;

  // prepare information in scratch[] for timervec.
  // scratch[0..2] : space for timervec to save registers.
  // scratch[3] : address of CLINT MTIMECMP register.
  // scratch[4] : desired interval (in cycles) between timer interrupts.
  uint64 *scratch = &timer_scratch[id][0];
  scratch[3] = CLINT_MTIMECMP(id);
  scratch[4] = interval;
  w_mscratch((uint64)scratch);

  // set the machine-mode trap handler.
  w_mtvec((uint64)timervec);

  // enable machine-mode interrupts.
  w_mstatus(r_mstatus() | MSTATUS_MIE);

  // enable machine-mode timer interrupts.
  w_mie(r_mie() | MIE_MTIE);
}
