#ifndef __INTERP_HPP__
#include <errno.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <vector>
#include <ext/stdio_filebuf.h>

class Interpreter {
    int fd;
    bool first;
    std::filebuf buff;
    std::string prompt = ">> ";

public:
    Interpreter(int _fd): fd(_fd), buff(__gnu_cxx::stdio_filebuf<std::fstream::char_type>(fd, std::ios::in | std::ios::out | std::ios::binary)) {
        first = true;
    }

    void parse_and_interpret(std::string buffer) {
        // This and the other end are a hack...
        // we should be just pulling into a buffer when there is available,
        // then parsing here for what is passed in...
        // also going to need handling for various keypresses, potentially...
        int len = buffer.length();
        if(len > 0)
            write(fd, buffer.c_str(), len);
        else if(len == 0)
            write(fd, "\n", 1);

        write(fd, prompt.c_str(), 2);

        // TODO: Actual command handling here!
    }

    void run_loop() {
        if (first) {
            std::cerr << "First Run" << std::endl;
            std::string welcome = "##HACK-OS v5.11 READY\n";
            std::string next_line = "## AWAITING INPUT\n";

            std::ostream outs(&buff);
            outs << welcome << next_line << prompt;
            first = false;
        }

        std::string ins;
        std::istream i_s(&buff);
        for(;;) {
            // Read at most BUF_SIZE bytes from the socket into buf.
            while( buff.in_avail() > 0) {
                std::getline(i_s,ins);
                parse_and_interpret(ins);
            }
        }
    }
};

#define __INTERP_HPP__
#endif
