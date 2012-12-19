# Pafish
## (Paranoid Fish)

Pafish is a demo tool that performs some anti(debugger/VM/sandbox) tricks. Most of them are often used by malware to avoid debugging and dynamic analysis.

The project is open source, you can read the code of all anti-analysis checks. You can also [download](https://github.com/a0rtega/pafish/raw/master/pafish.exe) the compiled executable (or compile it by yourself) and reverse engineer it, which is quite recommended.

# Scope

Note that the aim of the project is not to implement complex VM detections.

The objective of this project is to collect usual tricks seen in malware samples. This allows us to study it, and test if our analysis environments are properly implemented.

Example of [execution](http://malwr.com/analysis/99f7c1358e1f488a32b8919083a9b25b/) (v02).

# Build

Pafish is written in C and developed with [wxDev-C++](http://wxdsgn.sourceforge.net/).

# Author

Alberto Ortega (@[a0rtega](https://twitter.com/#!/a0rtega) - [profile](https://profiles.google.com/alberto.kun666)) - alberto[at]pentbox.net

# Contributions

Feel free to send me malware samples or more tricks to add. GPG ID: 6A06CF5A
