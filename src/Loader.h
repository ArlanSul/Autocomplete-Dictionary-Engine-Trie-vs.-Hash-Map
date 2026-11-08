#pragma once

#include <string>
#include <cstddef>
#include "Dictionary.h"


std::size_t loadDictionary(Dictionary& dict, const std::string& filePath = "data/words.txt");