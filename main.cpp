// ============================================================================
// David Rawlings
// Doug
// ============================================================================
// ciss360 final project 2
// ============================================================================
// Description:
// This program simulates MIPS with the registers $s0, $s1, $s2 and understands
// li, add, sub. At the top of main an array named s is declared that holds
// 3 ints. The program also uses creates and uses a class called Reader. In
// main the reader class is first used to read the users input. Then methods
// are called to parse through the input. The Reader class contains an index
// variable called i so I can move through the input. At the beginning of each
// of the methods the moveIndexToNextChar method is called so whitespace is not
// ignored.

#include <iostream>
#include <limits>
#include <cmath>
#include <iomanip>

const int MAX_BUF = 1024;

enum Command
{
    LI,
    MOVE,
    ADD,
    ADDU,
    SUB,
    SUBU,
    ADDI,
    ADDIU,
    MULT,
    MULTU,
    MFLO,
    MFHI,
    MUL,
    DIV,
    DIVU,
    SEQ,
    SNE,
    SLT,
    SLE,
    SGE,
    SLTI
};

// Reader class ===============================================================
class Reader
{
public:

    // constructor
    Reader()
        : i(0), nextRegisterInCalc(0)
    {}

    // methods that read ------------------------------------------------------
    bool readLine()
    {
        std::cin.getline(s, MAX_BUF);
        if (std::cin.eof()) return false;
        if (std::cin.fail() || std::cin.bad())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return true;
    }
    
