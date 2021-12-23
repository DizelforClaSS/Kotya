#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <codecvt>
#include <vector>
#include <filesystem>
#include <bitset>
#include <map>
#include <math.h>
 #include "Windows.h"
using namespace std;
namespace fs = std::filesystem;




struct MainPart
{
    // Обязательная сигнатура, равна 0x06054b50
    uint32_t signature = 0;
    // Минимальная версия для распаковки
    uint16_t versionToExtract = 1;
    // Всего записей в Central Directory
    uint16_t totalCentralDirectoryRecord;

};

struct CentralDirectoryFileHeader
{
    // Обязательная сигнатура, равна 0x02014b50
    uint32_t signature = 0;
    // Смещение до структуры LocalFileHeader
    uint32_t localFileHeaderOffset;
};

struct LocalFileHeader
{
    // Обязательная сигнатура, равна 0x04034b50
    uint32_t signature = 0;
    //Тип файл или папка
    uint16_t type;
    // Сжатый размер
    uint32_t compressedSize = 0;
    // Несжатый размер
    uint32_t uncompressedSize = 0;

    // Метод сжатия (0 - без сжатия, 1 - неадаптивное сжатие, 2 - адаптивное)
    uint16_t compressionMethod = 0;

    uint16_t extraDataLength = 0;
    // Длина название файла
    uint16_t filenameLength = 0;
};

struct EndOfArchive
{
    // Обязательная сигнатура, равна 0x02014b50
    uint32_t signature = 0;
    // Смещение до структуры CentralDirectoryFileHeader
    uint32_t CentralDirectoryFileHeader;
};

struct arguments
{
    int type = 0; // Что сделать с файлом
    string archivePath; //путь куда сохранить архив
    string directoryPath; // Путь файла для архивации
    int compression = 0; //
    int labAddition = 0; // Номер лабы
};

#pragma pack(push)
#pragma pack(1)
struct frequencyStructure
{
    int amount;
    unsigned char letter;
};
#pragma pack(pop)

struct friquency_structure_double
{
    long long amount;
    long long prev_amount;
};

#endif // STRUCTURES_H
