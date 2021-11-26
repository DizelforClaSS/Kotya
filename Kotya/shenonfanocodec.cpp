#include "shenonfanocodec.h"

ShenonFanoCodec::ShenonFanoCodec()
{

}
void ShenonFanoCodec::stepOfAlgorithm(map<unsigned char, vector<bool>>& table, vector< frequencyStructure >& part, int start, int end)
{
    if (end == 0)
    {
        end = part.size();
    }

    int partSize = 0; //Размер группы
    for (int i = start; i < end; i++)
    {
        partSize += part[i].amount;
    }

    int tempSumm = part[start].amount;
    table[part[start].letter].push_back(false);
    int ptr = start;

    while (abs(partSize / 2 - tempSumm) > abs(partSize / 2 - tempSumm - part[ptr + 1].amount)) //Не превысит ли 1 часть символ по колич информации Вторую
    {
        ptr++;
        tempSumm += part[ptr].amount;
        table[part[ptr].letter].push_back(false);
    }
    for (int i = ptr + 1; i < end; i++) //Правая часть группы (с 1)
    {
        table[part[i].letter].push_back(true);
    }

    if (ptr != start) //Левая часть
    {
        stepOfAlgorithm(table, part, start, ptr + 1);
    }
    if (ptr != end - 2) //Правая часть
    {
        stepOfAlgorithm(table, part, ptr + 1, end);
    }

}

void ShenonFanoCodec::encode(vector<unsigned char> buffer, vector<unsigned char>& compressedBuffer, vector<  frequencyStructure >& frequency)
{
    vector< pair <int, unsigned char> > vect(256); // количество / символ
    map<unsigned char, vector<bool>> table; //таблица  чар / код
    vector <bool> temp; //Временный код

    for (int i = 0; i < vect.size(); i++)
    {
        vect[i].second = i; //Символ
    }
    for (int i = 0; i < buffer.size(); i++)
    {
        vect[buffer[i]].first++; //читают кол символов

    }
    sort(vect.begin(), vect.end(), greater<>()); //Сортируют в порядке неубывания
    for (int i = 0; i < vect.size(); i++)
    {
        if (vect[i].first != 0)
        {
            frequencyStructure tempFrequency;
            tempFrequency.amount = vect[i].first;
            tempFrequency.letter = vect[i].second;
            frequency.push_back(tempFrequency);
            table.insert(make_pair(vect[i].second, vector <bool>())); //Вставляют символ с кодом
        }
    }

    stepOfAlgorithm(table, frequency);

    bitset<8> tempBits;
    int tempBitsPtr = 0;
    unsigned long tempTransform;
    for (int i = 0; i < buffer.size(); i++)
    {
        for (int j = 0; j < table[buffer[i]].size(); j++)
        {
            tempBits[tempBitsPtr] = table[buffer[i]][j];
            if (tempBitsPtr == 7)
            {
                tempBitsPtr = 0;
                tempTransform = tempBits.to_ulong();
                compressedBuffer.push_back(static_cast<unsigned char>(tempTransform));

            }
            else
            {
                tempBitsPtr++;
            }
        }
    }
    if (tempBitsPtr != 0)
    {
        tempTransform = tempBits.to_ulong();
        compressedBuffer.push_back(static_cast<unsigned char>(tempTransform));
    }
}



void ShenonFanoCodec::adaptiveEncode(vector<unsigned char> buffer, vector<unsigned char>& compressed_buffer)
{
    //vector< pair <int, unsigned char> > vect(256);
    vector<  frequencyStructure > frequency;
    map<unsigned char, vector<bool>> table;
    vector <bool> temp;

    frequencyStructure frequencyStruct;
    for (int i = 0; i < 256; i++)
    {
        frequencyStruct.amount = 1;
        frequencyStruct.letter = i;
        frequency.push_back(frequencyStruct);
        table.insert(make_pair(frequencyStruct.letter, vector <bool>()));
    }
    stepOfAlgorithm(table, frequency);

    bitset<8> tempBits;

    int tempBitsPtr = 0;
    unsigned long tempTransform;
    for (int i = 0; i < buffer.size(); i++)
    {
        for (int j = 0; j < table[buffer[i]].size(); j++)
        {
            tempBits[tempBitsPtr] = table[buffer[i]][j];
            if (tempBitsPtr == 7)
            {
                tempBitsPtr = 0;
                tempTransform = tempBits.to_ulong();
                compressed_buffer.push_back(static_cast<unsigned char>(tempTransform));
            }
            else
            {
                tempBitsPtr++;
            }
        }
        if (i % 100 == 0)
        {

            auto result1 = std::find_if(frequency.begin(), frequency.end(), [tst = buffer[i]](const frequencyStructure& x)
            { return (tst == x.letter); });
            if (result1 != frequency.end())
            {
                (*result1).amount++;
            }
            sort(frequency.begin(), frequency.end(), [](const frequencyStructure& x, const frequencyStructure& y)
            { return ((x.amount > y.amount) || ((x.amount == y.amount) && (x.letter > y.letter))); });
            for (int j = 0; j < frequency.size(); j++)
            {
                table[frequency[j].letter].resize(0);
            }
            stepOfAlgorithm(table, frequency);
        }
    }


    if (tempBitsPtr != 0)
    {
        tempTransform = tempBits.to_ulong();
        compressed_buffer.push_back(static_cast<unsigned char>(tempTransform));
    }
}

