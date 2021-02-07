#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <unistd.h>
#include <limits.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
json fileTree = nullptr;
uint asarHeaderSize;

namespace asarunpackfile {

    pair<int, string> seekFilePos(string path, json node, string curpath) {
        vector <string> pathSegments = functions::split(path, '/');
        string filename = pathSegments[pathSegments.size() - 1];
        json json = node;
        for(auto pathSegment: pathSegments) {
            if(pathSegment.length() == 0 || json.is_null() || json["files"].is_null())
                continue;
            json = json["files"][pathSegment];
        }
        if(!json.is_null())
            return make_pair<int, string>(json["size"].get<int>(), json["offset"].get<std::string>());
        return make_pair<int, string>(-1, "");
     }

    string getFileContent(string filename) {
        pair<int, string> p = seekFilePos(filename, fileTree, "");
        if(p.first != -1) {
            std::ifstream asarArchive;
            asarArchive.open("res.neu", std::ios::binary);
            if (!asarArchive) {
                perror("res.neu is missing");
                return "";
            }
            uint uSize = p.first;
            uint uOffset = std::stoi(p.second);

            std::vector<char>fileBuf ( uSize );
            asarArchive.seekg(asarHeaderSize + uOffset);
            asarArchive.read(fileBuf.data(), uSize);
            std::string fileContent(fileBuf.begin(), fileBuf.end());
            return fileContent;
       }
       else {
           return "";
       }
    }

    void makeFileTree() {
        std::ifstream asarArchive;
        asarArchive.open("res.neu", std::ios::binary);
        if (!asarArchive) {
            perror("res.neu is missing");
            return;
        }

        char *sizeBuf = new char[8];
        asarArchive.read(sizeBuf, 8);
        uint uSize = *(uint *)(sizeBuf + 4) - 8;

        delete[] sizeBuf;

        asarHeaderSize = uSize + 16;
        std::vector<char> headerBuf(uSize);
        asarArchive.seekg(16); // skip header
        asarArchive.read(headerBuf.data(), uSize);
        json files;
        std::string headerContent(headerBuf.begin(), headerBuf.end());
        try {
            files = json::parse(headerContent);
        }
        catch(exception e) {
            // TODO: log to file
        }
        fileTree = files;
    }

}
