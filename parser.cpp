#include <iostream>
#include <string>
#include "constants.hpp"
#include "file.hpp"

enum filetypes { NONE = 0, REGULAR, PIPE };

struct Opts {
    int mode = 0;
    filetypes filetype;
    int offset = 0;
    int bytes = -1; // number of bytes not specified

    Opts(int argc, char *opts[]) {
        parseOptions(argc, opts);
    }

    void parseOptions(int argc, char *opts[]) {
        for (int i = 0; i < argc - 1; i++) {
            const std::string opt = opts[i];
            const std::string val = opts[i+1];
            if (opt == "-mode") {
                mode = std::stoi(val, nullptr, 8);
            }
            else if (opt == "-filetype") {
                if (val == "regular") filetype = REGULAR;
                else if (val == "pipe") filetype = PIPE;
                else std::cerr << "Bad filetype" << std::endl;
            }
            else if (opt == "-offset") {
                offset = std::stoi(val, nullptr, 10);
            }
            else if (opt == "-bytes") {
                bytes = std::stoi(val, nullptr, 10);
            }
        }
    }
};

void showHelp() { std::cout << HELP_TEXT << std::endl; };

void parseArgs(int argc, char *argv[]) {
    
    // if command and filename not given as arguments, show help text
    if (argc <= 2) showHelp();

    if (argc >= 3) {
        const std::string command = argv[1];
        const std::string filepath = argv[2];
        Opts opts(argc, argv);

        if(command == "create") {
            if (opts.mode == 0) {
                std::cerr << "create: mode/permissions not specified, creating file with 0777 mode" << std::endl;
                opts.mode = 0777;
            }
            if (opts.filetype == REGULAR) {
                create_norm_file(filepath.c_str(), opts.mode);
            } else if (opts.filetype == PIPE) {
                create_pipe(filepath.c_str(), opts.mode);
            } else {
                std::cerr << "filetype is incorrect or not specified" << std::endl;
            }
        } else if (command == "read") {
            read_file(filepath.c_str(), opts.offset, opts.bytes);
        } else if (command == "write") {
            write_file(filepath.c_str(), opts.offset, opts.bytes);
        } else if (command == "stat") {
            file_stat(filepath.c_str());
        } else {
            std::cerr << "Error: Choose a valid command" << std::endl;
        };
    }
}