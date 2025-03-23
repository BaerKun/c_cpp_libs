#include "tree/trie_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STEAM_SIZE (1 << 14)

typedef struct {
    unsigned char input[STEAM_SIZE];
    unsigned char output[STEAM_SIZE + 8];
    unsigned insize;
    unsigned outsize;
    unsigned nbytes;
    unsigned lastByte;
    unsigned restBitWidth;
    unsigned long restBits;
    unsigned helper1[256];
    unsigned helper2[256];
} Transfer;

void calcByteCount(const unsigned char *input, unsigned *byteCount, unsigned size) {
    for (int i = 0; i < size; ++i)
        byteCount[input[i]]++;
}

TrieTreePtr getHuffmanCodeTree(unsigned byte[], unsigned byteCount[], unsigned *nbyte) {
    int j = 0;
    for (int i = 0; i < 256; ++i) {
        if (byteCount[i] != 0) {
            byteCount[j] = byteCount[i];
            byte[j++] = i;
        }
    }

    *nbyte = j;
    return HuffmanCode((int *) byte, (int *) byteCount, j);
}

void getCodeAndDepthHelper(TrieTreeNodePtr node, unsigned *codeArr, unsigned *widthArr, unsigned code, unsigned width) {
    if (node->data != NO_DATA) {
        codeArr[node->data] = code;
        widthArr[node->data] = width;
        return;
    }
    getCodeAndDepthHelper(node->left, codeArr, widthArr, code, width + 1);
    getCodeAndDepthHelper(node->right, codeArr, widthArr, code | (1 << width), width + 1);
}

void getCodeAndDepth(TrieTreePtr tree, unsigned *code, unsigned *width) {
    getCodeAndDepthHelper(tree->root, code, width, 0, 0);
}

void loadOutput(Transfer *transfer) {
    unsigned i = 0, j = 0, tmpwidth,
            totalWidth = transfer->restBitWidth;
    unsigned long tmpcode;
    unsigned long *output = (unsigned long *) transfer->output;
    unsigned char byte;

    memset(output, 0, sizeof(transfer->output));

    output[0] = transfer->restBits;
    do {
        byte = transfer->input[i];
        tmpcode = transfer->helper1[byte];
        tmpwidth = transfer->helper2[byte];
        if (totalWidth + tmpwidth >= 64) {
            output[j++] |= (tmpcode << totalWidth);
            tmpwidth -= 64 - totalWidth;
            tmpcode >>= 64 - totalWidth;
            totalWidth = 0;

            if (tmpwidth == 0)
                continue;
        }
        output[j] |= (tmpcode << totalWidth);
        totalWidth += tmpwidth;
    } while (++i != transfer->insize);

    transfer->outsize = j * 8 + totalWidth / 8;
    transfer->restBits = transfer->output[transfer->outsize];
    transfer->restBitWidth = totalWidth % 8;
}

void finput(FILE *readFile, Transfer *transfer) {
    while (!feof(readFile)) {
        transfer->insize = (unsigned) fread(transfer->input, 1, STEAM_SIZE, readFile);
        calcByteCount(transfer->input, transfer->helper2, transfer->insize);
        transfer->lastByte = transfer->input[transfer->insize - 1];
    }
}

void process(Transfer *transfer) {
    TrieTreePtr tree = getHuffmanCodeTree(transfer->helper1, transfer->helper2, &transfer->nbytes);

    memset(transfer->helper2, 0, 256 * sizeof(int));
    getCodeAndDepth(tree, transfer->helper1, transfer->helper2);

    BT_destroy(tree);
}

void foutput(FILE *writeFile, FILE *readFile, Transfer *transfer) {
    fwrite(&transfer->nbytes, sizeof(int), 1, writeFile);

    fwrite(&transfer->lastByte, sizeof(int), 1, writeFile);

    unsigned *code = transfer->helper1;
    unsigned *depth = transfer->helper2;
    unsigned key;

    for (int i = 0; i < 256; ++i) {
        if (depth[i] != 0) {
            key = code[i] | (1 << depth[i]);
            fwrite(&i, sizeof(char), 1, writeFile);
            fwrite(&key, sizeof(int), 1, writeFile);
        }

    }

    fseek(readFile, 0, SEEK_SET);
    while (!feof(readFile)) {
        transfer->insize = (unsigned) fread(transfer->input, 1, STEAM_SIZE, readFile);
        loadOutput(transfer);
        fwrite(transfer->output, 1, transfer->outsize, writeFile);
    }
    if(transfer->restBitWidth != 0) {
        if((transfer->helper1[transfer->lastByte] & 1) == 0)
            transfer->restBits |= (0xff << transfer->restBitWidth);
        fwrite(&transfer->restBits, 1, 1, writeFile);
    }
}

long getFileSize(FILE *file) {
    fseek(file, 0, SEEK_END);
    return ftell(file);
}

int main(int argc, char *argv[]) {
    char str[512];
    char *readFilePath = str,
        *writeFilePath = str + 256;

    switch (argc){
        case 1:
            puts("please give me a file to read: ");
            scanf("%s", readFilePath);
            puts("then give me a file to write: ");
            scanf("%s", writeFilePath);
            break;
        case 2:
            readFilePath = argv[1];
            puts("please give me a file to write: ");
            scanf("%s", writeFilePath);
            break;
        default:
            readFilePath = argv[1];
            writeFilePath = argv[2];
    }

    FILE *readFile = fopen(readFilePath, "rb");
    if(readFile == NULL) {
        printf("can't open this file: %s", readFilePath);
        return 0;
    }

    FILE *writeFile = fopen(writeFilePath, "wb");
    if(writeFile == NULL) {
        printf("can't open this file: %s", writeFilePath);
        fclose(readFile);
        return 0;
    }

    Transfer *transfer = malloc(sizeof(Transfer));
    if(transfer == NULL) {
        puts("Bao le!!!");
        fclose(readFile);
        fclose(writeFile);
        return 0;
    }

    finput(readFile, transfer);
    process(transfer);
    foutput(writeFile, readFile, transfer);

    free(transfer);

    printf("file is written to %s\n", writeFilePath);
    printf("file size compressed form %ldkb to %ldkb\n", getFileSize(readFile) / 1024, getFileSize(writeFile) / 1024);

    fclose(readFile);
    fclose(writeFile);

    return 0;
}