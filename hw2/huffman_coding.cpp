#include "huffman_coding.h"

void HuffmanCoding::ResetAll()
{
    DEBUG("In ResetAll");
    memset(dataFreq, 0, CHAR_MAX);
    left = right = top = root = NULL;
    codeMap.clear();
    fixCodeMap.clear();
}


bool HuffmanCoding::ReadFileInBinary(string fileName)
{
    DEBUG("In ReadFileInBinary");
    ResetAll();

    ifstream fin;
    fin.open(fileName, ios::binary);

    // because >> will skip space, need to cancel it
    fin.unsetf(ios::skipws);

    if(!fin){ 
        return false; 
    }

    unsigned char ch;

    while(fin>>ch)
    {   
        dataFreq[(int)ch]++;
    } 

    fin.close();

    return true;
}

bool HuffmanCoding::SetData(string fileName, bool checkFixed)
{
    DEBUG("In SetData");
    if(ReadFileInBinary(fileName))
    {
        if(checkFixed)
        {
            for(int i = 0; i < CHAR_MAX; i++)
            {
                if(dataFreq[i] != 0)
                {
                    minHeapForFix.push(new MinHeapNode((unsigned char)i, dataFreq[i], false));
                }
            }
            GenerateFixCodeMap();
            return true;
        }
        else
        {
            for(int i = 0; i < CHAR_MAX; i++)
            {
                if(dataFreq[i] != 0)
                {
                    minHeap.push(new MinHeapNode((unsigned char)i, dataFreq[i], false));
                }
            }
            GenerateCodeMap();
            return true;
        }
    }

    return false;
}

/***********************
* dynamic huffman code *
************************/

void HuffmanCoding::CreateMinHeap()
{
    DEBUG("In CreateMinHeap");
    // if minHeap only have one, then we can get a tree root from top
    while (minHeap.size() != 1) {
 
        // get two minimum frequency node
        left = minHeap.top();
        minHeap.pop();
 
        right = minHeap.top();
        minHeap.pop();

        // create new internal node to bind two minimum frequency node
        top = new MinHeapNode('\0', left->freq + right->freq, true);
 
        top->left = left;
        top->right = right;
 
        minHeap.push(top);
    }

    // get root and clear minHeap
    root = minHeap.top();
    minHeap.pop();
}

void HuffmanCoding::GenerateCodeMap()
{
    DEBUG("In GenerateCodeMap");
    CreateMinHeap();
    GenerateCodeMapRecursive(root, "");
}

void HuffmanCoding::GenerateCodeMapRecursive(MinHeapNode *cur, string str)
{
    if (cur == NULL) return;
    
    if (!cur->internal)
    {
        codeMap[cur->data] = str;
    }

    GenerateCodeMapRecursive(cur->left, str + "0");
    GenerateCodeMapRecursive(cur->right, str + "1");
}
/*
void HuffmanCoding::FreeMinHeap()
{
deallocate (node):
    //do nothing if passed a non-existent node
    if node is null
        return

    //now onto the recursion
    deallocate(left node)
    deallocate(right node)

    free node
}
*/
void HuffmanCoding::PrintCode()
{
    DEBUG("In PrintCode");
    for(auto it = codeMap.begin(); it != codeMap.end(); it++)
    {
        cout<<it->first<<" : "<<it->second<<endl;
    }
}

bool HuffmanCoding::CompressByCode(string fileName)
{
    DEBUG("In CompressByCode");
    if(!SetData(fileName, false))
    {
        return false;
    }

    // input file and ouput compressed file
    ifstream readOriginalFile;
    ofstream writeCompressedFile;
    ofstream writeCodeMap;

    // because >> will skip space, need to cancel it
    readOriginalFile.unsetf(ios::skipws);

    readOriginalFile.open(fileName, ios::binary);
    writeCompressedFile.open(fileName + "-compressed", ios::binary);
    writeCodeMap.open(fileName + "-code", ios::binary);

    if(!readOriginalFile || !writeCompressedFile || !writeCodeMap)
    {
        return false; 
    }

    // read original file and compress it
    unsigned char ch;
    string codeStr = "";

    bitset<8> bs;
    unsigned long binaryValue = 0;

    // read original file in char
    while(readOriginalFile>>ch)
    {        
        codeStr += codeMap[ch];

        while(codeStr.length() >= 8)
        {
            // convert string to unsigned long
            bs = bitset<8>(codeStr);
            binaryValue = bs.to_ulong();

            // convert first 8 bits to char, write to file and remove first 8 bits
            writeCompressedFile<<(char)binaryValue;

            codeStr.erase(0, 8);
        }
    }

    // remain bits but have 8 bits, add zero in end
    string zeroStr = "";
    int addZero = 8 - (int)codeStr.length();
    for(int i = 0; i < addZero; i++)
    {
        zeroStr += "0";
    }

    bs = bitset<8>(codeStr + zeroStr);
    binaryValue = bs.to_ulong();
    writeCompressedFile<<(char)binaryValue;

    // write how many zero we add
    writeCodeMap<<addZero<<endl;

    // create fix code map file
    for(auto it = codeMap.begin(); it != codeMap.end(); it++)
    {
        writeCodeMap<<it->first<<"="<<it->second<<endl;
    }

    readOriginalFile.close();
    writeCompressedFile.close();
    writeCodeMap.close();

    return true;
}

