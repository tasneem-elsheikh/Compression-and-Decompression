#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a node in the linked list priority queue
struct Node
{
    int frequency;
    char character;
    struct Node* next;
    struct TreeNode* treeNode;
};

// Structure to represent a node in the binary tree
struct TreeNode
{
    int frequency;
    char character;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Structure to represent linked list to store codes
struct CodeLinkedList
{
    char character;
    char* code;
    struct CodeLinkedList* next;
};

//Function to replace every / with // in a string
char* DoubleBackslashes(char* Path)
{
    double LengthPath = strlen(Path);
    double NewLengthPath = LengthPath + strlen(Path) - strlen(strchr(Path, '\\')) + 1;

    char* NewPath = (char*)malloc(NewLengthPath);

    char* PathP = Path;
    char* NewPathP = NewPath;
    while (*PathP != '\0')
    {
        if (*PathP == '\\')
        {
            *NewPathP++ = '\\';
            *NewPathP++ = '\\';
        }
        else if (*PathP == '"')
        {
        }
        else
            *NewPathP++ = *PathP;
        PathP++;
    }
    *NewPathP = '\0';
    return NewPath;
}

// Function to read a user-specified text file and generate the frequency table
void ReadFile(char* filename, Node** head)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening Input file!");
        exit(0);
    }
    int CharCount[128] = { 0 };
    char c;
    while ((c = fgetc(file)) != EOF)
        CharCount[(int)c]++;

    fclose(file);

    // Create a linked list node for each unique character
    for (int i = 0; i < 127; i++)
    {
        if (CharCount[i] > 0)
        {
            struct Node* node = (Node*)malloc(sizeof(Node));
            if (!node)
            {
                printf("Memory allocation error!");
                exit(0);
            }
            node->frequency = CharCount[i];
            node->character = (char)i;
            node->next = *head;
            node->treeNode = NULL;
            *head = node;
        }
    }
}

// Function to implement a linked-list priority queue to dynamically sort the frequency table
Node* PriorityQueueSort(Node** head)
{
    Node* current = *head;
    Node* next = NULL;

    while (current)
    {
        next = current->next;
        while (next)
        {
            if (current->frequency > next->frequency) // Swap nodes
            {
                int temp_freq = current->frequency;
                char temp_char = current->character;
                TreeNode* temp_treeNode = current->treeNode;
                current->frequency = next->frequency;
                current->character = next->character;
                current->treeNode = next->treeNode;
                next->frequency = temp_freq;
                next->character = temp_char;
                next->treeNode = temp_treeNode;
            }
            next = next->next;
        }
        current = current->next;
    }
    return *head;
}

// Function to add a tree node into a sorted queue
Node* AddToPQ(Node* head, TreeNode* treeNode)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode)
    {
        printf("Memory allocation error!");
        exit(0);
    }
    newNode->frequency = treeNode->frequency;
    newNode->character = treeNode->character;
    newNode->treeNode = treeNode;
    newNode->next = NULL;

    if (head == NULL || newNode->frequency < head->frequency)
    {
        newNode->next = head;
        return newNode;
    }

    Node* current = head;
    while (current->next != NULL && current->next->frequency < newNode->frequency)
        current = current->next;
    newNode->next = current->next;
    current->next = newNode;
    return head;
}

// Function to delete the least 2 nodes of the queue used in the Huffman tree and link to a new node
Node* DeleteLeastTwo(Node* head)
{
    if (head == NULL || head->next == NULL)
        return NULL;
    Node* first = head;
    Node* second = head->next;
    head = second->next;
    free(first);
    free(second);
    return head;
}

