
#ifndef VBOX_H
#define VBOX_H

int vbox_reg_key1();
int vbox_reg_key2();
int vbox_reg_key3();
int vbox_reg_key4();
int vbox_reg_key5();
int vbox_reg_key7();
int vbox_reg_key8();
int vbox_reg_key9(int writelogs);
int vbox_reg_key10();

int vbox_sysfile1(int writelogs);
int vbox_sysfile2(int writelogs);

int vbox_mac();

int vbox_devices(int writelogs);

int vbox_traywindow();

int vbox_network_share();

int vbox_processes(int writelogs);

int vbox_wmi_devices();

#endif
