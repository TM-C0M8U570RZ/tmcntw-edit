#pragma once

#include <vector>
#include <string>
#include <utility>
#include <type_traits>
#include <limits>
#include <stdexcept>
#include "tmc_global_types.h"
#include <filesystem>

namespace tmc {

class Application
{
public:

    static Application* getGlobalInstance(int argc = 0, char** argv = nullptr);

    template <class T>
    T getAs(const std::string& longArg);

    template <class T>
    T getAs(char shortArg);

    // in case the user wants to use their own parser outside of main.cpp
    inline std::vector<std::string> getRawArgs()
    {
        return args;
    }

    inline std::vector<std::filesystem::path> getFilePaths()
    {
        return filepaths;
    }

    bool hasArg(const std::string& longArg);

    bool hasArg(char shortArg);

    std::filesystem::path getExefsDir();

private:
    Application(int argc, char** argv);
    std::vector<std::string> args;
    std::vector<std::pair<char, std::vector<std::string>>> shortArgs;
    std::vector<std::pair<std::string, std::vector<std::string>>> longArgs;
    std::vector<std::filesystem::path> filepaths;
    std::filesystem::path exefsDir;

    std::vector<char> processShortArgs(std::string shortArgs);
};

} // namespace tmc
