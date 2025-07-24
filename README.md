# NovaOS

Note that all of the documentation of usage (and errors) is found in the header files.
Any implementation notes and TODOs are found inside of the source files.
If the file is assembly, there isn't any documentation, only explanatory comments.

NovaOS Version 0.1.5.4x
Patch notes :-
Currently fixing bootloop error found in bootloader.
It's a massive pain in the arse.
I think I might have to phone a friend.
Will update to 0.1.5.5 when major development happens.

NOTE: Version System is [MAJOR VERSION NUMBER].[MINOR VERSION NUMBER].[PATCH NUMBER].[CANARY TEST NUMBER] {add an x if canary is nonfunctional}
Definitions:
MAJOR VERSION - Proper New Adaptation/Feature Set, eg. UserSpace/Shell/Graphics/Networking/UI/UX. Typically means code has to be changed majorly.
MINOR VERSION - New Feature Added, eg. Formatted Printing/Memory Allocation. Code doesn't have to be changed by a lot.
PATCH - Error Fix Batch or Minor Improvement (eg. new libc/k functions). Just a fix.
CANARY - Unstable Temporary Release. The user shouldn't even see this.
