#include "grep_pptx.h"
#include "files_list.h"
#include <iostream>
#include <vector>
using namespace std;

/*
 * A command line utility to search text within powerpoint presentations of format pptx
 * Outputs zero or more lines with file name, slide number, and the matched text snippet
 * Terminal color output is similar to the output of grep options -n (print line number) and -H (print file name with each match)
 *
 * TODO:
 *     create installation instructions, instructions for setting up all third party libraries used, which must create "include" and "lib" directories within this directory
 *     testing: design a test suite and test it reasonably well ; compare with Python version, and test both
 *     get rid of color terminal codes on Windows; or note the limitation
 *     make a note of all supported platforms, terminals
 *     create a readme file with markdown
 *     specify compiler and version in documentation: e.g. g++-4.8
 *     improve code e.g. inline getter functions; *static* const to define symbolic constants within classes
 * NOTES:
 *     libraries:
 *         Boost libraries used (boost version 1.6.3):
 *             boost program_options:  http://www.boost.org/doc/libs/1_63_0/doc/html/program_options.html
 *             boost filesystem:       http://www.boost.org/doc/libs/1_63_0/libs/filesystem/doc/index.htm
 *             boost regex:            http://www.boost.org/doc/libs/1_63_0/libs/regex/doc/html/index.html
 *         libzip:                     https://nih.at/libzip/libzip.html
 *     note that libraries gzip and zlib will not handle zip archives by themselves
 *     magic number of pptx: 50 4b 03 04; not 1f 8b
 */

int main(int argc, char* argv[]) {
    try {

        // parse all arguments and options
        Command_line_args_options args_options = Command_line_args_options(argc, argv);

        // return if -h/--help
        if (args_options.help())
            return 0;

        // obtain list of files to be searched from the arguments
        Files_list f_list = Files_list(args_options.files(), args_options.recursive(), args_options.silent());

        // grep for PATTERN in each file in list of files, and display the results
        Grep_pptx pptx_files (f_list.get_files(), args_options);
        pptx_files.grep_print();

    }
    catch (...) {
        return 1;
    }
}
