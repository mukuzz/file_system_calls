#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string_view>

const std::string_view HELP_TEXT = R"(
Usage:  filer COMMAND FILEPATH [ options ]

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
)";

#endif