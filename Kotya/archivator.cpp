#include "archivator.h"

Archivator::Archivator()
{

}
LocalFileHeader Archivator::fileHeaderConstructor(string fileName, bool isFile, int typeCompression, string fileFullPath)
{

    LocalFileHeader fileHeader;
    fileHeader.type = isFile;
    fileHeader.compressionMethod = typeCompression;
    if (isFile)
    {
        fileHeader.compressedSize = fs::file_size(fileFullPath);
        fileHeader.uncompressedSize = fs::file_size(fileFullPath);
    }
    fileHeader.filenameLength = fileName.size();
    return fileHeader;
}

MainPart Archivator::mainPartConstructor(int directoryRecords)
{
    MainPart main_part;
    main_part.signature = ourSignature;
    main_part.totalCentralDirectoryRecord = directoryRecords;
    return main_part;
}

void Archivator::createFile(arguments mainArgs, vector<LocalFileHeader> allFiles, vector<string> filePaths, vector<string>  erasedFilePaths, MainPart mainPart)
{
    std::ofstream out;          // поток для записи

    string tempArchivePath = mainArgs.archivePath + archiveExtention;

    out.open(tempArchivePath, ios::binary); // окрываем файл для записи
    out.write((char*)&mainPart, sizeof(mainPart));

    vector<vector<int> > randPtr(mainPart.totalCentralDirectoryRecord, vector<int>()); //Количество векторов равное колиеству файлов

    vector<unsigned char> compressedBuffer;

    vector< frequencyStructure > frequency;

    vector<CentralDirectoryFileHeader> allDirectoryHeaders(allFiles.size());

    EndOfArchive end_of_archive;



    int ptr;
    int ptrNumbers;

    long FileHeaderOffset = sizeof(mainPart);

    if (mainArgs.labAddition == 3)
    {
        for (int i = 0; i < mainPart.totalCentralDirectoryRecord; i++)
        {

            ptrNumbers = 0;
            ptr = rand() % 16 + 1;
            while (ptr < allFiles[i].compressedSize) {
                randPtr[i].push_back(ptr + ptrNumbers * 2);
                ptrNumbers++;
                ptr += rand() % 16 + 1;
            }
            allFiles[i].compressedSize += randPtr[i].size();
        }
    }

    vector<unsigned char> buffer;

    unsigned char delimiter; //Разделитель

    for (int i = 0; i < allFiles.size(); i++)
    {
        allDirectoryHeaders[i].localFileHeaderOffset = FileHeaderOffset;
        allFiles[i].signature = signature_of_LocalFileHeader;

        //Если это файл
        if (allFiles[i].type) {
            buffer.resize(allFiles[i].uncompressedSize);
            std::ifstream file(filePaths[i], std::ios::binary);
            file.read((char*)buffer.data(), allFiles[i].uncompressedSize);
            file.close();

            if (mainArgs.labAddition == 3) //В 3 лабе разделение на блоки
            {
                ptr = 1;
                for (int j = 0; j < randPtr[i].size(); j++)
                {
                    buffer.emplace(buffer.cbegin() + randPtr[i][j], 'q');
                    buffer.emplace(buffer.cbegin() + 1 + randPtr[i][j], ptr % 255 + 1);
                    ptr++;
                }

            }

            if (mainArgs.labAddition == 4) //в 4 лабе Шенон
            {

                double summ = 0;
                frequency.resize(0);
                vector< pair <int, unsigned char> > vect(256);
                for (int j = 0; j < vect.size(); j++)
                {
                    vect[j].second = j;
                }

                for (int j = 0; j < buffer.size(); j++)
                {
                    vect[buffer[j]].first++;

                }

                sort(vect.begin(), vect.end(), greater<>());

                for (int j = 0; j < vect.size(); j++)
                {
                    frequencyStructure tempStructure;
                    if (vect[j].first != 0)
                    {
                        tempStructure.amount = vect[j].first;
                        tempStructure.letter = vect[j].second;
                        frequency.push_back(tempStructure);
                    }
                }

                for (int j = 0; j < frequency.size(); j++)
                {
                    cout << endl << frequency[j].letter << " ";
                    cout << ((double)frequency[j].amount) / allFiles[i].uncompressedSize << " ";
                    cout << -log(((double)frequency[j].amount) / allFiles[i].uncompressedSize) / log(2);
                    summ += -log(((double)frequency[j].amount) / allFiles[i].uncompressedSize) / log(2) * frequency[j].amount;
                }
                cout << endl << "full information " << summ / 8;
            }

            if (mainArgs.compression == 1)
            {
                ShenonFanoCodec codec;
                compressedBuffer.resize(0);
                frequency.resize(0);
                codec.encode(buffer, compressedBuffer, frequency);
                allFiles[i].extraDataLength = frequency.size() * 5; //Стркуьтура 5 байт весит
                allFiles[i].compressedSize = compressedBuffer.size();
            }

            if (mainArgs.compression == 2)
            {
                ShenonFanoCodec codec;
                compressedBuffer.resize(0);
                codec.adaptiveEncode(buffer, compressedBuffer);
                allFiles[i].extraDataLength = 0;
                allFiles[i].compressedSize = compressedBuffer.size();
            }

            if (mainArgs.compression == 3)
            {
                ArifmeticalCodec codec;
                compressedBuffer.resize(0);
                frequency.resize(0);
                codec.encode(buffer, compressedBuffer, frequency);
                allFiles[i].extraDataLength = frequency.size() * 5;
                allFiles[i].compressedSize = compressedBuffer.size();
            }


            if (mainArgs.compression == 5)
            {
                RleCodec codec;
                compressedBuffer.resize(0);
                codec.encode(buffer, compressedBuffer, delimiter);
                allFiles[i].extraDataLength = 1;
                allFiles[i].compressedSize = compressedBuffer.size();
            }

            if (mainArgs.compression == 6)
            {
                HemmingCodec codec;
                compressedBuffer.resize(0);
                codec.encoding(buffer,compressedBuffer);
                allFiles[i].extraDataLength = 0;
                allFiles[i].compressedSize = compressedBuffer.size();
            }
        }

        out.write((char*)&allFiles[i], sizeof(allFiles[i]));
        out.write((char*)erasedFilePaths[i].data(), erasedFilePaths[i].size());

        if (allFiles[i].type)
        {
            if (mainArgs.compression == 1) //Обычный Шеннон
            {
                if (allFiles[i].extraDataLength != 0)
                {
                    out.write((char*)&frequency[0], allFiles[i].extraDataLength);
                }
                out.write((char*)&compressedBuffer[0], compressedBuffer.size());
                cout << endl << "finish " << erasedFilePaths[i] << endl;
                cout << " before: " << allFiles[i].uncompressedSize;
                cout << " after: " << allFiles[i].compressedSize;

            }
            if (mainArgs.compression == 2) //Адаптивынй
            {
                out.write((char*)&compressedBuffer[0], compressedBuffer.size());
                cout << "finish " << erasedFilePaths[i] << endl;
            }
            if (mainArgs.compression == 3) //Арифм.
            {
                if (allFiles[i].extraDataLength != 0)
                {
                    out.write((char*)&frequency[0], allFiles[i].extraDataLength);
                }
                out.write((char*)&compressedBuffer[0], compressedBuffer.size());
                cout << "finish " << erasedFilePaths[i] << endl;
            }
            if (mainArgs.compression == 4) //Адапт. Арифметический
            {
                out.write((char*)&compressedBuffer[0], compressedBuffer.size());
                cout << "finish " << erasedFilePaths[i] << endl;
            }
            if (mainArgs.compression == 5) //RLE
            {
                if (allFiles[i].extraDataLength != 0)
                {
                    out.write((char*)&delimiter, allFiles[i].extraDataLength);
                }
                out.write((char*)&compressedBuffer[0], compressedBuffer.size());
                cout << "finish " << erasedFilePaths[i] << endl;
            }
            if (mainArgs.compression == 6)//Хемминг
            {
                out.write((char*)&compressedBuffer[0], compressedBuffer.size());
                cout << "finish " << erasedFilePaths[i] << endl;
            }
            if (mainArgs.compression == 0)
            {
                out.write((char*)&buffer[0], buffer.size());
            }
        }
        //Информация о файлу     //Дополнительная информация    //Имя файлф                  //Данные
        FileHeaderOffset += sizeof(LocalFileHeader) + allFiles[i].extraDataLength + allFiles[i].filenameLength + allFiles[i].compressedSize;
    }

    for (int i = 0; i < allFiles.size(); i++)
    {
        allDirectoryHeaders[i].signature = signature_of_CentralDirectoryFileHeader;
    }
    // Сигнатура Смещение к файлу | Сигнатура смеение к файлу | ...
    out.write((char*)&allDirectoryHeaders[0], sizeof(allDirectoryHeaders[0]) * allDirectoryHeaders.size());
    end_of_archive.signature = ourEndOfArchive;
    end_of_archive.CentralDirectoryFileHeader = FileHeaderOffset;
    out.write((char*)&end_of_archive, sizeof(end_of_archive));
    out.close();
}

