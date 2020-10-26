#ifndef __Rmdir_include__
#define __Rmdir_include__

#include <lib/OutputStream.h>
#include <lib/String.h>
#include <lib/file/FileStatus.h>
#include "Command.h"

/**  need to change this
 * Implementation of Command.
 * Deletes files. Multiple paths can be handed over.
 *
 * -r, --recursive: Recursively delete an entire directory
 * -h, --help: Show help message
 *
 * @author Fabian Ruhland
 * @date 2018
 */
class Rmdir : public Command {

private:
    FileSystem *fileSystem = nullptr;

    void deleteFile(const String &progName, FileStatus &fStat);

    void deleteDirectory(const String &progName, Directory &dir);

public:
    /**
     * Default-constructor.
     */
    Rmdir() = delete;

    /**
     * Copy-constructor.
     */
    Rmdir(const Rmdir &copy) = delete;

    /**
     * Constructor.
     *
     * @param shell The shell, that executes this command
     */
    explicit Rmdir(Shell &shell);

    /**
     * Destructor.
     */
    ~Rmdir() override = default;

    /**
     * Overriding function from Command.
     */
    void execute(Util::Array<String> &args) override;

    /**
     * Overriding function from Command.
     */
    const String getHelpText() override;
};

#endif