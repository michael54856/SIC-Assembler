#include <iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<unordered_map>
using namespace std;

struct line //store the info of each line
{
    string wholeString;

    string label;
    string mnemonic;
    string operand;

    int location;
    string myOPCODE;

};

struct symbolNode //store the info of the symbol and its address
{
    string symbol;
    int address;
};

int hexToDec(string s)//string to int
{
    int total = 0;
    int mul = 1;
    int len = strlen(s.c_str());
    for(int i = len-1; i >= 0; i--)//reverse order to get each digit(from least digit)
    {
        int t;
        if(s[i] >= '0' && s[i] <= '9')//number
        {
            t = s[i] - '0';
        }
        else//alphabet
        {
            t = (s[i]-'A'+10);
        }
        total += t*mul;
        mul*=16;
    }
    return total;
}

int dec_atoi(string s)//string to int
{
    int total = 0;
    int mul = 1;
    int len = strlen(s.c_str());
    for(int i = len-1; i >= 0; i--)//reverse order to get each digit(from least digit)
    {
        total += (s[i] - '0')*mul;
        mul*=10;
    }
    return total;
}

string decToHex(int num)//turn the decimal to hexadecimal(int to string)
{
    string s = "";
    while(num > 0)
    {
        int t = num % 16;
        if(t <= 9)//0~9 turn to number
        {
            s += (char)('0'+t);
        }
        else//10~15turn to alphabet
        {
            s += (char)('A'+(t-10));
        }
        num /= 16;
    }
    string rev_s = "";
    int len = strlen(s.c_str());
    for(int i = len-1; i >= 0; i--)//the reverse order is the correct answer
    {
        rev_s += s[i];
    }
    return rev_s;
}

string decToHex_four(int num)//turn the decimal to hexadecimal(int to string)(four digit,purpose of creating 4 digit location)
{
    string s = "";
    while(num > 0)
    {
        int t = num % 16;
        if(t <= 9)//0~9 turn to number
        {
            s += (char)('0'+t);
        }
        else//10~15turn to alphabet
        {
            s += (char)('A'+(t-10));
        }
        num /= 16;
    }
    string rev_s = "";
    int len = strlen(s.c_str());
    if(len  < 4)
    {
        for(int i = 0; i < (4-len); i++)//the reverse order is the correct answer
        {
            rev_s += "0";
        }
    }
    for(int i = len-1; i >= 0; i--)//the reverse order is the correct answer
    {
        rev_s += s[i];
    }
    return rev_s;
}

