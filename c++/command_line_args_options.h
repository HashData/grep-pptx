#include <string>
#include <vector>

/* parses command line arguments and options, and does basic error checking if incorrect argumentions or options provided
 * stores all arguments and options, and displays help or usage message, as needed
 */
class Command_line_args_options {
    public:
        Command_line_args_options(int argc, char* argv[]);

        // getter functions
        std::string pattern() const;
        std::vector<std::string> files() const;
        bool help() const;
        bool perl() const;
        bool fixed() const;
        bool ignore_case() const;
        bool recursive() const;
        bool silent() const;
    private:
        // store arguments and options
        std::string arg_pattern;
        std::vector<std::string> arg_files;
        bool option_help;
        bool option_perl;
        bool option_fixed;
        bool option_ignore_case;
        bool option_recursive;
        bool option_silent;

        // help and usage
        std::string help_message() const;
        std::string usage() const;
};
