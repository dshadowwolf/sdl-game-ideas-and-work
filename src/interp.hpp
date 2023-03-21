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
#include <functional>
#include <ext/stdio_filebuf.h>

#include "program_status.hpp"

class Interpreter {
public:
    // commands are
    typedef struct interp_command_s {
        std::string name;
        const std::function<void (int fd, std:: vector<std::string> args)> handler;
    } InterpreterCommand;

private:
    int fd;
    bool first;
    std::filebuf buff;
    std::string prompt = ">> ";
    std::vector<InterpreterCommand> commandStore{
        { "echo", [](int fd, std::vector<std::string> args) -> void {
            write(fd, "ECHO: ", 6);
            std::for_each(args.begin(), args.end(), 
                [&fd](std::string arg) { 
                    write(fd, arg.c_str(), arg.length());
                    write(fd, " ", 1);
                });
            write(fd, "\n", 1);
            }
        },
        { "exit", [](int fd, std::vector<std::string> args) -> void {
            write(fd, "Exiting...\n", 12);
            GameThingStatus::quit = true;
          }
        },
        { "quit", [](int fd, std::vector<std::string> args) -> void {
            write(fd, "Exiting...\n", 12);
            GameThingStatus::quit = true;
          }
        }
    };

    void call_command(std::string name, std::vector<std::string> args) {
        std::cerr << "Looking for " << name << " with args as follows: " << std::endl;
        std::for_each(args.begin(), args.end(), [](std::string arg) -> void { std::cerr << "\t" << arg << std::endl; });
        std::cerr << std::endl;
        std::string error = "Unknown Command";
        auto x = std::find_if(commandStore.begin(), commandStore.end(), [&name](InterpreterCommand possible) { return possible.name == name; });
        if (x != std::end(commandStore)) {
            (*x).handler(fd, args);
        } else {
            std::string emess = "Error: " + name + "\n" + error + "\n";
            size_t l = emess.length();
            write(fd, emess.c_str(), l);
        }
    }

public:
    Interpreter(int _fd) : 
      fd(_fd), buff(__gnu_cxx::stdio_filebuf<std::fstream::char_type>(fd, std::ios::in | std::ios::out | std::ios::binary)) {
        first = true;
    }

    void parse_and_interpret(std::string buffer) {
        // This and the other end are a hack...
        // we should be just pulling into a buffer when there is available,
        // then parsing here for what is passed in...
        // also going to need handling for various keypresses, potentially...
        int len = buffer.length();
        if(len > 0) {
            // get the first word of the input buffer, if such exists
ws_loop:    int index = buffer.find_first_of(' ');
            if (index == 0) {
                buffer = buffer.substr(1,buffer.length());
                goto ws_loop;
            } else if(index < 0) {
                // whole word must be the command, lets just go with that...
                std::vector<std::string> emptyArgs;
                call_command(buffer, emptyArgs);
            } else {
                std::stringstream ss(buffer);
                std::vector<std::string> baseBits;
                std::string word;
                while(std::getline(ss, word, ' ')) {
                    baseBits.emplace_back(word);
                    std::cerr << word << std::endl;
                }
                // pop the bits, prep for command call...
                std::string command = baseBits.front();
                baseBits.erase(baseBits.begin());
                call_command(command, baseBits);
            }
        } else if(len == 0) write(fd, "\n", 1);

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
            if (GameThingStatus::quit) return;
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
