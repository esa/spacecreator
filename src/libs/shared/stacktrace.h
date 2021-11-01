#pragma once

#include <ctime>
#include <cxxabi.h>
#include <execinfo.h>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>

namespace stacktrace {

static inline void crit_err_hdlr(int sig_num, siginfo_t *info, void *ucontext)
{
    (void)sig_num;
    (void)info;
    (void)ucontext;

    std::time_t tt = std::time(0);
    std::tm tm = *std::localtime(&tt);
    std::stringstream ss;
    ss << std::filesystem::temp_directory_path().native() << "/";
    ss << std::put_time(&tm, "%Y_%m_%d-%H_%M_%S.st");

    std::ofstream os(ss.str(), std::ios::binary);

    void *array[50];
    int size = backtrace(array, 50);

    char **messages = backtrace_symbols(array, size);

    // skip first stack frame (points here)
    for (int i = 1; i < size && messages != NULL; ++i) {
        char *mangled_name = 0, *offset_begin = 0, *offset_end = 0;

        // find parantheses and +address offset surrounding mangled name
        for (char *p = messages[i]; *p; ++p) {
            if (*p == '(') {
                mangled_name = p;
            } else if (*p == '+') {
                offset_begin = p;
            } else if (*p == ')') {
                offset_end = p;
                break;
            }
        }

        // if the line could be processed, attempt to demangle the symbol
        if (mangled_name && offset_begin && offset_end && mangled_name < offset_begin) {
            *mangled_name++ = '\0';
            *offset_begin++ = '\0';
            *offset_end++ = '\0';

            int status;
            char *real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);

            // if demangling is successful, output the demangled function name
            if (status == 0) {
                os << "[bt]: (" << i << ") " << messages[i] << " : " << real_name << "+" << offset_begin << offset_end
                   << std::endl;

            }
            // otherwise, output the mangled function name
            else {
                os << "[bt]: (" << i << ") " << messages[i] << " : " << mangled_name << "+" << offset_begin
                   << offset_end << std::endl;
            }
            free(real_name);
        }
        // otherwise, print the whole line
        else {
            os << "[bt]: (" << i << ") " << messages[i] << std::endl;
        }
    }
    os.close();

    free(messages);

    exit(EXIT_FAILURE);
}

static inline void init()
{
    struct sigaction sigact;

    sigact.sa_sigaction = crit_err_hdlr;
    sigact.sa_flags = SA_RESTART | SA_SIGINFO;

    if (sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0) {
        fprintf(stderr, "error setting signal handler for %d (%s)\n", SIGSEGV, strsignal(SIGSEGV));

        exit(EXIT_FAILURE);
    }
}

} // namespace stacktrace
