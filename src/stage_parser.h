#ifndef _STAGE_PARSER_H_
#define _STAGE_PARSER_H_

#include <string>
#include <fstream>
#include "stage.h"

void parse_stage(sStage* to_fill, std::string file_name) {
    std::string curr_line;
    std::ifstream stage_file(file_name);

    if (stage_file.is_open()) {
        getline(stage_file, curr_line);

        assert(curr_line != "define items/n" && "Stage file format issue");

        while(getline(stage_file, curr_line)) {
            if (curr_line == "define stage/n")
                break;

            //switch() {};
            
        }
    }
}

#endif