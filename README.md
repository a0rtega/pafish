
# Pafish
## (Paranoid Fish)

Pafish is a demo tool that performs some anti(debugger/VM/sandbox) tricks. Most of them are often used by malware to avoid debugging and dynamic analysis.

The project is open source, you can read the code of all anti-analysis checks. You can also [download](https://github.com/a0rtega/pafish/raw/master/pafish.exe) the compiled executable (or compile it by yourself) and reverse engineer it, which is quite recommended.

It is licensed under GNU/GPL version 3.

# Scope

Note that the aim of the project is not to implement complex VM detections.

The objective of this project is to collect usual tricks seen in malware samples. This allows us to study them, and test if our analysis environments are properly implemented.

Examples of execution (v025 all of them):

[ThreatExpert](http://www.threatexpert.com/report.aspx?md5=7662cb4b1abc4ccb30b3682acc3dae24)  
[Cuckoo Sandbox](https://malwr.com/analysis/NTI1YjgyM2IxNzk3NDI3YjkyYzNkN2Y3NGE1NWFmZjE/)  
[Anubis](http://anubis.iseclab.org/?action=result&task_id=1f3d255d33107d034adcfcd3f2b1fb52c&format=html)  
[CWSandbox](https://mwanalysis.org/?page=report&analysisid=2154169&password=iplryjxwnj)  
[Comodo](http://camas.comodo.com/cgi-bin/submit?file=bbf9b5b80121a82cf93a67d86c23a8bd9b18313414fe49c30054fb4348e65547)  

# Certificate

All releases from v024 will be shipped signed by the original development team. Consider everything without our certificate unofficial, you can check it against the certificates present in the binaries from this repository.

# Build

Pafish is written in C and built with pure MinGW (make + gcc).

# Author

Alberto Ortega (@[a0rtega](https://twitter.com/#!/a0rtega) - [profile](http://aortega.badtrace.com))

# Contributions

Feel free to send me malware samples or more tricks to add. GPG ID: [6A06CF5A](https://keybase.io/alberto/key.asc)
