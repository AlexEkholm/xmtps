/** The @ref util::file::FileSystemUtils class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 18, 2014 12:47:22 PM
 *
 *  @author     Mr. Alexander K. Ekholm <aekholm@whoi.edu>,
 *              Engineer I,
 *              Department of Physical Oceanography,
 *              Woods Hole Oceanographic Institution
 *
 *  @copyright  2014 Woods Hole Oceanographic Institution.
 *              Released under the GNU General Public License.
 *
 *  This is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 *  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details. You should have received a copy of the GNU General
 *  Public License along with this software. If not, please visit http://www.gnu.org/licenses/.
 */

#ifndef FILESYSTEMUTILITY_H_
#define FILESYSTEMUTILITY_H_

/* Includes */

// local
#include "Log.h"
#include "StreamUtils.h"

namespace util {
namespace file {

using boost::filesystem::create_directories;
using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;
using boost::filesystem::exists;
using boost::filesystem::ifstream;
using boost::filesystem::is_directory;
using boost::filesystem::is_regular_file;
using boost::filesystem::ofstream;
using boost::filesystem::path;
using std::string;
using std::vector;

/* Class Definition */

/** Class providing library of useful filesystem functions.
 *
 */
class FileSystemUtils {

public:

    FileSystemUtils() {
      /* Do nothing. */
    }

    virtual ~FileSystemUtils() {
      /* Do nothing. */
    }

    /* Member Functions */

    /** Get a directory path.
     *
     *  @throws         MessageException if create == FALSE and the directory specified by path does
     *                  not exist.
     *
     *  @param[in]      dir             Directory path.
     *  @param[in]      create          If TRUE, the directory specified by path will be created if it
     *                                  does not already exists. If FALSE, an exception will be thrown
     *                                  if the directory does not exist.
     *
     *  @return         dir
     */
    static const path& GetDirectoryPath(const path& dir, bool create = false) {

        try {

            if(dir.empty())
                EXC_THROW("Empty directory path!");

            if(!is_directory(dir)) {

                if(exists(dir))
                    EXC_THROW("Invalid path to directory!");

                if(create) {

                    try { // Create the directory.

                        ldbg() << "Creating directory: " << dir << "...";
                        create_directories(dir);

                    }CATCH_BLOCK("Failed to create directory!")

                } else {

                    EXC_THROW("Directory does not exist!");

                }

            }

        }CATCH_BLOCK("Failed to get directory path" + dir.string() + "!")

        return dir;
    }

    /** Generate a boost::filesystem::path object from the directory path string.
     *
     *  @throws         MessageException if create == FALSE and the directory specified by path does
     *                  not exist.
     *
     *  @param[in]      dir             Directory path string.
     *  @param[in]      create          If TRUE, the directory specified by path will be created if it
     *                                  does not already exists. If FALSE, an exception will be thrown
     *                                  if the directory does not exist.
     *
     *  @return         A boost::filesystem::path object from the directory path string.
     */
    static path GetDirectoryPathFromString(const string& dir, bool create = false) {

        path p;

        try {

            p = GetDirectoryPath(path(dir), create);

        }CATCH_BLOCK("Failed to get path from directory string!")

        return p;
    }

    /** List all regular files contained in the given directory.
     *
     *  @param[in]      dir     The path to the directory of interest.
     *  @param[in]      n       If n>0, the maximum number of paths to return, else no effect.
     *
     *  @throw          log::MessageException if the directory does not exist, is not a directory, or
     *                  an error occurs.
     *
     *  @return         A vector of paths to all the regular files contained in the given directory.
     */
    static vector<path> ListFiles(path dir, size_t n = 0) {
        vector<path> files;

        try {
            if(!exists(dir))
                EXC_THROW("Directory does not exist: " + dir.string() + "!");
            if(!is_directory(dir))
                EXC_THROW("Path is not a valid directory: " + dir.string() + "!");

            copy_if(
                    directory_iterator(dir),
                    directory_iterator(),
                    back_inserter(files),
                    [](directory_entry & d) {
                        return is_regular_file(d.path());
                    });

            if(n > 0)
              files.resize(n);

        }CATCH_BLOCK("Failed to list files for directory!")

        return files;
    }

    /** List all directories contained in the given directory.
     *
     *  @param[in]      dir     The path to the directory of interest.
     *
     *  @throw          log::MessageException if the directory does not exist, is not a directory, or
     *                  an error occurs.
     *
     *  @return         A vector of paths to all the directories contained in the given directory.
     */
    static vector<path> ListDirectories(const path& dir) {

        vector<path> dirs;

        try {
            if(!exists(dir))
                EXC_THROW("Directory does not exist: " + dir.string() + "!");
            if(!is_directory(dir))
                EXC_THROW("Path is not a valid directory: " + dir.string() + "!");

            copy_if(
                    directory_iterator(dir),
                    directory_iterator(),
                    back_inserter(dirs),
                    [](directory_entry & d) {
                        return is_directory(d.path());
                    });

        }CATCH_BLOCK("Failed to list directories!")

        return dirs;
    }

    static path MoveFileToDirectory(const path& file, const path& dir_dst, bool create_dir = false) {

        path file_dst;

        try{

            if(!exists(file))
                EXC_THROW("File not found!");

            if(!is_regular_file(file))
                EXC_THROW("Invalid file path!");

            GetDirectoryPath(dir_dst, create_dir);

            file_dst = dir_dst;
            file_dst /= file.filename();

            if(!exists(file_dst)) {
//                      copy_file(list_file, list_file_dest);

                ifstream src(file, std::ios::binary);
                ofstream dst(file_dst, std::ios::binary);

                dst << src.rdbuf();

            }

        } CATCH_BLOCK("Failed to move file: " + file.string() + "!")


        return file_dst;

    }


};

} /* namespace file */
} /* namespace util */
#endif /* FILESYSTEMUTILITY_H_ */
