#include "tmc_application.h"
#include "strextra.h"

tmc::Application* globalInstance;

namespace tmc {

Application::Application(int argc, char** argv)
{
    globalInstance = this;
    args = std::vector<std::string>(argc);
    for (s32 i = 0; i < argc; i++)
    {
        args[i] = argv[i];
    }
    u64 fileStart = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < args.size(); i++)
    {
        if (args[i].size() >= 2 && args[i][0] == '-' && args[i][1] != '-')
        {
            std::vector<char> temp = processShortArgs(args[i]);
            for (u64 j = 0; j < temp.size(); j++)
            {
                shortArgs.push_back({temp[j], std::vector<std::string>({""})});
            }
            u64 j = 1;
            while (args.size() > (i + j) && args[i + j][0] != '-')
            {
                if (j == 1) shortArgs.back().second.clear();
                shortArgs.back().second.push_back(args[i + j]);
                j++;
            }
        }
        else if (args[i].size() > 2 && args[i][0] == '-' && args[i][1] == '-')
        {
            longArgs.push_back({args[i].substr(2, args[i].size() - 2), std::vector<std::string>({""})});
            u64 j = 1;
            if (args.size() > (i + 1) && args[i + 1][0] != '-')
            {
                if (j == 1) longArgs.back().second.clear();
                longArgs.back().second.push_back(args[i + 1]);
                j++;
            }
        }
        else if (args[i] == "--" && args.size() > (i + 1))
        {
            fileStart = i + 1;
            break;
        }
    }
    if (fileStart != std::numeric_limits<u64>::max())
    {
        for (u64 i = fileStart; i < args.size(); i++)
        {
            filepaths.push_back(std::filesystem::path(args[i]));
        }
    }
}

Application* Application::getGlobalInstance()
{
    return globalInstance;
}

template <>
std::vector<bool> Application::getAs(const std::string& longArg)
{
    std::vector<bool> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(toAllUpper(longArgs[idx].second[i]) == "TRUE");
    }
    return result;
}

template <>
std::vector<char> Application::getAs(const std::string& longArg)
{
    std::vector<char> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(longArgs[idx].second[i][0]);
    }
    return result;
}