// Function to build the Huffman tree from the priority queue
TreeNode* BuildHuffman(Node* head)
{
    if (head == NULL)
    {
        printf("Input File is empty!");
        exit(0);
    }
    if (head->next == NULL)
    {
        if (head != NULL)
            return head->treeNode;
        else
        {
            printf("No enough data in File!");
            exit(0);
        }
    }
    TreeNode* TreeLeft = (TreeNode*)malloc(sizeof(TreeNode));
    if (!TreeLeft)
    {
        printf("Memory allocation error!");
        exit(0);
    }
    TreeLeft->frequency = head->frequency;
    TreeLeft->character = head->character;
    if (head->treeNode)
    {
        TreeLeft->left = head->treeNode->left;
        TreeLeft->right = head->treeNode->right;
    }
    else
    {
        TreeLeft->left = NULL;
        TreeLeft->right = NULL;
    }
    TreeNode* TreeRight = (TreeNode*)malloc(sizeof(TreeNode));
    if (!TreeRight)
    {
        printf("Memory allocation error!");
        exit(0);
    }
    TreeRight->frequency = head->next->frequency;
    TreeRight->character = head->next->character;
    if (head->next->treeNode)

    {
        TreeRight->left = head->next->treeNode->left;
        TreeRight->right = head->next->treeNode->right;
    }
    else
    {
        TreeRight->left = NULL;
        TreeRight->right = NULL;
    }
    // Create a new internal tree node
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (!newNode)
    {
        printf("Memory allocation error\n");
        return NULL;
    }
    newNode->frequency = TreeLeft->frequency + TreeRight->frequency;
    newNode->character = '\0';
    newNode->left = TreeLeft;
    newNode->right = TreeRight;

    head = DeleteLeastTwo(head);
    head = AddToPQ(head, newNode);

    return BuildHuffman(head);
}

// Function that generates the code for each character
void GenerateCodes(TreeNode* root, char code[], int length, FILE* codFile)
{
    if (root == NULL)
        return;
    if (!root->left && !root->right)
    {
        code[length] = '\0';

        fprintf(codFile, "%c %s.\n", root->character, code);

        printf("Character: %c | Code: %s\n", root->character, code);

        return;
    }
    code[length] = '0';
    GenerateCodes(root->left, code, length + 1, codFile);
    code[length] = '1';
    GenerateCodes(root->right, code, length + 1, codFile);
}

//Function to Extract file name from main file to output file
char* ExtractFileName(char* input_filename, char* output_filename)
{
    int i = 0;
    int j = 0;
    while (input_filename[i] != '\0')
    {
        i++;
    }
    while (input_filename[i - 1] != '\\')
    {
        i--;
    }
    while (input_filename[i] != '.')
    {
        output_filename[j] = input_filename[i];
        j++;
        i++;
    }
    output_filename[j] = '\0';

    return output_filename;
}

//Function to Count linked list
int ListNumber(Node* node)
{
    Node* tmp1 = node;
    int k = 0;
    while (tmp1 != NULL)
    {
        tmp1 = tmp1->next;
        k++;
    }
    free(tmp1);
    return k;
}

//Function to print the Characters and their Frequency
void CharacterFrequencyPrint(Node* node)
{
    Node* tmp = node;
    while (tmp != NULL)
    {
        printf("Character: %c | Frequency: %d\n", tmp->character, tmp->frequency);
        tmp = tmp->next;
    }
    printf("\n");
    free(tmp);
}

//Function to cod file and put it in a LinkedList
CodeLinkedList* CharacterMapping(CodeLinkedList* head, FILE* CompressingScan)
{
    while (!feof(CompressingScan))
    {
        char character;
        char code[128];
        fscanf(CompressingScan, "%c %[^.] ", &character, code);
        fgetc(CompressingScan);
        fgetc(CompressingScan);
        CodeLinkedList* occurrence = (CodeLinkedList*)malloc(sizeof(CodeLinkedList));
        if (!occurrence)
        {
            printf("Memory allocation error!");
            exit(0);
        }
        occurrence->code = (char*)malloc(strlen(code));
        if (!occurrence->code)
        {
            printf("Memory allocation error!");
            exit(0);
        }
        occurrence->character = character;
        strcpy(occurrence->code, code);
        occurrence->next = NULL;

        if (head == NULL)
            head = occurrence;
        else
        {
            CodeLinkedList* temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = occurrence;
        }
    }
    return head;
}