    bool readCommand()
    {
        moveIndexToNextChar();
        if (s[i] == 'l' && s[i + 1] == 'i')
        {
            command = LI;
            i += 2;
            return true;
        }
        else if (s[i] == 'm' && s[i + 1] == 'o' && s[i + 2] == 'v'
                 && s[i + 3] == 'e')
        {
            command = MOVE;
            i += 4;
            return true;
        }
        else if (s[i] == 'a' && s[i + 1] == 'd' && s[i + 2] == 'd'
                 && s[i + 3] == 'i' && s[i + 4] == 'u')
        {
            command = ADDIU;
            i += 5;
            return true;
        }
        else if (s[i] == 'a' && s[i + 1] == 'd' && s[i + 2] == 'd'
                 && s[i + 3] == 'i')
        {
            command = ADDI;
            i += 4;
            return true;
        }
        else if (s[i] == 'a' && s[i + 1] == 'd' && s[i + 2] == 'd'
                 && s[i + 3] == 'u')
        {
            command = ADDU;
            i += 4;
            return true;
        }
        else if (s[i] == 'a' && s[i + 1] == 'd' && s[i + 2] == 'd')
        {
            command = ADD;
            i += 3;
            return true;
        }
        else if (s[i] == 's' && s[i + 1] == 'u' && s[i + 2] == 'b'
                 && s[i + 3] == 'u')
        {
            command = SUBU;
            i += 4;
            return true;
        }
        else if (s[i] == 's' && s[i + 1] == 'u' && s[i + 2] == 'b')
        {
            command = SUB;
            i += 3;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool readRegister()
    {
        moveIndexToNextChar();
        
        if (s[i] == '$' && s[i + 1] == 'v')
        {            
            if (s[i + 2] >= '0' && s[i + 2] <= '1')
            {
                registerInCalc[nextRegisterInCalc] = s[i + 2] - '0' + 2;
                ++nextRegisterInCalc;
                i += 3;
                return true;
            }
            else
            {
                return false;
            }
        }
        if (s[i] == '$' && s[i + 1] == 'a')
        {            
            if (s[i + 2] >= '0' && s[i + 2] <= '3')
            {
                registerInCalc[nextRegisterInCalc] = s[i + 2] - '0' + 4;
                ++nextRegisterInCalc;
                i += 3;
                return true;
            }
            else
            {
                return false;
            }
        }
        if (s[i] == '$' && s[i + 1] == 't')
        {
            if (s[i + 2] >= '0' && s[i + 2] <= '7')
            {
                registerInCalc[nextRegisterInCalc] = s[i + 2] - '0' + 8;
                ++nextRegisterInCalc;
                i += 3;
                return true;
            }
            if (s[i + 2] >= '8' && s[i + 2] <= '9')
            {
                registerInCalc[nextRegisterInCalc] = s[i + 2] - '0' + 16;
                ++nextRegisterInCalc;
                i += 3;
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (s[i] == '$' && s[i + 1] == 's')
        {
            if (s[i + 2] >= '0' && s[i + 2] <= '7')
            {
                registerInCalc[nextRegisterInCalc] = s[i + 2] - '0' + 16;
                ++nextRegisterInCalc;
                i += 3;
                return true;
            }
            if (s[i + 2] == '8')
            {
                registerInCalc[nextRegisterInCalc] = 30;
                ++nextRegisterInCalc;
                i += 3;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    bool readInt()
    {     
        moveIndexToNextChar();
        
        intInCalc = 0;

        // posneg (positive/negitve) a -1 is placed in this variable if the
        // first char is -. Regardless at the end the intInCalc is multiplied
        // by posneg
        int posneg = 1;
        if ((s[i] == '-'))
        {
            posneg = -1;
            ++i;
        }

        // I will first count how many digits the number contains starting at
        // i and store the results in intInCalcLength
        int intInCalcLength = 0;
        while (1)
        {
            ++intInCalcLength;
            // I didn't get this error handling working for the first char
            // if the user enters 'r' or 'r4' it passes. If the user enters 
            // 'sr' or '42r0' or 'aoijeo4w' it fails as it should. I haven't
            // worked it out but I know this is happening because 
            // ++intCalcLength happens before the first check for validation
            if (int(s[i + intInCalcLength]) < 48
                || int(s[i + intInCalcLength]) >= 58)
            {
                if (int(s[i + intInCalcLength]) == 0)                
                    break;
                else                
                    return false;                
            }
        }

        // now I will put the number into intInCalc using
        // i + intInCalcLength - k - 1
        // so as to move backward through the string copying the 1s, 10s, 100s
        // etc, until all is copied to intInCalc
        for (int k = 0; k < intInCalcLength; ++k)
        {
            intInCalc += (int(s[i + intInCalcLength - k - 1]) - 48)
                * pow(10, k);
        }

        // flip to negitive if nessessary (if posneg is -1)
        intInCalc = intInCalc * posneg;
        
        return true;
    }
    
    bool readComma()
    {
        moveIndexToNextChar();
        
        if (s[i] == ',')
        {
            i += 1;
            return true;
        }
        else
        {
            i += 1;
            return false;
        }
    }

    void moveIndexToNextChar()
    {
        while (s[i] == ' ')
        {
            ++i;
        }
    }

    // methods that get/set private members -----------------------------------
    int get_intInCalc()
    {
        return intInCalc;
    }

    void reset_nextRegisterInCalc()
    {
        nextRegisterInCalc = 0;
    }

    int get_registerInCalc(int i)
    {
        return registerInCalc[i];
    }

    Command get_command()
    {
        return command;
    }

    void reset_i()
    {
        i = 0;
    }

    // other methods ----------------------------------------------------------
    bool s_isEmpty()
    {
        if (int(s[0]) == 0)        
            return true;
        else
            return false;
    }
        
private:
    // the input is stored inside this class
    char s[MAX_BUF];
    // index variable that will keep track of what we have parsed through
    int i;
    // command variable of the enum Command type that will keep track of what
    // command (li, add, sub) is being used
    Command command;
    // this array stores what register is reference in what part of the input
    // so if the user enters
    // add $s1, $s0, $s2
    // it will look like this
    // registerInCalc[0] = 1
    // registerInCalc[1] = 0
    // registerInCalc[2] = 2
    int registerInCalc[3];
    // this keeps track of what registerInCalc will be written to next
    int nextRegisterInCalc;
    // this is where the int is stored only for the li command
    int intInCalc;
};

// LI function
void li(Reader reader, int reg[])
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command"
                  << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','" << std::endl;
        return;
    }
    
    // read next item expecting an int
    if (!reader.readInt())
    {
        std::cout << "ERROR: expected an int following li command"
                  << std::endl;
        return;
    }
    
    // perform li operation
    reg[reader.get_registerInCalc(0)] = reader.get_intInCalc();
}

// MOVE function
void move(Reader reader, int reg[])
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command"
                  << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
       std::cout << "ERROR: expected a ','" << std::endl;
       return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register" << std::endl;
        return;
    }
    
    // perform add operation
    reg[reader.get_registerInCalc(0)]
        = reg[reader.get_registerInCalc(1)];
}

// ADD function
void add(Reader reader, int reg[], bool usingUnsigned)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command"
                  << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','" << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register" << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','" << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register" << std::endl;
        return;
    }

    if (usingUnsigned)
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = (unsigned)reg[reader.get_registerInCalc(1)]
            + (unsigned)reg[reader.get_registerInCalc(2)];
    }
    else
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = reg[reader.get_registerInCalc(1)]
            + reg[reader.get_registerInCalc(2)];
    }
}

