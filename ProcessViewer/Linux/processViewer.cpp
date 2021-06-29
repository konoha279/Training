#include <iostream>
#include <cstring>
#include <cstdlib>
#include <proc/readproc.h>
int main()
{
  PROCTAB* proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
  proc_t proc_info;
  memset(&proc_info, 0, sizeof(proc_info));
  while (readproc(proc, &proc_info) != NULL) {
	std::cout << "Program: " << proc_info.cmd << std::endl;
	std::cout << "PID: " << proc_info.tid << std::endl;
	std::cout << "PPID: " << proc_info.ppid << std::endl;
	std::cout << "MEM: " << proc_info.resident << std::endl;
	std::cout << "utime: " << proc_info.utime << std::endl;
	std::cout << "stime : " << proc_info.stime << std::endl;
	std::cout << "=============================================================" << std::endl;
  }
  closeproc(proc);
  return 0;
}
