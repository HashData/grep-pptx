#include "command_line_args_options.h"
#include <string>
#include <vector>

/* Opens each provided file, and searches for a match of PATTERN, based on the options provided
 * Outputs result to stdout
 */
class Grep_pptx {
    public:
        Grep_pptx(std::vector<std::string> filepaths, const Command_line_args_options& args_options);

        // open each filepath, and grep for PATTERN
        void grep_print() const;

    private:
        // list of pptx filepaths
        std::vector<std::string> filepaths;

        // args_options readonly access
        const Command_line_args_options& args_options;

        // terminal color codes
        const std::string term_reset = "\x1b[0m";
        const std::string red_bold = "\x1b[01;31m";
        const std::string green = "\x1b[32m";
        const std::string magenta = "\x1b[35m";
        const std::string cyan = "\x1b[36m";

};
