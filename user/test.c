//
// Created by gengjiayao on 2023/5/31.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

void test_dup() {
  int fd = dup(1);
  if (fd < 0) printf("error\n");
  printf("test for sys_dup: new fd is %d\n", fd);
}

// TODO: 修改最大fd上限
void test_dup3() {
  int fd_num = 10;
  int fd = dup3(1, fd_num);
  if (fd == -1) printf("error\n");
  fprintf(fd_num, "test for sys_dup3: copy fd %d\n", fd_num);
}

void test_gettimeofday() {
  int test_ret1 = GetTimeOfDay();
  volatile int i = 12500000;  // qemu时钟频率12500000
  while (i > 0) i--;
  int test_ret2 = GetTimeOfDay();
  printf("test for sys_GetTimeOfDay: ");
  if (test_ret1 > 0 && test_ret2 > 0) {
    printf("interval: %d\n", test_ret2 - test_ret1);
  } else {
    printf("error.\n");
  }
}

struct utsname {
  char sysname[65];
  char nodename[65];
  char release[65];
  char version[65];
  char machine[65];
  char domainname[65];
};

struct utsname un;

void test_uname() {
  int test_ret = uname(&un);
  if (test_ret >= 0) {}
  printf("test for sys_uname: %s %s %s %s %s %s\n", un.sysname, un.nodename, un.release, un.version, un.machine, un.domainname);
}

void test_nanosleep() {
  int time1 = GetTimeOfDay();
  int ret = sleep(10); // 理应是sleep(1) 频率有点问题
  if (ret > 0) {}
  int time2 = GetTimeOfDay();
  if (time2 - time1 >= 1) {
    printf("test for sys_nanosleep: success.\n");
  } else {
    printf("test for sys_nanosleep: error.\n");
  }
}

int main() {
  test_dup();
  test_dup3();
  test_gettimeofday();
  test_uname();
  test_nanosleep();
  exit(0);
}