// SUB function
void sub(Reader reader, int reg[], bool usingUnsigned)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command"
                  << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
       std::cout << "ERROR: expected a ','" << std::endl;
       return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register" << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','" << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register" << std::endl;
        return;
    }
    

    if (usingUnsigned)
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = (unsigned)reg[reader.get_registerInCalc(1)]
            - (unsigned)reg[reader.get_registerInCalc(2)];
    }
    else
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = reg[reader.get_registerInCalc(1)]
            - reg[reader.get_registerInCalc(2)];
    }
}

// ADDI function
void addi(Reader reader, int reg[], bool usingUnsigned)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command"
                  << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','" << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register" << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','" << std::endl;
        return;
    }
    
    // read next item expecting an int
    if (!reader.readInt())
    {
        std::cout << "ERROR: expected an int"
                  << std::endl;
        return;
    }

    if (usingUnsigned)
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = (unsigned)reg[reader.get_registerInCalc(1)]
            + (unsigned)reader.get_intInCalc();
    }
    else
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = reg[reader.get_registerInCalc(1)]
            + reader.get_intInCalc();
    }
}

// print registers function
void printRegisters(int reg[], const int NUM_RESISTERS)
{
    std::cout << std::left
              << "                             General Registers" << std::endl
              << "R0  (r0) = " << std::setw(10) << reg[0]
              << "R8  (t0) = " << std::setw(10) << reg[8]
              << "R16  (s0) = " << std::setw(9) << reg[16]
              << "R24  (t8) = " << reg[24] << std::endl
              << "R1  (at) = " << std::setw(10) << reg[1]
              << "R9  (t1) = " << std::setw(10) << reg[9]
              << "R17  (s1) = " << std::setw(9) << reg[17]
              << "R25  (t9) = " << reg[25] << std::endl
              << "R2  (v0) = " << std::setw(10) << reg[2]
              << "R10  (t2) = " << std::setw(9) << reg[10]
              << "R18  (s2) = " << std::setw(9) << reg[18]
              << "R26  (k0) = " << reg[26] << std::endl
              << "R3  (v1) = " << std::setw(10) << reg[3]
              << "R11  (t3) = " << std::setw(9) << reg[11]
              << "R19  (s3) = " << std::setw(9) << reg[19]
              << "R27  (k1) = " << reg[27] << std::endl
              << "R4  (a0) = " << std::setw(10) << reg[4]
              << "R12  (t4) = " << std::setw(9) << reg[12]
              << "R20  (s4) = " << std::setw(9) << reg[20]
              << "R28  (gp) = " << reg[28] << std::endl
              << "R5  (a1) = " << std::setw(10) << reg[5]
              << "R13  (t5) = " << std::setw(9) << reg[13]
              << "R21  (s5) = " << std::setw(9) << reg[21]
              << "R29  (sp) = " << reg[29] << std::endl
              << "R6  (a2) = " << std::setw(10) << reg[6]
              << "R14  (t6) = " << std::setw(9) << reg[14]
              << "R22  (s6) = " << std::setw(9) << reg[22]
              << "R30  (s8) = " << reg[30] << std::endl
              << "R7  (a3) = " << std::setw(10) << reg[7]
              << "R15  (t7) = " << std::setw(9) << reg[15]
              << "R23  (s7) = " << std::setw(9) << reg[23]
              << "R31  (ra) = " << reg[31] << std::endl;
}