void Archivator::pack(arguments mainArg)
{
    std::string fullPath = mainArg.directoryPath;
    std::string directory = fullPath;
    std::string mainDirectory = fullPath;

    MainPart mainPart;

    vector<LocalFileHeader> allFiles;
    vector<string> filePath;
    vector<string>  erasedPathFile;

    bool flag = true;

    directory.erase(directory.find_last_of("\\")); //Взяли имя папки или файла
    mainDirectory.erase(0, mainDirectory.find_first_not_of(directory)); //Записываем весь путь до папки (невключая саму папку)

    filePath.push_back(fullPath);

    erasedPathFile.push_back(mainDirectory);

    if (fs::is_directory(mainArg.directoryPath /*fullPath*/))
    {
        allFiles.push_back(fileHeaderConstructor(erasedPathFile.back(), false, mainArg.compression));

        for (const auto& entry : fs::recursive_directory_iterator(fullPath))
        {
            filePath.push_back(entry.path().string());
            erasedPathFile.push_back(filePath.back());
            erasedPathFile.back().erase(0, erasedPathFile.back().find_first_not_of(directory));

            if (fs::is_directory(entry.path())) //Почему не идет дальше по папке не понятно
            {
                allFiles.push_back(fileHeaderConstructor(erasedPathFile.back(), false, mainArg.compression));
            }
            else
            {
                allFiles.push_back(fileHeaderConstructor(erasedPathFile.back(), true, mainArg.compression, entry.path().string()));
            }
        }
    }
    else
    {
        uint32_t signature;
        std::ifstream file(fullPath, std::ios::binary);
        file.read((char*)&signature, sizeof(ourSignature));
        file.close();
        if (signature != ourSignature)
        {
            allFiles.push_back(fileHeaderConstructor(erasedPathFile.back(), true, mainArg.compression, fullPath));
        }
        else
        {
            cout << "архив уже создан";
            flag = false;
        }
    }
    if (flag)
    {
        mainPart = mainPartConstructor(allFiles.size());
        createFile(mainArg, allFiles, filePath, erasedPathFile, mainPart);
    }
}