template <>
std::vector<u8> Application::getAs(const std::string& longArg)
{
    std::vector<u8> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<u8>(std::stoull(longArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<s8> Application::getAs(const std::string& longArg)
{
    std::vector<s8> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<s8>(std::stoll(longArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<u16> Application::getAs(const std::string& longArg)
{
    std::vector<u16> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<u16>(std::stoull(longArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<s16> Application::getAs(const std::string& longArg)
{
    std::vector<s16> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<s16>(std::stoll(longArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<u32> Application::getAs(const std::string& longArg)
{
    std::vector<u32> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<u32>(std::stoull(longArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<s32> Application::getAs(const std::string& longArg)
{
    std::vector<s32> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<s32>(std::stoll(longArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<u64> Application::getAs(const std::string& longArg)
{
    std::vector<u64> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(std::stoull(longArgs[idx].second[i]));
    }
    return result;
}

template <>
std::vector<s64> Application::getAs(const std::string& longArg)
{
    std::vector<s64> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(std::stoll(longArgs[idx].second[i]));
    }
    return result;
}

template <>
std::vector<f32> Application::getAs(const std::string& longArg)
{
    std::vector<f32> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<f32>(std::stod(longArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<f64> Application::getAs(const std::string& longArg)
{
    std::vector<f64> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(std::stod(longArgs[idx].second[i]));
    }
    return result;
}

template <>
std::vector<std::string> Application::getAs(const std::string& longArg)
{
    std::vector<std::string> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(longArgs[idx].second[i]);
    }
    return result;
}

template <>
std::vector<std::filesystem::path> Application::getAs(const std::string& longArg)
{
    std::vector<std::filesystem::path> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < longArgs[idx].second.size(); i++)
    {
        result.push_back(std::filesystem::path(longArgs[idx].second[i]));
    }
    return result;
}

template <class T>
T Application::getAs(const std::string& longArg)
{
    T result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    if (std::is_signed<T>::value)
    {
        result = static_cast<T>(std::stoll(longArgs[idx].second[0]));
    }
    else if (std::is_unsigned<T>::value)
    {
        result = static_cast<T>(std::stoull(longArgs[idx].second[0]));
    }
    else if (std::is_floating_point<T>::value)
    {
        result = static_cast<T>(std::stod(longArgs[idx].second[0]));
    }
    return result;
}

template <>
std::string Application::getAs(const std::string& longArg)
{
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    return longArgs[idx].second[0];
}

template <>
bool Application::getAs(const std::string& longArg)
{
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    return (toAllUpper(longArgs[idx].second[0]) == "TRUE");
}

template <>
std::filesystem::path Application::getAs(const std::string& longArg)
{
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    return std::filesystem::path(longArgs[idx].second[0]);
}

template <>
std::vector<bool> Application::getAs(char shortArg)
{
    std::vector<bool> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(toAllUpper(shortArgs[idx].second[i]) == "TRUE");
    }
    return result;
}

template <>
std::vector<char> Application::getAs(char shortArg)
{
    std::vector<char> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(shortArgs[idx].second[i][0]);
    }
    return result;
}

template <>
std::vector<u8> Application::getAs(char shortArg)
{
    std::vector<u8> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<u8>(std::stoull(shortArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<s8> Application::getAs(char shortArg)
{
    std::vector<s8> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<s8>(std::stoll(shortArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<u16> Application::getAs(char shortArg)
{
    std::vector<u16> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<u16>(std::stoull(shortArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<s16> Application::getAs(char shortArg)
{
    std::vector<s16> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<s16>(std::stoll(shortArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<u32> Application::getAs(char shortArg)
{
    std::vector<u32> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<u32>(std::stoull(shortArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<s32> Application::getAs(char shortArg)
{
    std::vector<s32> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<s32>(std::stoll(shortArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<u64> Application::getAs(char shortArg)
{
    std::vector<u64> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(std::stoull(shortArgs[idx].second[i]));
    }
    return result;
}

template <>
std::vector<s64> Application::getAs(char shortArg)
{
    std::vector<s64> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(std::stoll(shortArgs[idx].second[i]));
    }
    return result;
}

template <>
std::vector<f32> Application::getAs(char shortArg)
{
    std::vector<f32> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(static_cast<f32>(std::stod(shortArgs[idx].second[i])));
    }
    return result;
}

template <>
std::vector<f64> Application::getAs(char shortArg)
{
    std::vector<f64> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(std::stod(shortArgs[idx].second[i]));
    }
    return result;
}

template <>
std::vector<std::string> Application::getAs(char shortArg)
{
    std::vector<std::string> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(shortArgs[idx].second[i]);
    }
    return result;
}

template <>
std::vector<std::filesystem::path> Application::getAs(char shortArg)
{
    std::vector<std::filesystem::path> result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    for (u64 i = 0; i < shortArgs[idx].second.size(); i++)
    {
        result.push_back(std::filesystem::path(shortArgs[idx].second[i]));
    }
    return result;
}

template <class T>
T Application::getAs(char shortArg)
{
    T result;
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    if (std::is_signed<T>::value)
    {
        result = static_cast<T>(std::stoll(shortArgs[idx].second[0]));
    }
    else if (std::is_unsigned<T>::value)
    {
        result = static_cast<T>(std::stoull(shortArgs[idx].second[0]));
    }
    else if (std::is_floating_point<T>::value)
    {
        result = static_cast<T>(std::stod(shortArgs[idx].second[0]));
    }
    return result;
}

template <>
std::string Application::getAs(char shortArg)
{
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    return shortArgs[idx].second[0];
}

template <>
bool Application::getAs(char shortArg)
{
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    return (toAllUpper(shortArgs[idx].second[0]) == "TRUE");
}

template <>
std::filesystem::path Application::getAs(char shortArg)
{
    u64 idx = std::numeric_limits<u64>::max();
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg)
        {
            idx = i;
            break;
        }
    }
    if (idx == std::numeric_limits<u64>::max())
    {
        throw(std::runtime_error("Argument not found!"));
    }
    return std::filesystem::path(shortArgs[idx].second[0]);
}

std::vector<char> Application::processShortArgs(std::string shortArgs)
{
    std::vector<char> result;
    for (u64 i = 1; i < shortArgs.size(); i++)
    {
        result.push_back(shortArgs[i]);
    }
    return result;
}

bool Application::hasArg(const std::string& longArg)
{
    for (u64 i = 0; i < longArgs.size(); i++)
    {
        if (longArgs[i].first == longArg) return true;
    }
    return false;
}

bool Application::hasArg(char shortArg)
{
    for (u64 i = 0; i < shortArgs.size(); i++)
    {
        if (shortArgs[i].first == shortArg) return true;
    }
    return false;
}

std::filesystem::path Application::getExefsDir()
{
    std::filesystem::path result(args[0]);
    result = result.parent_path();
    return result;
}

} // namespace tmc
