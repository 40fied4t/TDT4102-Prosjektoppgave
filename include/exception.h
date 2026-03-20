#pragma once

#include <stdexcept>
#include <filesystem>

class FileNotOpen : public std::exception 
{
private:
    std::filesystem::path fileName;
public:
    FileNotOpen(std::filesystem::path fileName): fileName{fileName} {}
    const char* what() const noexcept override {return "Kunne ikke åpne fil!";}
    std::filesystem::path getPath() {return fileName;}
};

class WrongFileExtension : public std::exception
{
private:
    std::filesystem::path fileName;
    std::filesystem::path expectedExtension;
public:
    WrongFileExtension(std::filesystem::path fileName, std::filesystem::path expectedExtension):
    fileName{fileName}, 
    expectedExtension{expectedExtension}
    {}

    const char* what() const noexcept override {return "Feil filtype!";}
    std::filesystem::path getPath() {return fileName;}
    std::filesystem::path getExtension() {return expectedExtension;}
};

struct BadFormat : public std::exception
{
    const char* what() const noexcept override {return "Feil formatering";}
};