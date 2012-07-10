#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#include "debuggers.h"
#include "sandboxie.h"
#include "vbox.h"
#include "vmware.h"

/*
  Pafish (Paranoid fish)
  
  All code from this project, including
  functions, procedures and the main program
  is licensed under GNU/GPL version 3.
  
  So, if you are going to use functions or
  procedures from this project to develop
  your malware, you have to release the
  source code as well :)
  
  Contributors:
   - Alberto Ortega (alberto[at]pentbox[dot]net)
   - Moshe Zioni aka dalmoz (zimoshe[at]gmail[dot]com)
   
  
*/

int main(int argc, char *argv[])
{
    char icon[] = "Blue fish icon thanks to http://www.fasticon.com/";
    
    init_cmd_colors();
    print_header();
    
    printf("[*] Running checks ...\n");
    
    /* Debuggers detection tricks */
    printf("\n[-] Debuggers detection\n");
    printf("[*] Using IsDebuggerPresent() ... ");
    if (debug_isdebuggerpresent() == 0)
        print_traced();
    else
        print_not_traced();
    printf("[*] Using OutputDebugString() ... ");
    if (debug_outputdebugstring() == 0)
        print_traced();
    else
        print_not_traced();
    
    /* Sandboxie detection tricks */
    printf("\n[-] Sandboxie detection\n");
    printf("[*] Using sbiedll.dll ... ");
    if (sboxie_detect_sbiedll() == 0)
        print_traced();
    else
        print_not_traced();
    printf("[*] Looking for default working directory ... ");
    if (sboxie_detect_work_dir() == 0)
        print_suspicious();
    else
        print_not_traced();
    
    /* VirtualBox detection tricks */
    printf("\n[-] VirtualBox detection\n");
    printf("[*] Using RegOpenKey w/ known Guest additions key ... ");
    if (vbox_gadd_reg_key() == 0)
        print_traced();
    else
        print_not_traced();
        
    /* VMware detection tricks */
    printf("\n[-] VMware detection\n");
    printf("[*] Looking for default vmtools working directory ... ");
    if (vmtools_detect_work_dir()) == 0)
        print_traced();
    else
        print_not_traced();


    printf("\n\n");
    printf("[-] Finished, feel free to RE me.");
    fflush(stdin); getchar();
    return 0;
}
