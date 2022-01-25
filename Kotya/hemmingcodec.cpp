#include "hemmingcodec.h"

HemmingCodec::HemmingCodec()
{

}

/*Формирует последовательности вида

   n строка -  2^n  0  2^n 1 b и так до конца
    следовательно
    6 строка -  64 - 0  64 - 1
    7 строка вся из 1 , так как 2^7 = 128

    Используется в кодировании ХЕММИНГА
    указывает на управляемые биты Управляющим битом
*/
void HemmingCodec:: fillMatrix()
{
    int flagSetOne;
    int flagSetZero;
    for (int i = 0; i < 7; i++)
    {
        flagSetOne = 0;
        flagSetZero = 0;
        for (int j = pow(2, i); j < 128; j++)
        {
            if (flagSetOne < pow(2, i) || flagSetZero == 0)
            {
                flagSetZero++;
                flagSetOne = flagSetZero;
                matrix[i][j] = 1;
            }
            else if (flagSetZero != 0)
            {
                flagSetZero--;
            }
        }
    }
    for (int j = 0; j < 128; j++)
    {
        matrix[7][j] = 1;
    }

}

//Функция получения управляющего байта
void HemmingCodec::addinationallByte(unsigned char& additionalByte, vector <int> tempBuffer)
{
    bitset<8> tempBits;
    int summ;
    for (int i = 0; i < 7; i++)
    {
        summ = 0;
        for (int j = 0; j < 128; j++)
        {
            summ +=tempBuffer[j] * matrix[i][j]; // если бит контролируеться, то он идет в сумму
        }
        tempBits[i] = summ % 2; // Выставляем значения контролирующим битам если четно то 0, нечетно 1
    }
    //Этот блок для последнего контролирующего бита
    summ = 0;
    for (int i = 0; i < 7; i++)
    {
        summ += tempBits[i]; //Понять для чего суммирует управляющие биты/ скорее всего контролирует, остальные управляющие биты
    }
    for (int j = 0; j < 128; j++)
    {
        summ += tempBuffer[j];
    }
    tempBits[7] = summ % 2;

    unsigned long tempToTransform;
    tempToTransform = tempBits.to_ulong(); // возвращаем значение байта
    additionalByte = static_cast<unsigned char>(tempToTransform); //переводим в доп Байт
}