void ShenonFanoCodec::mapTransforming(map<unsigned char, vector<bool>> table, map<string, unsigned char>& tableForDecoding, vector< frequencyStructure> frequency)
{
    tableForDecoding.clear();
    string tempString;
    for (int i = 0; i < table.size(); i++)
    {
        tempString = "";
        for (int j = 0; j < table[frequency[i].letter].size(); j++)
        {
            if (table[frequency[i].letter][j])
            {
                tempString += "1";
            }
            else
            {
                tempString += "0";
            }
        }
        tableForDecoding.insert(make_pair(tempString, frequency[i].letter));
    }
}


void ShenonFanoCodec::decode(vector<unsigned char>& buffer, vector<unsigned char> compressedBuffer, vector< frequencyStructure> frequency, int uncompressedSize)
{
    map<unsigned char, vector<bool>> table;
    for (int i = 0; i < frequency.size(); i++)
    {
        table.insert(make_pair(frequency[i].letter, vector <bool>()));
    }
    stepOfAlgorithm(table, frequency);

    map<string, unsigned char> tableForDecoding;
    string tempString;
    mapTransforming(table, tableForDecoding, frequency);
    bitset<8> temp_bits;
    tempString = "";
    int ptrOfTempBits;

    for (int i = 0; i < compressedBuffer.size(); i++)
    {
        temp_bits = compressedBuffer[i];
        ptrOfTempBits = 0;
        while (ptrOfTempBits != 8)
        {
            if (temp_bits[ptrOfTempBits] == 0)
            {
                tempString += "0";
            }
            else
            {
                tempString += "1";
            }
            ptrOfTempBits++;
            if (tableForDecoding.count(tempString) == 1)
            {
                buffer.push_back(tableForDecoding[tempString]);
                if (uncompressedSize == buffer.size())
                    break;
                tempString = "";
            }
        }
    }
}

void ShenonFanoCodec:: adaptiveDecode(vector<unsigned char>& buffer, vector<unsigned char> compressedBuffer, int uncompressedSize)
{
    map<unsigned char, vector<bool>> table;
    vector< frequencyStructure> frequency;
    for (int i = 0; i < 256; i++)
    {
        frequencyStructure tempStructure;
        tempStructure.amount = 1;
        tempStructure.letter = i;
        frequency.push_back(tempStructure);
        table.insert(make_pair(tempStructure.letter, vector <bool>()));
    }
    stepOfAlgorithm(table, frequency);

    map<string, unsigned char> tableForDecoding;
    string tempString;

    mapTransforming(table, tableForDecoding, frequency);
    bitset<8> temp_bits;
    tempString = "";
    int ptrOfTempBits;
    int ptrSpeed = -1;

    for (int i = 0; i < compressedBuffer.size(); i++)
    {
        temp_bits = compressedBuffer[i];
        ptrOfTempBits = 0;
        while (ptrOfTempBits != 8)
        {
            if (!temp_bits[ptrOfTempBits])
            {
                tempString += "0";
            }
            else
            {
                tempString += "1";
            }

            ptrOfTempBits++;
            if (tableForDecoding.count(tempString) == 1)
            {
                buffer.push_back(tableForDecoding[tempString]);
                ptrSpeed++;
                if (uncompressedSize == buffer.size())
                {
                    cout << "endddd";
                    break;
                }
                if (ptrSpeed % 10 == 0)
                {
                    auto result1 = std::find_if(frequency.begin(), frequency.end(), [tst = tableForDecoding[tempString]](const frequencyStructure& x)
                    { return (tst == x.letter); });
                    if (result1 != frequency.end())
                    {
                        (*result1).amount++;
                    }
                    sort(frequency.begin(), frequency.end(), [](const frequencyStructure& x, const frequencyStructure& y)
                    { return ((x.amount > y.amount) || ((x.amount == y.amount) && (x.letter > y.letter))); });
                    for (int j = 0; j < frequency.size(); j++)
                    {
                        table[frequency[j].letter].resize(0);
                    }
                    stepOfAlgorithm(table, frequency);

                    mapTransforming(table, tableForDecoding, frequency);
                }
                tempString = "";
            }
        }
    }
}