bool HuffmanCoding::DecompressByCode(string fileName)
{
    DEBUG("In DecompressByCode");
    ifstream readCodeMap;
    ifstream readCompressedFile;
    ofstream writeOriginalFile;

    // because >> will skip space, need to cancel it
    readCodeMap.unsetf(ios::skipws);
    readCompressedFile.unsetf(ios::skipws);

    readCodeMap.open(fileName + "-code", ios::binary);
    readCompressedFile.open(fileName + "-compressed", ios::binary);
    writeOriginalFile.open(fileName + "-origin", ios::binary);

    if(!readCodeMap || !readCompressedFile || !writeOriginalFile)
    {
        return false; 
    }

    // read fix code map file to create fixCodeMap for decompress
    map<string, unsigned char> codeMapForDecompress;
    string codeStr;

    char garbageChar;
    unsigned char data;
    string code;
    int addZero = 0;
    int codeMaxLength = 0;

    // get fix code length in first line of file
    getline(readCodeMap, codeStr);
    addZero = stoi(codeStr);

    // read all fix code map and create fixCodeMap
    while(readCodeMap>>data>>garbageChar>>code>>garbageChar)
    {
        codeMapForDecompress[code] = data;

        if(codeMaxLength < (int)code.length())
        {
            codeMaxLength = code.length();
        }
    }

    // read compressed file
    unsigned char ch;
    string codeTmpStr = "";
    int codeTmpStrBound = 2 >> 20;
    bool findCode = false;

    bitset<8> bs;

    // read original file in char
    while(readCompressedFile>>ch)
    {
        bs = bitset<8>(ch);
        codeTmpStr += bs.to_string();

        if((int)codeTmpStr.length() >= codeTmpStrBound)
        {
            findCode = true;
        }

        if(readCompressedFile.peek() == EOF)
        {
            codeTmpStr.erase(codeTmpStr.length() - addZero, addZero);
            findCode = true;
        }

        while(findCode && !codeTmpStr.empty())
        {
            string tmpStr = "";

            findCode = false;

            for(int i = 0; i < (int)codeTmpStr.length(); i++)
            {
                // search code in map and decompress
                tmpStr += codeTmpStr[i];
                if(codeMapForDecompress.find(tmpStr) != codeMapForDecompress.end())
                {
                    data = codeMapForDecompress[tmpStr];
                    codeTmpStr.erase(0, i + 1);

                    // write original char to file
                    writeOriginalFile<<data;

                    findCode = true;
                    break;
                }
            }
        }
    }

    readCodeMap.close();
    readCompressedFile.close();
    writeOriginalFile.close();

    return true;
}

/***********************
** fixed huffman code **
************************/

string HuffmanCoding::NumberToBinary(int num)
{
    DEBUG("In NumberToBinary");
    string str = "";

    while(num != 0)
    {
        if(num%2 == 1)
        {
            str = "1" + str;
        }
        else
        {
            str = "0" + str;
        }

        num/=2;
    }

    return str;
}


void HuffmanCoding::GenerateFixCodeMap()
{
    DEBUG("In GenerateFixCodeMap");
    int fixCodeLength = 0;
    int dataSize = minHeapForFix.size();
    string str = "";

    while(dataSize != 0)
    {
        dataSize = dataSize >> 1;
        fixCodeLength++;
    }
    
    DEBUG(fixCodeLength);

    dataSize = minHeapForFix.size();

    for(int i = 0; i < dataSize; i++)
    {
        // get fixed binary code
        str = NumberToBinary(i);

        // if code length not equal then insert zero
        if((int)str.length() < fixCodeLength)
        {
            string zeroStr = "";
            for(int j = 0; j < fixCodeLength - (int)str.length(); j++)
            {
                zeroStr += "0";
            }

            str = zeroStr + str;
        }
        
        // store fixed code in fixCodeMap
        fixCodeMap[minHeapForFix.top()->data] = str;
        minHeapForFix.pop();
    }
}