/*Помехозащитное кодирование по Хеммингу
    Читаем строки
*/
void HemmingCodec::encoding(vector<unsigned char> buffer, vector<unsigned char>& compressedBuffer)
{
    fillMatrix(); // Получаем матрицу для управлящих битов
    int newBufferSize = buffer.size() + (15 - buffer.size() % 15) % 15; // Размер буффера + (15 - размер буффера % 15) | Зачм последнее деление по модулю не понятно :/

    buffer.resize(newBufferSize); // Увеличиваем размер буффера на количество управляющих байтов
    vector <int> tempBuffer(128); //Временный буффер битов для 7 байт
    unsigned char additionalByte;

    bitset<8> tempBits;

    int zeroIndex; //индекс управляющего Бита
    int bitsetIndex; //индекс Нового байта ( так как он расширяеться из-за управляющих битов)
    int bufferIndex = 0;
    for (int i = 0; i < buffer.size()/15; i++) //Делим входной поток на строки по 15 байт
    {
        zeroIndex = 4;
        bitsetIndex = 8;
        for (int j = 3; j < 128; j++) // Начинаем с 4 бита
        {
            // Весь этот бллок раскладывает байты на биты
            if (j == zeroIndex)
            {
                zeroIndex *= 2; // Идем по степеням 2
            }
            else
            {
                //Если набралось 8 бит
                if (bitsetIndex == 8)
                {
                    tempBits = buffer[bufferIndex]; //Получаем битовое значение символа
                    compressedBuffer.push_back(buffer[bufferIndex]); // Вносим новый байт в буффер сжатия
                    bufferIndex++; //Переход на следущий байт
                    bitsetIndex = 0;
                }
                tempBuffer[j] = tempBits[bitsetIndex]; //Выставляем бит из чара (соответственно все станут туда куда нужно)
                bitsetIndex++;
            }
        }
        addinationallByte(additionalByte, tempBuffer);
        compressedBuffer.push_back(additionalByte);
        //По итогу считали 7 байт и 8 байтом выставляем как управляющий всей строкой

    }
}
//Показывают где ошибка, но фикса нет
void HemmingCodec::fix(vector <int>& tempBuffer, int &errorBit)
{
    bitset<8> tempBits;
    int summ;
    bool flag = false;
    errorBit = -1;

    for (int i = 0; i < 7; i++)
    {
        summ = 0;
        for (int j = 0; j < 128; j++)
        {
            summ += tempBuffer[j] * matrix[i][j];
        }
        tempBits[i] = summ % 2; //четность
        if (tempBits[i]) //Непонятно
        {
            flag = true;
        }
    }
    summ = 0;
    for (int j = 0; j < 128; j++)
    {
        summ += tempBuffer[j];
    }
    tempBits[7] = summ % 2;
    if (!flag)
    {
        if (tempBits[7])
        {
            cout << "Error in additional bit";
        }
    }
    else
    {
        if (tempBits[7])
        {
            errorBit = 0;
            for (int i = 6; i >= 0; i--)
            {
                errorBit = errorBit * 2 + tempBits[i];
            }
            cout << endl << "Error in one bit " << errorBit << endl;
        }
        else // почему при всех четных управляющих битах, выдает ошибку?
        {
            cout << "double error";
        }
    }
};

void HemmingCodec::decoding(vector<unsigned char> &buffer, vector<unsigned char> compressedBuffer, int uncompressedSize)
{
    fillMatrix();
    vector <int> tempBuffer(128);
    bitset<8> addinationalByte;
    bitset<8> tempBits;
    int zeroIndex;
    int bitsetIndex;
    int inAddinationalByteIndex=1;
    int inBufferIndex = 0;
    int errorBit;
    unsigned long tempForTransform;

    for (int i = 0; i < compressedBuffer.size() / 16; i++)
    {
        zeroIndex = 1;
        inAddinationalByteIndex = 0;
        bitsetIndex = 8;
        addinationalByte = compressedBuffer[inBufferIndex + 15];
        tempBuffer[0] = addinationalByte[7];
        for (int j = 1; j < 128; j++)
        {
            if (j == zeroIndex)
            {
                tempBuffer[zeroIndex] = addinationalByte[inAddinationalByteIndex];
                inAddinationalByteIndex++;
                zeroIndex *= 2;
            }
            else
            {
                if (bitsetIndex == 8)
                {
                    tempBits = compressedBuffer[inBufferIndex];
                    inBufferIndex++;
                    bitsetIndex = 0;
                }
                tempBuffer[j] = tempBits[bitsetIndex];
                bitsetIndex++;
            }
        }

        inBufferIndex++;
        fix(tempBuffer, errorBit);
        if (errorBit != -1) // Если ошибка в 2-ух битах сразу, то ничего починить не удается
        {
            tempBuffer[errorBit] = !tempBuffer[errorBit];
        }

        zeroIndex = 4;
        bitsetIndex = 0;
        for (int j = 3; j < 128; j++)
        {
            if (j == zeroIndex)
            {
                zeroIndex *= 2;
            }
            else
            {
                tempBits[bitsetIndex] = tempBuffer[j];
                bitsetIndex++;
                if (bitsetIndex == 8)
                {
                    tempForTransform = tempBits.to_ulong();
                    if (buffer.size() < uncompressedSize)
                    {
                        buffer.push_back(static_cast<unsigned char>(tempForTransform));
                    }
                    bitsetIndex = 0;
                }
            }
        }
    }
}
