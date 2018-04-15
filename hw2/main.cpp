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

    huffmanCoding.CompressByFixCode(fileName2);
    huffmanCoding.DecompressByFixCode(fileName2);

    //huffmanCoding.CompressByCode(fileName2);
    //huffmanCoding.DecompressByCode(fileName2);

    return 0;
}