void HuffmanCoding::PrintFixCode()
{
    DEBUG("In PrintFixCode");
    for(auto it = fixCodeMap.begin(); it != fixCodeMap.end(); it++)
    {
        cout<<it->first<<" : "<<it->second<<endl;
    }
}

bool HuffmanCoding::CompressByFixCode(string fileName)
{
    DEBUG("In CompressByFixCode");
    if(!SetData(fileName, true))
    {
        return false;
    }

    // input file and ouput compressed file
    ifstream readOriginalFile;
    ofstream writeCompressedFile;
    ofstream writeFixCodeMap;

    // because >> will skip space, need to cancel it
    readOriginalFile.unsetf(ios::skipws);

    readOriginalFile.open(fileName, ios::binary);
    writeCompressedFile.open(fileName + "-fix_compressed", ios::binary);
    writeFixCodeMap.open(fileName + "-fix_code", ios::binary);

    if(!readOriginalFile || !writeCompressedFile || !writeFixCodeMap)
    {
        return false; 
    }

    // read original file and compress it
    unsigned char ch;
    string codeStr = "";

    bitset<8> bs;
    unsigned long binaryValue = 0;

    // read original file in char
    while(readOriginalFile>>ch)
    {        
        codeStr += fixCodeMap[ch];

        while(codeStr.length() >= 8)
        {
            // convert string to unsigned long
            bs = bitset<8>(codeStr);
            binaryValue = bs.to_ulong();

            // convert first 8 bits to char, write to file and remove first 8 bits
            writeCompressedFile<<(char)binaryValue;

            codeStr.erase(0, 8);
        }
    }

    // remain bits but have 8 bits, add zero in end
    string zeroStr = "";
    for(int i = 0; i < 8 - (int)codeStr.length(); i++)
    {
        zeroStr += "0";
    }

    bs = bitset<8>(codeStr + zeroStr);
    binaryValue = bs.to_ulong();
    writeCompressedFile<<(char)binaryValue;

    // write fix code length to map file
    writeFixCodeMap<<(fixCodeMap.begin()->second).length()<<endl;

    // create fix code map file
    for(auto it = fixCodeMap.begin(); it != fixCodeMap.end(); it++)
    {
        writeFixCodeMap<<it->first<<"="<<it->second<<endl;
    }

    readOriginalFile.close();
    writeCompressedFile.close();
    writeFixCodeMap.close();

    return true;
}

bool HuffmanCoding::DecompressByFixCode(string fileName)
{
    DEBUG("In DecompressByFixCode");
    ifstream readFixCodeMap;
    ifstream readCompressedFile;
    ofstream writeOriginalFile;

    // because >> will skip space, need to cancel it
    readFixCodeMap.unsetf(ios::skipws);
    readCompressedFile.unsetf(ios::skipws);

    readFixCodeMap.open(fileName + "-fix_code", ios::binary);
    readCompressedFile.open(fileName + "-fix_compressed", ios::binary);
    writeOriginalFile.open(fileName + "-origin", ios::binary);

    if(!readFixCodeMap || !readCompressedFile || !writeOriginalFile)
    {
        return false; 
    }

    // read fix code map file to create fixCodeMap for decompress
    map<string, unsigned char> fixCodeMapForDecompress;
    string fixCodeStr;

    char garbageChar;
    unsigned char data;
    string fixCode;
    int fixCodeLength = 0;

    // get fix code length in first line of file
    getline(readFixCodeMap, fixCodeStr);
    fixCodeLength = stoi(fixCodeStr);

    // read all fix code map and create fixCodeMap
    while(readFixCodeMap>>data>>garbageChar>>fixCode>>garbageChar)
    {
        fixCodeMapForDecompress[fixCode] = data;
    }

    // read compressed file
    unsigned char ch;
    string codeTmpStr = "";

    bitset<8> bs;

    // read original file in char
    while(readCompressedFile>>ch)
    {
        bs = bitset<8>(ch);
        codeTmpStr += bs.to_string();

        while(codeTmpStr.length() >= (unsigned int)fixCodeLength)
        {
            // search fix code in map and decompress
            fixCode = codeTmpStr.substr(0, fixCodeLength);
            data = fixCodeMapForDecompress[fixCode];
            codeTmpStr.erase(0, fixCodeLength);

            // write original char to file
            writeOriginalFile<<data;
        }
    }

    readFixCodeMap.close();
    readCompressedFile.close();
    writeOriginalFile.close();

    return true;
}