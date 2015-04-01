
#ifndef CPU_H
#define CPU_H

int cpu_rdtsc();

int cpu_rdtsc_force_vmexit();

int cpu_hv();

void cpu_write_vendor(char *);

int cpu_known_vm_vendors(char *);

#endif
