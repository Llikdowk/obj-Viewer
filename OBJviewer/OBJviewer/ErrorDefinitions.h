#pragma once

static void fileNotFoundError(std::string path) {
    std::string msg = "ERROR: The file " + path + " cannot be found";
    throw std::invalid_argument(msg);
}

static void parsingError(std::string s, int linenum) {
    std::string msg = "ERROR: parsing " + s + " at line " + std::to_string(linenum);
    throw std::invalid_argument(msg);
}