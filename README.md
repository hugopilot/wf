**OBSOLETE**: Linux users can use `shred`. This was just another random side-project of mine :)

# wf
Wipes your Files.


## How does it wipe?
Well, first of all it will overwrite the contents of the file a couple of times (3 times is default), then it will change the metadata and then finally delete it from the filetable.


**This app only compiles with g++ atm. MSVC starts whining about stuff and I can't seem to fix it. So for Windows users: Use CYGWIN**


