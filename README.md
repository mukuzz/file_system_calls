# File System Calls Assignment

## Compilation
```
gcc main.cpp file.cpp parser.cpp -o filer -lstdc++ -std=c++17
```

## Usage
```
./filer COMMAND FILEPATH [ options ]

Note: All data input/outputs are handled through stdin and stdout.

Commands
        create: create a file with given file path, mode, and filetype
        read: read file with given file path to stdout
        write: write the content of stdin to file with given path
        stat: get information of the file`

FILEPATH
        path to the file to manipulate

Options
    -mode
        the file permissions for owner, group, and any user in octal system.
        eg. 0777 for full access to everyone.
    -filetype
        regular or pipe.
    -offset
        byte offset in file read/write
    -bytes
        bytes to read/write
```

## Examples

### To copy contents of file
```
./filer create file2 -mode 0777 -filetype regular
./filer read file1 | ./filer write file2
```

### To get details of file
```
./filer stat main.cpp
```