
# Pafish
## (Paranoid Fish)

Pafish is a demonstration tool that employs several techniques to detect sandboxes and analysis environments in the same way as malware families do.

The project is open source, you can read the code of all anti-analysis checks. You can also [download](https://github.com/a0rtega/pafish/raw/master/pafish.exe) the compiled executable (or compile it by yourself) and reverse engineer it, which is quite recommended.

It is licensed under GNU/GPL version 3.

# Scope

Note that the aim of the project is not to implement complex VM detections.

The objective of this project is to collect usual tricks seen in malware samples. This allows us to study them, and test if our analysis environments are properly implemented.

Examples of execution (v0.5.1 all of them):

[Cuckoo Sandbox](https://malwr.com/analysis/NGRkMjkxMzllOTFiNDJmOGJmNjM0YWU1MDcwNGZkM2Y/)  
[Anubis](https://anubis.iseclab.org/?action=result&task_id=11db4238cbf5db5a451d251820e73d4de&format=html)  
[ThreatExpert](http://www.threatexpert.com/report.aspx?md5=87b08b9db49b4322df2249b7059bc1f5)  

# Build

Pafish is written in C and can be built with MinGW (gcc + make).

Official MinGW ([http://www.mingw.org/](http://www.mingw.org/)) and Cygwin ([https://cygwin.com/](https://cygwin.com/)) are proven to work well.

To compile you will likely need to use:
```
make -f Makefile.win
make -f Makefile.linux # if compiling on linux
```

# Author

Alberto Ortega (@[a0rtega](https://twitter.com/#!/a0rtega) - [profile](http://aortega.badtrace.com))

# Contributions

Feel free to send me malware samples or more tricks to add. GPG ID: [6A06CF5A](https://keybase.io/alberto/key.asc)

