#include "command_line_args_options.h"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;
using namespace std;

// parse all arguments and options; do basic error checking; initialize all members
Command_line_args_options::Command_line_args_options(int argc, char* argv[]) {

    // declare the supported required arguments
    po::positional_options_description positional;
    positional.add("PATTERN", 1);
    positional.add("FILE", -1);
    
    // hide the required arguments
    po::options_description hidden("hidden");
    hidden.add_options()
        ("PATTERN", po::value<string>(), "Pattern to be searched")
        ("FILE", po::value< vector<string> >(), "Powerpoint pptx file")
        ;

    // declare the supported optional arguments
    po::options_description optional("optional arguments");
    optional.add_options()
        ("help,h", "Print this help message and exit")
        ("perl-regexp,P", "Interpret PATTERN as a Perl regular expression")
        ("fixed-string,F", "Interpret PATTERN as a string literal (default)")
        ("ignore-case,i", "Ignore case distinctions in PATTERN")
        ("recursive,r", "Read all pptx files under each directory, recursively")
        ("no-messages,s", "Suppress error and warning messages about nonexistent files, unreadable files, and files in an unsupported format")
        ;

    // aggregate all options
    po::options_description all_options;
    all_options.add(optional);
    all_options.add(hidden);

    // parse arguments and options
    po::variables_map var_map;
    try {
        po::store(po::command_line_parser(argc, argv).options(all_options).positional(positional).run(), var_map);
        po::notify(var_map);    
    }
    catch (exception &e) {
        cerr << "grep-pptx: " << e.what() << endl << usage();
        throw;
    }

    // initialize all options; default for all options is false
    option_help = option_perl = option_fixed = option_ignore_case = option_recursive = option_silent = false;
    if (var_map.count("help")) {
        option_help = true;
        // if -h/--help is specified, display help message, and return with current partial initialization
        cout << help_message() << optional << endl;
        return;
    }
    if (var_map.count("perl-regexp"))
        option_perl = true;
    if (var_map.count("fixed-string"))
        option_fixed = true;
    if (var_map.count("ignore-case"))
        option_ignore_case = true;
    if (var_map.count("recursive"))
        option_recursive = true;
    if (var_map.count("no-messages"))
        option_silent = true;

    // PATTERN and at least one FILE must be specified
    if (!(var_map.count("PATTERN") && var_map.count("FILE"))) {
        cerr << "grep-pptx: PATTERN and FILE are required arguments" << endl << usage();
        throw "Invalid arguments";
    }

    // initialize all arguments
    arg_pattern = var_map["PATTERN"].as<string>();
    arg_files = var_map["FILE"].as< vector<string> >();
}

string Command_line_args_options::pattern() const {
    return arg_pattern;
}

vector<string> Command_line_args_options::files() const {
    return arg_files;
}

bool Command_line_args_options::help() const {
    return option_help;
}
bool Command_line_args_options::perl() const {
    return option_perl;
}

bool Command_line_args_options::fixed() const {
    return option_fixed;
}

bool Command_line_args_options::ignore_case() const {
    return option_ignore_case;
}

bool Command_line_args_options::recursive() const {
    return option_recursive;
}

bool Command_line_args_options::silent() const {
    return option_silent;
}

string Command_line_args_options::usage() const {
    return {"Usage: grep-pptx [OPTION]... PATTERN FILE...\nTry 'grep-pptx --help' for more information.\n"};
}

string Command_line_args_options::help_message() const {
    return { "Usage: grep-pptx [OPTION]... PATTERN FILE...\n\n" \
             "Search each FILE (of format pptx) for lines containing a match to PATTERN.\n" \
             "Default output is the matching lines with the file name, slide number, and matched text snippet.\n" \
             "Example: grep-pptx -iP 'foo.*bar' presentation.pptx\n\n"};
}
