#include <string>
#include <vector>

/* Files_list searches and stores the list of pptx files to be searched; recursively within directories, if needed
 * uses the FILEs arguments provided, and checks each file's mimetype for a match with the pptx mimetype
 */
class Files_list {
    public:
        Files_list(const std::vector<std::string>& file_args, bool recursive, bool silent);

        // getter function; retrieve the list of files
        std::vector<std::string> get_files() const;

    private:
        std::vector<std::string> files;

        // mime type for pptx files
        const std::string pptx_mimetype = "application/vnd.openxmlformats-officedocument.presentationml.presentation";

        // true iff mime type of "file" matches pptx mime type
        bool is_pptx(std::string file);
};
