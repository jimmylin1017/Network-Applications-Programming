#include "huffman_coding.h"

int main()
{
    HuffmanCoding huffmanCoding;

    string fileName = "testfile";
    string fileName2 = "Oversoul.mp3";

    /*cout<<"PrintCode"<<endl;
    huffmanCoding.PrintCode();
    cout<<"\nPrintFixCode"<<endl;
    huffmanCoding.PrintFixCode();*/

    //huffmanCoding.CompressByFixCode(fileName);
    //huffmanCoding.DecompressByFixCode(fileName);

    huffmanCoding.CompressByCode(fileName);
    huffmanCoding.DecompressByCode(fileName);

    return 0;
}