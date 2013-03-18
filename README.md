# Pafish
## (Paranoid Fish)

Pafish is a demo tool that performs some anti(debugger/VM/sandbox) tricks. Most of them are often used by malware to avoid debugging and dynamic analysis.

The project is open source, you can read the code of all anti-analysis checks. You can also [download](https://github.com/a0rtega/pafish/raw/master/pafish.exe) the compiled executable (or compile it by yourself) and reverse engineer it, which is quite recommended.

# Scope

Note that the aim of the project is not to implement complex VM detections.

The objective of this project is to collect usual tricks seen in malware samples. This allows us to study it, and test if our analysis environments are properly implemented.

Example of [execution](http://malwr.com/analysis/99f7c1358e1f488a32b8919083a9b25b/) (v02).

# Executable certificate

All releases from v024 will be shipped signed by the original development team. Consider everything without our certificate as unofficial, you can check it against the certificates present in the binaries from this repository.

# Build

Pafish is written in C and developed with [wxDev-C++](http://wxdsgn.sourceforge.net/).

# Author

Alberto Ortega (@[a0rtega](https://twitter.com/#!/a0rtega) - [profile](https://profiles.google.com/alberto.kun666)) - alberto[at]pentbox.net

# Contributions

[![Flattr](http://api.flattr.com/button/flattr-badge-large.png)](http://flattr.com/thing/1179653/a0rtegapafish-on-GitHub)

Bitcoins are really appreciated: 1DeSQzrEuQLUDjD25CX6yrchVuQnArWTeN

Feel free to send me malware samples or more tricks to add. GPG ID: 6A06CF5A