void Archivator::unpack(arguments main_arg)
{
    std::string path = main_arg.directoryPath;
    std::string path_of_archive = main_arg.archivePath;
    MainPart main_part;
    EndOfArchive end_of_archive;
    vector<CentralDirectoryFileHeader> temp_header;
    LocalFileHeader all_files;
    string path_to_file;
    string  erased_path_to_file;
    string  temp_erased_path_to_file;
    vector<unsigned char> buffer;
    vector< frequencyStructure > frequency;
    std::ifstream file(path_of_archive, std::ios::binary);
    bool flag = true;
    int ptr = 1;
    int shiftToEndFile = 0 - sizeof(EndOfArchive);
    file.read((char*)&main_part, sizeof(main_part));
    unsigned char delimiter = 'a';
    if (main_part.signature != ourSignature)
    {
        cout << "архив не соответствует данной программе";
    }
    else
    {
        file.seekg(shiftToEndFile, ios::end);
        file.read((char*)&end_of_archive, sizeof(end_of_archive));
        if (end_of_archive.signature != ourEndOfArchive)
        {
            cout << "конец архива сбит";
        }
        else
        {
            file.seekg(end_of_archive.CentralDirectoryFileHeader, ios::beg);
            temp_header.resize(main_part.totalCentralDirectoryRecord);
            for (int i = 0; i < main_part.totalCentralDirectoryRecord; i++)
            {
                file.read((char*)&temp_header[i], sizeof(temp_header[i]));
                if (temp_header[i].signature != signature_of_CentralDirectoryFileHeader)
                {
                    cout << temp_header[i].signature;
                    cout << "Несоответствие структуры содержания";
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                for (int i = 0; i < main_part.totalCentralDirectoryRecord; i++)
                {
                    file.seekg(temp_header[i].localFileHeaderOffset, ios::beg);
                    file.read((char*)&all_files, sizeof(all_files));
                    if (all_files.signature != signature_of_LocalFileHeader)
                    {
                        cout << "несоответствие структуры файловой системы";
                        flag = false;
                        break;
                    }
                    erased_path_to_file.resize(all_files.filenameLength);
                    file.read((char*)erased_path_to_file.data(), all_files.filenameLength);
                    if (!all_files.type)
                    {
                        path_to_file = path + erased_path_to_file;
                        fs::create_directories(path_to_file);
                    }
                    else
                    {
                        temp_erased_path_to_file = erased_path_to_file;
                        cout << temp_erased_path_to_file << endl;
                        if (temp_erased_path_to_file.find_last_of("\\") != string::npos)
                        {
                            temp_erased_path_to_file.erase(temp_erased_path_to_file.find_last_of("\\"));
                            path_to_file = path + temp_erased_path_to_file;
                            fs::create_directories(path_to_file);
                        }
                    }

                    if (all_files.type)
                    {
                        path_to_file = path + "\\" + erased_path_to_file;
                        vector<unsigned char> compressed_buffer(all_files.compressedSize);
                        if (all_files.extraDataLength / sizeof(frequencyStructure) != 0)
                        {
                            frequency.resize(all_files.extraDataLength / sizeof(frequencyStructure));
                            file.read((char*)&frequency[0], all_files.extraDataLength);
                        }
                        else if (all_files.extraDataLength == 1 && all_files.compressionMethod == 5)
                        {
                            file.read((char*)&delimiter, all_files.extraDataLength);
                        }
                        file.read((char*)compressed_buffer.data(), all_files.compressedSize);
                        std::ofstream out;
                        buffer.resize(0);
                        if (all_files.compressionMethod == 1)
                        {
                            ShenonFanoCodec codec;
                            codec.decode(buffer, compressed_buffer, frequency, all_files.uncompressedSize);
                        }

                        if (all_files.compressionMethod == 2)
                        {
                            ShenonFanoCodec codec;
                            codec.adaptiveDecode(buffer, compressed_buffer, all_files.uncompressedSize);
                        }
                        if (all_files.compressionMethod == 3)
                        {
                            ArifmeticalCodec codec;
                            codec.decode(buffer, compressed_buffer, frequency, all_files.uncompressedSize);
                        }

                        if (all_files.compressionMethod == 5)
                        {
                            RleCodec codec;
                            codec.decode(buffer, compressed_buffer, delimiter);
                        }
                        if (all_files.compressionMethod == 6)
                        {
                            HemmingCodec codec;
                            codec.decoding(buffer, compressed_buffer, all_files.uncompressedSize);
                        }


                        if (main_arg.labAddition == 3)
                        {
                            for (int j = 0; j < compressed_buffer.size() - 1; j++)
                            {
                                if (compressed_buffer[j - (ptr - 1) * 2] == 'q')
                                {
                                    if (compressed_buffer[j + 1 - (ptr - 1) * 2] == ptr % 255 + 1)
                                    {
                                        cout << ptr % 255 + 1 << endl;
                                        compressed_buffer.erase(compressed_buffer.begin() + j - (ptr - 1) * 2);
                                        compressed_buffer.erase(compressed_buffer.begin() + j - (ptr - 1) * 2);
                                        ptr++;
                                    }
                                }
                            }
                        }
                        out.open(path_to_file, ios::binary); // окрываем файл для записи
                        if (all_files.compressionMethod != 0)
                        {
                            out.write((char*)&buffer[0], buffer.size());
                        }
                        else
                        {
                            out.write((char*)&compressed_buffer[0], compressed_buffer.size());
                        }
                        out.close();
                    }
                }
            }
        }
    }
}

void  Archivator::testArchive(arguments main_arg)
{
    std::string path = main_arg.directoryPath; //путь к директории
    std::string archivePath = main_arg.archivePath; //путь к файлу

    MainPart mainPart; //Основаная часть сигнатуры

    vector<CentralDirectoryFileHeader> tempHeader; //Информация перед файлами

    LocalFileHeader allFiles; //хз

    string filePath; //путь к файлу

    EndOfArchive archiveEnd; //сигнатурахвоста файла
    int endFileShift = 0 - sizeof(EndOfArchive); //смещение относительно конца к начау хвоста файла-архива

    std::ifstream file(archivePath, std::ios::binary);
    bool flag = true;

    file.read((char*)&mainPart, sizeof(mainPart)); //Читаем сигнатуру основной части опеределяющий архив

    if (mainPart.signature != ourSignature) //Проверка сигнатуры
    {
        cout << "архив не соответствует данной программе";
        flag = false;
    }
    else
    {
        file.seekg(endFileShift, ios::end); //Переходим на хвост архива, проверяем сигнатуры
        file.read((char*)&archiveEnd, sizeof(archiveEnd));
        if (archiveEnd.signature != ourEndOfArchive)
        {
            cout << "конец архива сбит";
        }
        else
        {
            file.seekg(archiveEnd.CentralDirectoryFileHeader, ios::beg); //переходим к части , описывающей информацию о всеех файлах в архиве
            tempHeader.resize(mainPart.totalCentralDirectoryRecord);

            for (int i = 0; i < mainPart.totalCentralDirectoryRecord; i++)
            {
                file.read((char*)&tempHeader[i], sizeof(tempHeader[i]));
                if (tempHeader[i].signature != signature_of_CentralDirectoryFileHeader) //Проверка что архив наш
                {
                    cout << "Несоответствие структуры содержания";
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                for (int i = 0; i < mainPart.totalCentralDirectoryRecord; i++) //Проход по всем файлам
                {
                    file.seekg(tempHeader[i].localFileHeaderOffset, ios::beg);
                    file.read((char*)&allFiles, sizeof(allFiles));
                    if (allFiles.signature != signature_of_LocalFileHeader)
                    {
                        cout << "несоответствие структуры файловой системы";
                        flag = false;
                        break;
                    }
                }
            }
        }
    }
    if (flag)
    {
        cout << "структура архива в порядке";
    }
};

void  Archivator:: printFiles(arguments mainArgs)
{
    std::string path = mainArgs.directoryPath;
    std::string archivePath = mainArgs.archivePath;

    MainPart mainPart;
    vector<CentralDirectoryFileHeader> tempHeader;
    LocalFileHeader allFiles;

    string filePath;

    EndOfArchive endArchive;
    int shiftEndFile = 0 - sizeof(EndOfArchive);
    std::ifstream file(archivePath, std::ios::binary);

    file.read((char*)&mainPart, sizeof(mainPart));
    file.seekg(shiftEndFile, ios::end);

    file.read((char*)&endArchive, sizeof(endArchive));
    file.seekg(endArchive.CentralDirectoryFileHeader, ios::beg);

    tempHeader.resize(mainPart.totalCentralDirectoryRecord);
    for (int i = 0; i < mainPart.totalCentralDirectoryRecord; i++) //считываем в массив структуры информацию к файлам
    {
        file.read((char*)&tempHeader[i], sizeof(tempHeader[i]));
    }

    for (int i = 0; i < mainPart.totalCentralDirectoryRecord; i++)
    {
        file.seekg(tempHeader[i].localFileHeaderOffset, ios::beg); //Перемещаемся к инфорамции о файле
        file.read((char*)&allFiles, sizeof(allFiles)); //считываем инфорамцию о файле
        //        erasedFilePath.resize(allFiles.filenameLength); //задаем строке длину по имени файла
        //        file.read((char*)erasedFilePath.data(), allFiles.filenameLength); //строку записываем имя файла
        //        cout << erasedFilePath << endl;
    }
};

