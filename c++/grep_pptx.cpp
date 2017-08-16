#include "grep_pptx.h"
#include <boost/regex.hpp>
#include <zip.h>
#include <iostream>

using namespace std;

Grep_pptx::Grep_pptx(vector<string> filepaths, const Command_line_args_options& args_options) 
    : filepaths (filepaths), args_options (args_options)
{}

// for each file in the list of files, search for a MATCH, taking into consideration the variations due to any options
void Grep_pptx::grep_print() const {

    for (string filepath : filepaths) {

        // unzip archive
        int err = 0;
        zip_t* ziparchive = zip_open(filepath.c_str(), ZIP_RDONLY, &err);
        if (!ziparchive) {
            if (!args_options.silent())
                cerr << "WARNING: Failed to unzip. Is this the correct format?: " << filepath << endl;
            return;
        }

        // for each slide in pptx
        for (int slidenumber = 1; ; slidenumber++) {

            // attempt to open slide, if cannot open then break
            string slidename = "ppt/slides/slide" + to_string(slidenumber) + ".xml";
            zip_file_t* zipfile = zip_fopen(ziparchive, slidename.c_str(), 0);
            if (!zipfile)
                break;

            // retreive uncompressed size of file
            struct zip_stat st;
            zip_stat_init(&st);
            zip_stat(ziparchive, slidename.c_str(), 0, &st);

            // retrieve raw XML content of slide
            char* contents = (char*) malloc(st.size + 1);
            if (!contents) {
                if (!args_options.silent())
                    cerr << "ERROR: malloc call failed on file: " << filepath << endl;
                continue;
            }

            if (zip_fread(zipfile, contents, st.size) == -1) {
                if (!args_options.silent())
                    cerr << "WARNING: Failed to read file: " << filepath << endl;
                free(contents);
                continue;
            }
            contents[st.size] = '\0';
            string slide_contents {contents};

            // retrieve all text content of slide within <a:t> tags
            boost::regex re_a_t {"<a:t>(.*?)</a:t>"};
            boost::sregex_token_iterator iter {slide_contents.begin(), slide_contents.end(), re_a_t, 1};
            boost::sregex_token_iterator end;
            boost::smatch result;

            // set pattern, based on the ignorecase option
            boost::regex re_pattern {args_options.pattern()};
            if (args_options.ignore_case())
                re_pattern = boost::regex {args_options.pattern(), boost::regex::icase};

            // search for the given PATTERN, output {filename, slide number, matched line} to stdout if there is a match
            for(; iter != end; ++iter) {
                string snippet = (string)(*iter);

                // perl regex matching
                if (args_options.perl()) {
                    if (boost::regex_search(snippet, result, re_pattern))
                        cout << magenta << filepath << cyan << ":" << green << slidenumber << cyan << ":" << term_reset << result.prefix()
                             << red_bold << result[0] << term_reset << result.suffix() << endl;
                }

                // default option of "fixed-string" matching
                else {
                    string pattern = args_options.pattern();
                    // if ignore_case, then convert both snippet and pattern to lowercase
                    if (args_options.ignore_case()) {
                        std::transform(snippet.begin(), snippet.end(), snippet.begin(), ::tolower);
                        std::transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
                    }
                    std::size_t found = snippet.find(pattern);
                    if (found != string::npos)
                        cout << magenta << filepath << cyan << ":" << green << slidenumber << cyan << ":" << term_reset << snippet.substr(0, found)
                             << red_bold << snippet.substr(found, pattern.length()) << term_reset << snippet.substr(found + pattern.length()) << endl;
                }
            }

            // free contents, close file
            free(contents);
            zip_fclose(zipfile);
        }
        // close zip archive without modifying
        zip_discard(ziparchive);
    }
}
