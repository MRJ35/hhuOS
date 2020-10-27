#ifndef __Pwd_include__
#define __Pwd_include__

#include <lib/OutputStream.h>
#include <lib/String.h>
#include "Command.h"

/**
 * Implementation of Command.
 * Changes the shell's working directory.
 *
 * -h, --help: Show help message
 *
 * @author Fabian Ruhland
 * @date 2018
 */
class Pwd : public Command {

public:
    /**
     * Default-constructor.
     */
    Pwd() = delete;

    /**
     * Copy-constructor.
     */
    Pwd(const Pwd &copy) = delete;

    /**
     * Constructor.
     *
     * @param shell The shell, that executes this command
     */
    explicit Pwd(Shell &shell);

    /**
     * Destructor.
     */
    ~Pwd() override = default;

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