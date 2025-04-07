## 1.0.1
- Fixed bug that caused final output file to be deleted at the end of compilation (embarrassing oversight when rushing to release 1.0.0)
- Fixed bug where tildeathc kept the output binary file open while its child process tried to run it, causing a Text File Busy error
- Fixed various bugs that caused temporary files to not be cleaned up when certain error conditions happened or the program was interrupted
- Changed how temporary files are handled so that the $TMPFILE environment variable is now used, defaulting to /tmp when not set

## 1.1.0
- Added ability to take input from stdin by providing - as the input file
