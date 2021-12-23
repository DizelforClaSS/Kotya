#ifndef ARCHIVATOR_H
#define ARCHIVATOR_H

#include <QObject>
#include "Structures.h"
#include "shenonfanocodec.h"
#include "arifmeticalcodec.h"
#include "rlecodec.h"
#include "hemmingcodec.h"

namespace fs = std::filesystem;

class Archivator : QObject
{
    //Q_OBJECT//////////////////////////////////////////////////////////////////////
    MainPart mainPartConstructor(int directoryRecords);
private:
    int max_freqency = 16383;
    LocalFileHeader fileHeaderConstructor(string fileName, bool isFile, int typeCompression, string fileFullPath = " ");

    string archiveExtention = ".kotik";

    uint32_t ourSignature = 0x6b6f7469; //koti
    uint32_t signature_of_LocalFileHeader = 0x04034b50;
    uint32_t signature_of_CentralDirectoryFileHeader = 0x02014b50;
    uint32_t ourEndOfArchive = 0x02014b51;

    //
    //int max_freqency = 16383;
public:
    Archivator();
    void createFile(arguments mainArg, vector<LocalFileHeader> allFiles, vector<string> filePaths, vector<string>  erasedFilePaths, MainPart mainPart);
    void pack(arguments main_arg);
    void unpack(arguments main_arg);
    void testArchive(arguments main_arg);
    void printFiles(arguments main_arg);
};

#endif // ARCHIVATOR_H
