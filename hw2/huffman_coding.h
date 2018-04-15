#include "header.h"

#define CHAR_MAX 256

class HuffmanCoding
{
    struct MinHeapNode
    {
        // input character
        unsigned char data;

        // frequence of data
        unsigned int freq;

        // check node is internal node or not
        bool internal;

        // left and right child of tree
        MinHeapNode *left, *right;

        // constructor for initial
        MinHeapNode(unsigned char data, unsigned int freq, bool internal)
        : data(data)
        , freq(freq)
        , internal(internal)
        , left(NULL)
        , right (NULL){}

    } *left, *right, *top, *root;

    struct compare
    {
        // freq from small to big
        bool operator()(MinHeapNode *l, MinHeapNode *r)
        {
            return (l->freq > r->freq);
        }
    };

    struct compareForFix
    {
        // freq from small to big
        bool operator()(MinHeapNode *l, MinHeapNode *r)
        {
            return (l->freq < r->freq);
        }
    };

    // for read file data
    int dataFreq[CHAR_MAX];
    void ResetAll();

    // create huffman code
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
    map<unsigned char, string> codeMap;

    // create fix huffman code
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compareForFix> minHeapForFix;
    map<unsigned char, string> fixCodeMap;
    int fixCodeLength;

    bool ReadFileInBinary(string fileName);
    bool SetData(string fileName, bool checkFixed);

    // for huffman code
    void CreateMinHeap();
    void GenerateCodeMap();
    void GenerateCodeMapRecursive(MinHeapNode *cur, string str);
    
    // for fix huffman code
    string NumberToBinary(int num);
    void GenerateFixCodeMap();
    int GetFileSizeInBytes(string fileName);

public:

    HuffmanCoding() : left(NULL), right(NULL), top(NULL), root(NULL), fixCodeLength(0)
    {
        DEBUG("HuffmanCoding Initial");
        codeMap.clear();
        fixCodeMap.clear();
    }

    // for huffman code
    void PrintCode();
    bool CompressByCode(string fileName);
    bool DecompressByCode(string fileName);
    
    // for fix huffman code
    void PrintFixCode();
    bool CompressByFixCode(string fileName);
    bool DecompressByFixCode(string fileName);
    
};