int main()
{
    // create an array for 32 registers
    const int NUM_REGISTERS = 32;
    int reg[NUM_REGISTERS] = {0};

    // put values into the gp and the sp
    reg[28] = 268468224;
    reg[29] = 2147479548;

    // create the reader object
    Reader reader;

    // if this variable turns to true the program will terminate
    bool exit = false;

    // main loop
    while (1)
    {
        std::cout << ">>> ";

        // get input from user
        if (!(reader.readLine()))
            break;

        // exit if empty string is entered
        if (reader.s_isEmpty())
            break;
        
        // read next item expecting a command
        if (!reader.readCommand())
            std::cout << "ERROR: expected command" << std::endl;

        // based on command branch off to parse through the rest of the line
        // and perform calcuation
        switch(reader.get_command())
        {
            case LI:
            {
                li(reader, reg);
                break;
            }
            case MOVE:
            {
                move(reader, reg);
                break;
            }
            case ADD:
            {
                add(reader, reg, false);
                break;
            }
            case ADDU:
            {
                add(reader, reg, true);
                break;
            }
            case SUB:
            {
                sub(reader, reg, false);
                break;
            }
            case SUBU:
            {
                sub(reader, reg, true);
                break;
            }
            case ADDI:
            {
                addi(reader, reg, false);
                break;
            }
            case ADDIU:
            {
                addi(reader, reg, true);
                break;
            }
        }
        
        printRegisters(reg, NUM_REGISTERS);

        // reset for next iteration
        reader.reset_i();
        reader.reset_nextRegisterInCalc();
    }

    std::cout << "END" << std::endl;

    return 0;
}
        /*
        // read next item expecting a register
        if (!reader.readRegister())
            std::cout << "ERROR: expected a register after command"
                      << std::endl;
        // read next item expecting a comma
        if (!reader.readComma())
            std::cout << "ERROR: expected a ','" << std::endl;

        // Now comes the part of the input that will vary depending on what
        // command is used
        
        if (reader.get_command() == LI)
        {
            // read next item expecting an int
            if (!reader.readInt())
                std::cout << "ERROR: expected an int following li command"
                          << std::endl;
            
            reg[reader.get_registerInCalc(0)] = reader.get_intInCalc();
        }

        if (reader.get_command() == ADD)
        {
            // read next item expecting a register
            if (!reader.readRegister())
                std::cout << "ERROR: expected a register" << std::endl;
            // read next item expecting a comma
            if (!reader.readComma())
                std::cout << "ERROR: expected a ','" << std::endl;
            // read next item expecting a register
            if (!reader.readRegister())
                std::cout << "ERROR: expected a register" << std::endl;

            // perform add calculation
            reg[reader.get_registerInCalc(0)]
                = reg[reader.get_registerInCalc(1)]
                + reg[reader.get_registerInCalc(2)];
        }

        if (reader.get_command() == SUB)
        {
            // read next item expecting a register
            if (!reader.readRegister())
                std::cout << "ERROR: expected a register" << std::endl;
            // read next item expecting a comma
            if (!reader.readComma())
                std::cout << "ERROR: expected a ','" << std::endl;
            // read next item expecting a register
            if (!reader.readRegister())
                std::cout << "ERROR: expected a register" << std::endl;

            // perform add calculation
            reg[reader.get_registerInCalc(0)]
                = reg[reader.get_registerInCalc(1)]
                - reg[reader.get_registerInCalc(2)];
        }
        */