// Function to Comress Text File to Binary File
void CompressTextToBinary(CodeLinkedList* head, char* InputFileName, char* OutputFileName)
{
    FILE* origin = fopen(InputFileName, "r");
    if (!origin)
    {
        printf("Error opening Input File!");
        exit(0);
    }
    FILE* compressed = fopen(OutputFileName, "w");
    if (!compressed)
    {
        printf("Error creating .com file!");
        exit(0);
    }
    char character;

    while ((character = fgetc(origin)) != EOF)
    {
        CodeLinkedList* temp = head;
        while (temp != NULL && temp->character != character)
            temp = temp->next;

        if (temp == NULL)
            printf("[ %c CHARACTER NOT FOUND] ", character);
        else
            fprintf(compressed, "%s", temp->code);
    }
    fclose(origin);
    fclose(compressed);
    printf("File Converted to Binary\n\n");
}

//Function to Compress Binary File to ASCII File
void CompressBinaryToASCII(char* BinaryFileName, char* FileName)
{
    FILE* BinaryFile = fopen(BinaryFileName, "rb");
    if (!BinaryFile)
    {
        printf("Error opening binary file\n");
        exit(1);
    }

    FILE* ASCIIFile = fopen(FileName, "wb"); // Open a separate file for output
    if (!ASCIIFile)
    {
        printf("Error creating ASCII file!\n");
        exit(1);
    }

    char byte[9]; // Buffer to hold 8 bits + null terminator
    int bytesRead;
    while ((bytesRead = fread(byte, 1, 8, BinaryFile)) > 0)
    {
        if (bytesRead < 8) // If we didn't read a full 8 bits, pad with zeros
            for (int i = bytesRead; i < 8; i++)
                byte[i] = '0';
        byte[8] = '\0'; // Null terminate the string

        char asciiChar = (char)strtol(byte, NULL, 2);
        fputc(asciiChar, ASCIIFile); // Write the ASCII character to the file
    }

    fclose(BinaryFile);
    fclose(ASCIIFile);
    remove(BinaryFileName);

    printf("File Compressed to ASCII Equivalent and added to the '.com' file.\n\n");
}

//Function to Decompress ASCII File to Binary File
void DecompressASCIIToBinary(char* InputFileName, char* OutputFileName)
{
    FILE* InputFile = fopen(InputFileName, "rb");
    if (!InputFile)
    {
        printf("Error opening File");
        exit(0);
    }
    FILE* OutputFile = fopen(OutputFileName, "wb");
    if (!OutputFile)
    {
        printf("Error opening file.\n");
        exit(1);
    }

    char c;
    while (fread(&c, 1, 1, InputFile) == 1)
        for (int i = 7; i >= 0; i--)
            fprintf(OutputFile, "%d", (c >> i) & 1);

    fclose(InputFile);
    fclose(OutputFile);
    printf("File Converted to Binary Equivalent.\n\n");
}

