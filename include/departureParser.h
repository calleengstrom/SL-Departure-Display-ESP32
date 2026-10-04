#pragma once
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "../utills/depature_model.h"

void parseDepartures(const std::string& jsonString,depature_model& lcd_text);