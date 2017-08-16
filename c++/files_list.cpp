#include "files_list.h"
#include "magic.h"
#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;
using namespace std;

// for each FILE argument, add to list iff it is of mimetype pptx (recursively, if needed)
Files_list::Files_list(const vector<string>& file_args, bool recursive, bool silent) {
    for (string file: file_args) {
        fs::path file_path {file};

        // if path doesn't exist, output error, and continue
        if (!fs::exists(file_path)) {
            if (!silent)
                cerr << "grep-pptx: " << file_path.string() << ": No such file or directory" << endl;
            continue;
        }
        // if regular file, and is a pptx then append it to list
        else if (fs::is_regular_file(file_path) && is_pptx(file)) {
            files.push_back(file);
        }
        // if file is a directory, and no "recursive" option then output error, and continue
        else if (fs::is_directory(file_path)) {
            if (!recursive) {
                if (!silent)
                    cerr << "grep-pptx: " << file_path.string() << ": Is a directory" << endl;
                continue;
            }
            // if recursive, iterate through directory (recursively) for all files matching pptx mimetype
            else {
                try {
                    auto fs_iterator = fs::recursive_directory_iterator(file_path);
                    while (fs_iterator != fs::recursive_directory_iterator()) {
                        string filepath = (*fs_iterator).path().string();
                        if (fs::is_regular_file(*fs_iterator) && is_pptx(filepath))
                            files.push_back(filepath);

                        // increment iterator; if there is an issue with a directory (e.g. no permission) then skip directory
                        try {
                            fs_iterator++;
                        }
                        catch (fs::filesystem_error& e) {
                            if (!silent) {
                                cerr << e.what() << endl;
                                cerr << "grep-pptx: " << filepath << ": Permission denied" << endl;
                            }
                            fs_iterator.no_push();
                            fs_iterator++;
                        }
                    }
                }
                catch (fs::filesystem_error& e) {
                    if (!silent) {
                        cerr << e.what() << endl;
                        cerr << "grep-pptx: " << file_path.string() << ": Permission denied" << endl;
                    }
                    continue;
                }
            }
        }
    }
}

// use libmagic and magic.h to get mime type of file
// true iff mime type of file matches pptx mimetype
bool Files_list::is_pptx(string file) {

    // using MAGIC_MIME_TYPE flag, since MAGIC_MIME flag includes MAGIC_MIME_ENCODING, which we don't want
    magic_t myt = magic_open(MAGIC_CONTINUE|MAGIC_ERROR/*|MAGIC_DEBUG|MAGIC_MIME*/|MAGIC_MIME_TYPE);
    magic_load(myt, NULL);
    string mime_type = magic_file(myt, file.c_str());
    magic_close(myt);

    return (mime_type == pptx_mimetype);
}

vector<string> Files_list::get_files() const {
    return files;
}
