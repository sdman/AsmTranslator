#include <regex>
#include "EndCommand.h"
#include "../LabelContainer.h"

EndCommand::EndCommand(const CommandData &data, LabelContainer *labelContainer, ErrorContainer *errorContainer)
        : Command(data, labelContainer, errorContainer)
{
    parseArg();
}

size_t EndCommand::size() const
{
    return m_commandSize;
}

void EndCommand::translate(VmExecutable &vmExec, Address)
{
    if(!hasError())
    {
        vmExec.setIp(m_ip);
        vmExec.appendByte(static_cast<Byte>(m_stopCommandCode));
    }
}

void EndCommand::parseArg()
{
    if(!checkArgCorrectness())
    {
        handleError(CompillerError::ArgumentIncorrect);
        return;
    }

    else if(!labelExists())
    {
        handleError(CompillerError::LabelNotFound);
        return;
    }

    m_ip = getArgAddress();
}

bool EndCommand::checkArgCorrectness() const
{
    const std::regex regEx("[a-zA-Z_]\\w+");
    return std::regex_match(data().arg, regEx);
}

bool EndCommand::labelExists() const
{
    return labelContainer()->contains(data().arg);
}

Address EndCommand::getArgAddress() const
{
    return labelContainer()->address(data().arg);
}