int main()
{
    unordered_map<string,string> opTable;
    unordered_map<string,int> symTable;

    int symbolCount = 0;
    symbolNode symbolTable[1000];//set the maximum symbol to 1000
    int lineCount = 0;
    line lineTable[1000];//set the maximum line to 1000


    fstream inputFile;
    inputFile.open("source.txt",ios::in); //read the soucre.txt

    fstream opcodeFile;
    opcodeFile.open("opcode.txt",ios::in); //read the opcode.txt

    string s;
    string codeNum;
    while(opcodeFile >> s)//put the opcode to opTable
    {
        opcodeFile >> codeNum;
        opTable[s] = codeNum;
    }
    opcodeFile.close();


    //==================================pass_1==================================

    //since the input is always correct, the first line is always START,we first construct the first line and get the start of program counter
    getline(inputFile,s); // get first line

    line firstLine;
    firstLine.wholeString = s;
    unsigned int index = 0;//point the char from the first line
    firstLine.myOPCODE = "";
    firstLine.label = "";
    while(true)//construct the label name till meet tab
    {
        if(s[index] == ' ')
        {
            index++;
            continue;
        }
        if(s[index] == '\t')
        {
            index++;
            break;
        }
        firstLine.label += s[index];
        index++;
    }

    firstLine.mnemonic = "";
    while(true)//construct the mnemonic name till meet tab
    {
        if(s[index] == ' ')
        {
            index++;
            continue;
        }
        if(s[index] == '\t')
        {
            index++;
            break;
        }
        firstLine.mnemonic += s[index];
        index++;
    }

    firstLine.operand = "";
    while(index < strlen(s.c_str()))//construct the operand till the end
    {
        if(s[index] == ' ')
        {
            index++;
            continue;
        }
        firstLine.operand += s[index];
        index++;
    }

    firstLine.location = hexToDec(firstLine.operand); //the start location is the operand itself

    int pc = firstLine.location; //set the program counter


    while(getline(inputFile,s))//get other line
    {
        line temp;
        temp.wholeString = s;
        temp.location = pc;//set the location to pc
        unsigned int i = 0;
        if(s[0] != '\t')//have label(the first character is not tab)
        {
            temp.label = "";
            while(true)
            {
                if(s[i] == ' ')
                {
                    i++;
                    continue;
                }
                if(s[i] == '\t')//construct the label name till meet tab
                {
                    i++;
                    break;
                }
                temp.label += s[i];
                i++;
            }

            symbolNode newSymbol; //create new symbol object
            newSymbol.symbol = temp.label;
            newSymbol.address = pc;

            symbolTable[symbolCount] = newSymbol; //insert the new symbol to symbol table(this one is for the output)
            symTable[newSymbol.symbol] = newSymbol.address; //insert the new symbol to symbol table(the one i really use,using hashmap)
            symbolCount++;
        }
        else
        {
            temp.label = "";
            i++;
        }

        temp.mnemonic = "";
        while(i < strlen(s.c_str()))//construct the mnemonic name till meet tab
        {
            if(s[i] == ' ')
            {
                i++;
                continue;
            }
            if(s[i] == '\t')
            {
                i++;
                break;
            }
            temp.mnemonic += s[i];
            i++;
        }

        temp.operand = "";
        while(i < strlen(s.c_str()))//construct the operand till the end
        {
            if(s[i] == ' ')
            {
                i++;
                continue;
            }
            temp.operand += s[i];
            i++;
        }

        //add the program counter
        if(temp.mnemonic == "WORD")
        {
            pc += 3;
        }
        else if(temp.mnemonic == "RESW")
        {
            pc += 3*(dec_atoi(temp.operand));
        }
        else if(temp.mnemonic == "RESB")
        {
            pc += dec_atoi(temp.operand);
        }
        else if(temp.mnemonic == "BYTE")
        {
            if((temp.operand)[0] == 'C')
            {
                pc += strlen((temp.operand).c_str()) - 3;//calculate how many character in the operand
            }
            else
            {
                pc += 1;
            }
        }
        else//normal instruction
        {
            pc += 3;
        }

        lineTable[lineCount] = temp;
        lineCount++;

    }

    //write the locationAndSource.txt
    fstream locationAndSource;
    locationAndSource.open("pass1_locationAndSource.txt",ios::out);
    locationAndSource << "Loc\t" << "Source statement" << endl;
    locationAndSource << decToHex_four(firstLine.location) << "\t" << firstLine.wholeString << endl;
    for(int i = 0; i < lineCount-1; i++)
    {
        locationAndSource << decToHex_four(lineTable[i].location) << "\t" << lineTable[i].wholeString << endl;
    }
    locationAndSource << "\t" << lineTable[lineCount-1].wholeString << endl;
    locationAndSource.close();
    cout << "Pass 1 : " << endl << endl;
    cout << "write the location of each source code to pass1_locationAndSource.txt!" << endl << endl;

    //write the symbolTable.txt
    fstream symbolTableFile;
    symbolTableFile.open("pass1_symbolTable.txt",ios::out);
    symbolTableFile << "Name\t" << "Address" << endl;
    for(int i = 0; i < symbolCount; i++)
    {
        symbolTableFile << symbolTable[i].symbol << "\t" << decToHex_four(symbolTable[i].address) << endl;
    }
    symbolTableFile.close();
    cout << "write the symbol table to pass1_symbolTable.txt!" << endl << endl;

    //==================================pass_1==================================

    //==================================pass_2==================================
    for(int i = 0; i < lineCount-1; i++)
    {
        if(lineTable[i].mnemonic == "RESW" || lineTable[i].mnemonic == "RESB")
        {
            lineTable[i].myOPCODE = "";
            continue;
        }
        else
        {
            if(lineTable[i].mnemonic == "WORD")
            {
                string opcode = "";
                string s1 = decToHex(dec_atoi(lineTable[i].operand));
                for(unsigned int j = 0; j < (6-strlen(s1.c_str())); j++)//fill 0 to length 6
                {
                    opcode += "0";
                }
                opcode += s1;
                lineTable[i].myOPCODE = opcode;
            }
            else if(lineTable[i].mnemonic == "BYTE")
            {
                if((lineTable[i].operand)[0] == 'C')//C''
                {
                    string opcode = "";
                    unsigned int charNum = strlen((lineTable[i].operand).c_str()) - 3;//check how many character
                    for(unsigned int j = 2; j < (strlen((lineTable[i].operand).c_str())-1); j++)//concatenate each character
                    {
                        string t = decToHex((int)((lineTable[i].operand)[j]));
                        if(strlen(t.c_str()) == 1)
                        {
                            opcode += "0";
                        }
                        opcode += t;
                    }
                    lineTable[i].myOPCODE = opcode;
                }
                else//X''
                {
                    int len = strlen((lineTable[i].operand).c_str());
                    string opcode = "";
                    for(int j = 2; j < len-1; j++)
                    {
                        opcode += (lineTable[i].operand)[j];
                    }
                    lineTable[i].myOPCODE = opcode;
                }
            }
            else if(lineTable[i].operand == "")//no operand
            {
                string opcode = opTable[lineTable[i].mnemonic];//use optable to get opcode
                opcode += "0000";
                lineTable[i].myOPCODE = opcode;
            }
            else//normal instruction
            {
                string opcode = opTable[lineTable[i].mnemonic];//use optable to get opcode

                int x = -1;
                for(int j = 0; j < strlen((lineTable[i].operand).c_str()); j++)
                {
                    if((lineTable[i].operand)[j] == ',')//indexed addressing
                    {
                        x = j;
                        break;
                    }
                }
                if(x == -1)//normal addressing
                {
                    opcode += decToHex_four(symTable[lineTable[i].operand]);
                }
                else//indexed addressing
                {
                    string realOperand = "";//to get the real operand without ',X'
                    for(int j = 0; j < x; j++)
                    {
                        realOperand += (lineTable[i].operand)[j];
                    }
                    int targetAddress = symTable[realOperand];
                    targetAddress += 32768;//to add the X
                    opcode += decToHex_four(targetAddress);
                }

                lineTable[i].myOPCODE = opcode;
            }

        }
    }

    cout << "Pass_2 : " << endl << endl;
     //write the source with location and objectcode to pass2_source_LocObj.txt
    fstream source_LocObj;
    source_LocObj.open("pass2_source_LocObj.txt",ios::out);
    source_LocObj << decToHex_four(firstLine.location) << "\t" << firstLine.wholeString << endl;
    for(int i = 0; i < lineCount-1; i++)
    {
        if(strlen(lineTable[i].operand.c_str()) >= 8)
        {
            source_LocObj << decToHex_four(lineTable[i].location) << "\t" << lineTable[i].wholeString << "\t" << lineTable[i].myOPCODE << endl;
        }
        else if(lineTable[i].operand == "")
        {
             source_LocObj << decToHex_four(lineTable[i].location) << "\t" << lineTable[i].wholeString << "\t\t\t" << lineTable[i].myOPCODE << endl;
        }
        else
        {
            source_LocObj << decToHex_four(lineTable[i].location) << "\t" << lineTable[i].wholeString << "\t\t" << lineTable[i].myOPCODE << endl;
        }
    }
    source_LocObj << "\t" << lineTable[lineCount-1].wholeString << endl;
    source_LocObj.close();
    cout << "write the source with location and objectcode to pass2_source_LocObj.txt!" << endl << endl;

    //write the text record to pass2_textRecord.txt
     fstream pass2_textRecord;
     pass2_textRecord.open("pass2_textRecord.txt",ios::out);

     //out format of first line
     pass2_textRecord << 'H';
     pass2_textRecord << firstLine.label;
     for(int i = 0; i < 6-strlen(firstLine.label.c_str()); i++)//fill space
     {
         pass2_textRecord << ' ';
     }
     pass2_textRecord << "00" << decToHex_four(firstLine.location) << "00" << decToHex_four(lineTable[lineCount-1].location - firstLine.location) << endl;

     for(int i = 0; i < lineCount-1; )
     {
         string opcodeBuffer = "";
         int recordLen = 0;
         bool first = true;
         int byteCount = 0;
         int startAddress;
         while(i < lineCount-1)
         {
             if(lineTable[i].mnemonic == "RESW" || lineTable[i].mnemonic == "RESB")//meet RESW RESB to change line
             {
                 while(i < lineCount-1 && (lineTable[i].mnemonic == "RESW" || lineTable[i].mnemonic == "RESB"))
                 {
                     i++;
                 }
                 break;
             }
             if(recordLen + strlen(lineTable[i].myOPCODE.c_str()) > 60)
             {
                 break;
             }
             if(first == true)//we need to get the first line location to know the start address
             {
                 startAddress = lineTable[i].location;
                 first = false;
             }

             if(lineTable[i].mnemonic == "BYTE")
             {
                 if((lineTable[i].operand)[0] == 'C')
                 {
                     byteCount += strlen(lineTable[i].operand.c_str()) - 3;
                 }
                 else
                 {
                     byteCount += 1;
                 }
             }
             else
             {
                 byteCount += 3;
             }

             recordLen += strlen(lineTable[i].myOPCODE.c_str());
             opcodeBuffer += lineTable[i].myOPCODE;
             i++;
         }
         pass2_textRecord << "T00" << decToHex_four(startAddress);
         string lineLen = decToHex(byteCount);
         if(strlen(lineLen.c_str()) == 1)
         {
             pass2_textRecord << '0';
         }
         pass2_textRecord << lineLen;
         pass2_textRecord << opcodeBuffer;
         pass2_textRecord << endl;

     }
     pass2_textRecord << "E00" << decToHex_four(firstLine.location);
     pass2_textRecord.close();
    cout << "write the text record to pass2_textRecord.txt!" << endl << endl;

    //==================================pass_2==================================
}
