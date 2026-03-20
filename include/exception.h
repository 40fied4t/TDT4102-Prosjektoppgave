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
    std::string expectedExtension;
public:
    WrongFileExtension(std::filesystem::path fileName, std::string expectedExtension):
    fileName{fileName}, 
    expectedExtension{expectedExtension}
    {}

    const char* what() const noexcept override {return "Feil filtype!";}
    std::filesystem::path getPath() {return fileName;}
    std::string getExtension() {return expectedExtension;}
};