//Function to Decompress Binary File to Text File
void DeCompressBinaryToText(CodeLinkedList* head, char* DeCompresseddBianryFileName, char* DeCompressedFileName)
{
    FILE* BinaryFile = fopen(DeCompresseddBianryFileName, "r");
    if (BinaryFile == NULL)
    {
        printf("Error opening  file!");
        exit(0);
    }

    FILE* DeCompressedFile = fopen(DeCompressedFileName, "w");
    if (DeCompressedFile == NULL)
    {
        printf("Error opening  file!");
        exit(0);
    }

    CodeLinkedList* temp = head;
    int MaxCodeLength = 0;
    while (temp)
    {
        if (MaxCodeLength < strlen(temp->code))
            MaxCodeLength = strlen(temp->code);
        temp = temp->next;
    }
    char* Binary = (char*)malloc(MaxCodeLength + 1);
    if (Binary == NULL)
    {
        printf("Memory allocation failed!");
        exit(0);
    }
    int i = 0;
    temp = head;
    while (1)
    {
        int c = fgetc(BinaryFile);
        if (c == EOF)
            break;
        Binary[i++] = c;
        Binary[i] = '\0';
        temp = head;
        while (temp != NULL)
        {
            if (!strcmp(Binary, temp->code))
            {
                fprintf(DeCompressedFile, "%c", temp->character);
                i = 0;
                break;
            }
            temp = temp->next;
        }
    }
    free(Binary);
    fclose(BinaryFile);
    fclose(DeCompressedFile);
    remove(DeCompresseddBianryFileName);

    printf("File Decompressed Successfully!\n\n");
}


//Main
int main()
{
    printf("Hello, Welcome to our program! :)\n");
    char MainFileNameTemp[256];
    printf("Enter a file path: ");
    gets_s(MainFileNameTemp);
    char* MainFileName = DoubleBackslashes(MainFileNameTemp);

    char* codFileName = (char*)malloc(strlen(MainFileName));
    if (!codFileName)
    {
        printf("Memory allocation error!");
        exit(0);
    }
    ExtractFileName(MainFileName, codFileName);
    strcat(codFileName, ".cod");

    char* CompressedFileName = (char*)malloc(strlen(MainFileName));
    if (!CompressedFileName)
    {
        printf("Memory allocation error!");
        exit(0);
    }
    ExtractFileName(MainFileName, CompressedFileName);
    strcat(CompressedFileName, "1.com");

    char* CompressedFileName2 = (char*)malloc(strlen(MainFileName));
    if (!CompressedFileName2)
    {
        printf("Memory allocation error!");
        exit(0);
    }
    ExtractFileName(MainFileName, CompressedFileName2);
    strcat(CompressedFileName2, ".com");

    char* DeCompressedFileName = (char*)malloc(strlen(MainFileName));
    if (!DeCompressedFileName)
    {
        printf("Memory allocation error!");
        exit(0);
    }
    ExtractFileName(MainFileName, DeCompressedFileName);
    strcat(DeCompressedFileName, "1.txt");

    char* DeCompressedFileName2 = (char*)malloc(strlen(MainFileName));
    if (!DeCompressedFileName2)
    {
        printf("Memory allocation error!");
        exit(0);
    }
    ExtractFileName(MainFileName, DeCompressedFileName2);
    strcat(DeCompressedFileName2, ".txt");

    FILE* codFile = fopen(codFileName, "w");
    if (!codFile)
    {
        printf("Error creating .cod file!");
        exit(0);
    }

    Node* PriorityQueue = NULL;
    ReadFile(MainFileName, &PriorityQueue);
    PriorityQueueSort(&PriorityQueue);

    CharacterFrequencyPrint(PriorityQueue);

    char* CompCode = (char*)malloc(ListNumber(PriorityQueue));

    TreeNode* HuffmanTree = BuildHuffman(PriorityQueue);
    GenerateCodes(HuffmanTree, CompCode, 0, codFile);
    printf("\n");
    fclose(codFile);

    FILE* codFile2 = fopen(codFileName, "r");
    if (!codFile2)
    {
        printf("Error creating .cod file!");
        exit(0);
    }

    CodeLinkedList* LinkedCode = NULL;

    LinkedCode = CharacterMapping(LinkedCode, codFile2);
    fclose(codFile2);

    CompressTextToBinary(LinkedCode, MainFileName, CompressedFileName);

    CompressBinaryToASCII(CompressedFileName, CompressedFileName2);

    DecompressASCIIToBinary(CompressedFileName2, DeCompressedFileName);

    DeCompressBinaryToText(LinkedCode, DeCompressedFileName, DeCompressedFileName2);

    return 0;
}