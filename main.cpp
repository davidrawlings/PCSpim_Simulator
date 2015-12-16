// ============================================================================
// David Rawlings
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
#include <fstream>
#include <string>

const int MAX_BUF = 1024;
const int TEXT_SEG_MAX_BUF = 80;
const int TEXT_SEG_MAX_SIZE = 100;

enum Command
{
    // error
    ERROR,
    // assembly commands
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
    //MUL,
    DIV,
    DIVU,
    SEQ,
    SNE,
    SLT,
    SGT,
    SLE,
    SGE,
    SLTI,
    SYSCALL,
    // change mode command
    MODE,
    // printing commands
    PRINT_REG,
    PRINT_DATA,
    PRINT_TEXT,
    // exec (to execute everything in the text and data segment)
    EXEC,
    // remove (to remove a line in the text segment)
    REMOVE,
    // exit command
    EXIT
};

enum Mode
{
    MODE_INTERACTIVE,
    MODE_TEXT,
    MODE_DATA
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

    // read line from the user input
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

    // read line from the text segment
    bool readLineTextSegment(char text[TEXT_SEG_MAX_SIZE][TEXT_SEG_MAX_BUF],
                             int i)
    {
        // this will put characters into s from a line in the data segment
        // the line that is read is determined by the parameter i
        for (int j = 0; j < TEXT_SEG_MAX_BUF; ++j)
        {            
            s[j] = text[i][j];
        }
        return true;
    } 
    
    bool readCommand()
    {
        moveIndexToNextChar();
        // when the user just presses enter with nothing inputed
        if (int(s[i]) == 0)
        {
            command = ERROR;
            return true;
        }
        // assembly commands
        if (s[i] == 'l' && s[i + 1] == 'i'
            && (s[i + 2] == 32 || s[i + 2] == 0 || s[i + 2] == 9))
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
        else if (s[i] == 'm' && s[i + 1] == 'u' && s[i + 2] == 'l'
                 && s[i + 3] == 't' && s[i + 4] == 'u')
        {
            command = MULTU;
            i += 5;
            return true;
        }
        else if (s[i] == 'm' && s[i + 1] == 'u' && s[i + 2] == 'l'
                 && s[i + 3] == 't')
        {
            command = MULT;
            i += 4;
            return true;
        }
        else if (s[i] == 'm' && s[i + 1] == 'f' && s[i + 2] == 'l'
                 && s[i + 3] == 'o')
        {
            command = MFLO;
            i += 4;
            return true;
        }
        else if (s[i] == 'm' && s[i + 1] == 'f' && s[i + 2] == 'h'
                 && s[i + 3] == 'i')
        {
            command = MFHI;
            i += 4;
            return true;
        }
        else if (s[i] == 'd' && s[i + 1] == 'i' && s[i + 2] == 'v'
                 && s[i + 3] == 'u')
        {
            command = DIVU;
            i += 4;
            return true;
        }
        else if (s[i] == 'd' && s[i + 1] == 'i' && s[i + 2] == 'v')
        {
            command = DIV;
            i += 3;
            return true;
        }
        else if (s[i] == 's' && s[i + 1] == 'l' && s[i + 2] == 't'
                 && s[i + 3] == 'i')
        {
            command = SLTI;
            i += 4;
            return true;
        }
        else if (s[i] == 's' && s[i + 1] == 'e' && s[i + 2] == 'q')
        {
            command = SEQ;
            i += 3;
            return true;
        }
        else if (s[i] == 's' && s[i + 1] == 'n' && s[i + 2] == 'e')
        {
            command = SNE;
            i += 3;
            return true;
        }
        else if (s[i] == 's' && s[i + 1] == 'l' && s[i + 2] == 't')
        {
            command = SLT;
            i += 3;
            return true;
        }
        else if (s[i] == 's' && s[i + 1] == 'g' && s[i + 2] == 't')
        {
            command = SGT;
            i += 3;
            return true;
        }
        else if (s[i] == 's' && s[i + 1] == 'l' && s[i + 2] == 'e')
        {
            command = SLE;
            i += 3;
            return true;
        }
        else if (s[i] == 's' && s[i + 1] == 'g' && s[i + 2] == 'e')
        {
            command = SGE;
            i += 3;
            return true;
        }
        else if (s[i] == 's' && s[i + 1] == 'y' && s[i + 2] == 's'
                 && s[i + 3] == 'c' && s[i + 4] == 'a' && s[i + 5] == 'l'
                 && s[i + 6] == 'l'
                 && (s[i + 7] == 32 || s[i + 7] == 0 || s[i + 7] == 9))
        {
            command = SYSCALL;
            i += 7;
            return true;
        }
        // change mode command
        else if (s[i] == 'm' && s[i + 1] == 'o' && s[i + 2] == 'd'
                 && s[i + 3] == 'e'
                 && (s[i + 4] == 32 || s[i + 4] == 0 || s[i + 4] == 9))
        {
            command = MODE;
            i += 4;
            return true;
        }
        // change mode shorcut command
        else if (s[i] == 'm'
                 && (s[i + 1] == 32 || s[i + 1] == 0 || s[i + 1] == 9))
        {
            command = MODE;
            i += 1;
            return true;
        }
        // printing commands
        else if (s[i] == 'r' && s[i + 1] == 'e' && s[i + 2] == 'g'
                 && (s[i + 3] == 32 || s[i + 3] == 0 || s[i + 3] == 9))
        {
            command = PRINT_REG;
            i += 3;
            return true;
        }
        else if (s[i] == 'd' && s[i + 1] == 'a' && s[i + 2] == 't'
                 && s[i + 3] == 'a' && (s[i + 4] == 32 || s[i + 4] == 0))
        {
            command = PRINT_DATA;
            i += 4;
            return true;
        }
        else if (s[i] == 't' && s[i + 1] == 'e' && s[i + 2] == 'x'
                 && s[i + 3] == 't'
                 && (s[i + 4] == 32 || s[i + 4] == 0 || s[i + 4] == 9))
        {
            command = PRINT_TEXT;
            i += 4;
            return true;
        }
        // exec command (to execute everything in the text and data segment)
        else if (s[i] == 'e' && s[i + 1] == 'x' && s[i + 2] == 'e'
                 && s[i + 3] == 'c'
                 && (s[i + 4] == 32 || s[i + 4] == 0 || s[i + 4] == 9))
        {
            command = EXEC;
            i += 4;
            return true;
        }
        // remove (to remove a line in the text segment)
        else if (s[i] == 'r' && s[i + 1] == 'e' && s[i + 2] == 'm'
                 && s[i + 3] == 'o' && s[i + 4] == 'v' && s[i + 5] == 'e'
                 && (s[i + 6] == 32 || s[i + 6] == 0 || s[i + 6] == 9))
        {
            command = REMOVE;
            i += 6;
            return true;
        }
        // exit command
        else if (s[i] == 'e' && s[i + 1] == 'x' && s[i + 2] == 'i'
                 && s[i + 3] == 't'
                 && (s[i + 4] == 32 || s[i + 4] == 0 || s[i + 4] == 9))
        {
            command = EXIT;
            i += 4;
            return true;
        }
        // if input doesn't match any command set command to error
        else
        {
            command = ERROR;
            return false;
        }
    }

    bool readMode()
    {
        moveIndexToNextChar();
        if (s[i] == 'i' && s[i + 1] == 'n' && s[i + 2] == 't'
            && s[i + 3] == 'e' && s[i + 4] == 'r'
            && s[i + 5] == 'a' && s[i + 6] == 'c' && s[i + 7] == 't'
            && s[i + 8] == 'i' && s[i + 9] == 'v' && s[i + 10] == 'e'
            && (s[i + 11] == 32 || s[i + 11] == 0 || s[i + 11] == 9)
            || s[i] == 'i'
            && (s[i + 1] == 32 || s[i + 1] == 0 || s[i + 1] == 9))
        {
            mode = MODE_INTERACTIVE;
            return true;
        }
        else if (s[i] == 't' && s[i + 1] == 'e' && s[i + 2] == 'x'
                 && s[i + 3] == 't'
                 && (s[i + 4] == 32 || s[i + 4] == 0 || s[i + 4] == 9)
                 || s[i] == 't'
                 && (s[i + 1] == 32 || s[i + 1] == 0 || s[i + 1] == 9))
        {
            mode = MODE_TEXT;
            return true;
        }
        else if (s[i] == 'd' && s[i + 1] == 'a' && s[i + 2] == 't'
                 && s[i + 3] == 'a'
                 && (s[i + 4] == 32 || s[i + 4] == 0 || s[i + 4] == 9)
                 || s[i] == 'd'
                 && (s[i + 1] == 32 || s[i + 1] == 0 || s[i + 1] == 9))
        {
            mode = MODE_DATA;
            return true;
        }
        else
        {
            command = ERROR;
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
        while (s[i] == 32 || s[i] == 9)
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

    Mode get_mode()
    {
        return mode;
    }

    char * get_s()
    {
        return s;
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
    // this holds the mode that is read in readMode so it can be returned
    // in get_mode
    Mode mode;
};

// LI function
void li(Reader reader, int reg[], int line = -1)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting an int
    if (!reader.readInt())
    {
        std::cout << "ERROR: expected an int following li command";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // perform li operation
    reg[reader.get_registerInCalc(0)] = reader.get_intInCalc();
}

// MOVE function
void move(Reader reader, int reg[], int line = -1)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout  << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // perform add operation
    reg[reader.get_registerInCalc(0)]
        = reg[reader.get_registerInCalc(1)];
}

// ADD function
void add(Reader reader, int reg[], bool usingUnsigned, int line = -1)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
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
void sub(Reader reader, int reg[], bool usingUnsigned, int line = -1)
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
       std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
       return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
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
void addi(Reader reader, int reg[], bool usingUnsigned, int line = -1)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting an int
    if (!reader.readInt())
    {
        std::cout << "ERROR: expected an int";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
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

// MULT function
void mult(Reader reader, int reg[], int & hi, int & lo, bool usingUnsigned,
          int line = -1)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }

    if (usingUnsigned)
    {
        // perform mult operation
        lo = (unsigned)reg[reader.get_registerInCalc(0)]
            * (unsigned)reg[reader.get_registerInCalc(1)]
            % (unsigned)2147483647;
        hi = (unsigned)reg[reader.get_registerInCalc(0)]
            * (unsigned)reg[reader.get_registerInCalc(1)]
            / (unsigned)2147483647;
    }
    else
    {
        // perform mult operation
        lo = reg[reader.get_registerInCalc(0)]
            * reg[reader.get_registerInCalc(1)]
            % 2147483647;
        hi = reg[reader.get_registerInCalc(0)]
            * reg[reader.get_registerInCalc(1)]
            / 2147483647;
    }
}

// MFLO function
void mflo(Reader reader, int reg[], int lo, int line = -1)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }

    // perform mflo operation
    reg[reader.get_registerInCalc(0)] = lo;
}

// MFLO function
void mfhi(Reader reader, int reg[], int hi, int line = -1)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }

    // perform mflo operation
    reg[reader.get_registerInCalc(0)] = hi;
}

// DIV function
void div(Reader reader, int reg[], int & hi, int & lo, bool usingUnsigned,
         int line = -1)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }

    if (usingUnsigned)
    {
        // perform mult operation
        lo = (unsigned)reg[reader.get_registerInCalc(0)]
            / (unsigned)reg[reader.get_registerInCalc(1)];
        hi = (unsigned)reg[reader.get_registerInCalc(0)]
            % (unsigned)reg[reader.get_registerInCalc(1)];
    }
    else
    {
        // perform mult operation
        lo = reg[reader.get_registerInCalc(0)]
            / reg[reader.get_registerInCalc(1)];
        hi = reg[reader.get_registerInCalc(0)]
            % reg[reader.get_registerInCalc(1)];
    }
}

// SET function
void set(Reader reader, int reg[], Command command, int i = 0, int line = -1)
{
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register after command";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a register
    if (!reader.readRegister())
    {
        std::cout << "ERROR: expected a register";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }
    
    // read next item expecting a comma
    if (!reader.readComma())
    {
        std::cout << "ERROR: expected a ','";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
        return;
    }

    if (command != SLTI)
    {
        // read next item expecting a register
        if (!reader.readRegister())
        {
            std::cout << "ERROR: expected a register";
            if (line != -1)
                std::cout << " line " << line;
            std::cout << std::endl;
            return;
        }
    }
    else
    {         
        // read next item expecting an int
        if (!reader.readInt())
        {
            std::cout << "ERROR: expected an int";
        if (line != -1)
            std::cout << " line " << line;
        std::cout << std::endl;
            return;
        }
    }
    
    if (command == SEQ)
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = reg[reader.get_registerInCalc(1)]
            == reg[reader.get_registerInCalc(2)];
    }
    else if (command == SNE)
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = reg[reader.get_registerInCalc(1)]
            != reg[reader.get_registerInCalc(2)];
    }
    else if (command == SLT)
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = reg[reader.get_registerInCalc(1)]
            < reg[reader.get_registerInCalc(2)];
    }
    else if (command == SGT)
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = reg[reader.get_registerInCalc(1)]
            > reg[reader.get_registerInCalc(2)];
    }
    else if (command == SLE)
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = reg[reader.get_registerInCalc(1)]
            <= reg[reader.get_registerInCalc(2)];
    }
    else if (command == SGE)
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = reg[reader.get_registerInCalc(1)]
            >= reg[reader.get_registerInCalc(2)];
    }
    else if (command == SLTI)
    {
        // perform add operation
        reg[reader.get_registerInCalc(0)]
            = reg[reader.get_registerInCalc(1)]
            < reader.get_intInCalc();
    }
}

// syscall function (unlike many of the command functions the bool returned
// does not indicate an error.)
bool syscall(int reg[], bool & exit)
{
    if (reg[2] == 1)
    {
        std::cout << reg[4] << std::endl;
    }
    if (reg[2] == 5)
    {
        std::cin >> reg[2];
    }
    if (reg[2] == 10)
    {
        exit = true;
    }
}

// parse line function
// the parse line function assumes the initial command has already been read
// and is being stored in the reader object. Then it parses through the read
// of the line and performs any nessessary operation
void parse_line(Reader reader, int reg[], int & hi, int & lo, bool & exit,
                int line = -1)
{
    // based on command branch off to parse through the rest of the
    // line and perform calcuation
    switch(reader.get_command())
    {
        // assembly commands
        case LI:
        {
            li(reader, reg, line);
            break;
        }
        case MOVE:
        {
            move(reader, reg, line);
            break;
        }
        case ADD:
        {
            add(reader, reg, false, line);
            break;
        }
        case ADDU:
        {
            add(reader, reg, true, line);
            break;
        }
        case SUB:
        {
            sub(reader, reg, false, line);
            break;
        }
        case SUBU:
        {
            sub(reader, reg, true, line);
            break;
        }
        case ADDI:
        {
            addi(reader, reg, false, line);
            break;
        }
        case ADDIU:
        {
            addi(reader, reg, true, line);
            break;
        }
        case MULT:
        {
            mult(reader, reg, hi, lo, false, line);
            break;
        }
        case MULTU:
        {
            mult(reader, reg, hi, lo, true, line);
            break;
        }
        case DIV:
        {
            div(reader, reg, hi, lo, false, line);
            break;
        }
        case DIVU:
        {
            div(reader, reg, hi, lo, true, line);
            break;
        }
        case MFLO:
        {
            mflo(reader, reg, lo, line);
            break;
        }
        case MFHI:
        {
            mfhi(reader, reg, hi, line);
            break;
        }
        case SEQ:
        case SNE:
        case SGT:
        case SLT:
        case SGE:
        case SLE:
        case SLTI:
        {
            set(reader, reg, reader.get_command(), line);
            break;
        }
        case SYSCALL:
        {
            syscall(reg, exit);
            break;
        }
    }
}

// EXEC function
void exec(char text[TEXT_SEG_MAX_SIZE][TEXT_SEG_MAX_BUF], int text_size,
          Reader reader, int reg[], int & hi, int & lo, bool & exit)
{
    for (int i = 0; i < text_size; ++i)
    {
        reader.readLineTextSegment(text, i);

        // we must read the first command on the outside of the parse_line
        // function because this is how it is set up for interactive mode
        if (!reader.readCommand())
            std::cout << "ERROR: command not recognized" << " line " << i + 4
                      << std::endl;
        parse_line(reader, reg, hi, lo, exit, i + 4);
        
        reader.reset_i();
        reader.reset_nextRegisterInCalc();
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

void printText(char text[TEXT_SEG_MAX_SIZE][TEXT_SEG_MAX_BUF], int text_size)
{
    for (int i = 0; i < text_size; ++i)
    {
        std::cout << "[0x00" << std::hex << 4194304 + i * 4 << "]    ";
        std::cout << std::dec << text[i] << std::endl;
    }

    
}

void moveToNextChar(std::string & s, int & i)
{
    while (s[i] == 32 || s[i] == 9)
    {
        ++i;
    }    
}

// read the first 3 lines of a .s file
bool readFirstThreeLines(std::ifstream & f, std::string & s)
{   
    /*
    for (int i = 0; i < 10; ++i)
    {
        std::cout << s[i] << ' ';
    }
    std::cout << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << int(s[i]) << ' ';
    }
    std::cout << std::endl;
    */

    /*
    int i = 0;
    while (s[i] == 32 || s[i] == 9)
    {
        ++i;
    }
    */

    int i;

    std::getline(f, s);
    i = 0;
    moveToNextChar(s, i);
    if (!(s[i] == '.' && s[i + 1] == 't' && s[i + 2] == 'e'
          && s[i + 3] == 'x' && s[i + 4] == 't'
          && (s[i + 5] == 32 || s[i + 5] == 0 || s[i + 5] == 9
              || s[i + 5] == 13)))
    {
        std::cout << "\nERROR: expected \".text\" at the top of the .s file, "
                  << "line 1 \n";
        return false;        
    }
    std::cout << s << std::endl;

    std::getline(f, s);
    i = 0;
    moveToNextChar(s, i);
    if (!(s[i] == '.' && s[i + 1] == 'g' && s[i + 2] == 'l'
          && s[i + 3] == 'o' && s[i + 4] == 'b' && s[i + 5] == 'l'
          && s[i + 6] == ' ' && s[i + 7] == 'm' && s[i + 8] == 'a'
          && s[i + 9] == 'i' && s[i + 10] == 'n'
          && (s[i + 11] == 32 || s[i + 11] == 0 || s[i + 11] == 9
              || s[i + 11] == 13)))
    {
        std::cout << "\nERROR: expected \".globl main\" at the top of the .s"
                  << " file, line 2\n";
        return false;        
    }
    std::cout << s << std::endl;

    std::getline(f, s);
    i = 0;
    moveToNextChar(s, i);
    if (!(s[i] == 'm' && s[i + 1] == 'a' && s[i + 2] == 'i'
          && s[i + 3] == 'n' && s[i + 4] == ':'
          && (s[i + 5] == 32 || s[i + 5] == 0 || s[i + 5] == 9
              || s[i + 5] == 13)))
    {
        std::cout << "\nERROR: expected \"main:\" at the top of the .s file, "
                  << "line 3\n";
        return false;        
    }
    std::cout << s << std::endl;
    
    return true;
}   

void readFileToTextSegment(char text[TEXT_SEG_MAX_SIZE][TEXT_SEG_MAX_BUF],
                           int & text_size, Reader reader)
{
    std::ifstream f("file.s", std::ios::in);
    std::string s;

    std::cout << "reading file..." << std::endl;
    
    if (!(readFirstThreeLines(f, s)))
        return;
    
    for (int i = 0; !f.eof(); ++i)
    {
        std::getline(f, s);

        std::cout << s << std::endl;

        // remove bad char        
        for (int j = 0; j < TEXT_SEG_MAX_BUF; ++j)
        {
            if (s[j] == 13)
                s[j] = 0;
        }
        
        for (int j = 0; j < TEXT_SEG_MAX_BUF; ++j)
        {
            text[i][j] = s[j];
        }
        ++text_size;
    }
    
    std::cout << "file has been read..." << std::endl;
}

void print_tutorial()
{
    std::cout << std::endl
              << "Welcome to the Spimulator! (PC Spim Simulator)\n"
              << "There are 3 modes (Interactive, Text, Data).\n"
              << "You can change from mode to mode using the mode command.\n"
              << "Ex: \"mode text\" will change the mode to text.\n"
              << "You can also change mode by appreviating to one letter.\n"
              << "Ex: \"m i\" will change the mode to interactive.\n\n"
              << "To print the registers \"reg\"\n"
              << "To print the text segment \"text\"\n"
              << "To print the data segment \"data\"\n\n"
              << "In interactive mode you can enter in simple MIPS\n"
              << "instructions that are immediately interperted by the\n"
              << "simulator and will change the registers.\n\n"
              << "In text mode you can enter in instructions just like in\n"
              << "interactive mode but the instructions are saved into the\n"
              << "text segment. While in text mode you can use the\n"
              << "\"remove\" command to remove the most recent line from\n"
              << "the text segment. You can use the \"exec\" command to \n"
              << "execute all the commands in the text segment.\n\n"
              << "The simulator will automatically read the \"file.s\" into\n"
              << "the text segment.\n"
              << std::endl;
}

int main()
{        
    // create an array for 32 registers
    const int NUM_REGISTERS = 32;
    int reg[NUM_REGISTERS] = {0};

    // put values into the gp and the sp
    reg[28] = 268468224;
    reg[29] = 2147479548;

    // create hi and lo registers
    int hi = 0;
    int lo = 0;

    // text segment array
    char text[TEXT_SEG_MAX_SIZE][TEXT_SEG_MAX_BUF];
    // index to move through text segment when a line is saved it is saved at
    // this index and then the index will be increased by 1
    int text_size = 0;
    /*
    char b[15] = "li $s0, 1";
    for (int i = 0; i < TEXT_SEG_MAX_BUF; ++i)
    {
        text[0][i] = b[i];
    }
    */

    // create the reader object
    Reader reader;

    // read a file
    readFileToTextSegment(text, text_size, reader);

    // mode
    Mode mode = MODE_INTERACTIVE;

    // if this variable turns to true the program will terminate
    bool exit = false;

    // print tutorial instructions for using the simulator
    print_tutorial();
    
    // main loop
    while (!exit)
    {
        // print out the current mode and then ">>>"
        switch (mode)
        {
            case (MODE_INTERACTIVE):
                std::cout << "Interactive ";
                break;
            case (MODE_TEXT):
                std::cout << "Text ";
                break;
            case (MODE_DATA):
                std::cout << "Data ";
                break;
        }
        std::cout << ">>> ";

        // get input from user
        if (!(reader.readLine()))
            std::cout << "ERROR: readline error" << std::endl;
        
        // read first item expecting a command
        if (!reader.readCommand())
            std::cout << "ERROR: command not recognized" << std::endl;

        // ---------------- commands used across modes -----------------------
        
        // check for user changing mode
        if (reader.get_command() == MODE)
        {
            if (!reader.readMode())
                std::cout << "ERROR: mode not recognized" << std::endl;
            mode = reader.get_mode();
        }
        // check for user printing registers
        else if (reader.get_command() == PRINT_REG)
        {                            
            printRegisters(reg, NUM_REGISTERS);
        }
        // check for user printing text segment
        else if (reader.get_command() == PRINT_TEXT)
        {
            printText(text, text_size);
        }
        // check for user executing the program in the text and data segment
        else if (reader.get_command() == EXEC)
        {
            // reset for exec
            reader.reset_i();
            reader.reset_nextRegisterInCalc();
            // call exec
            exec(text, text_size, reader, reg, hi, lo, exit);
        }
        // check for user removing a line from the data segment
        else if (reader.get_command() == REMOVE)
        {
            if (mode == MODE_TEXT)
            {
                if (text_size > 0)
                    --text_size;
            }
        }
        // check for user exiting
        else if (reader.get_command() == EXIT)
        {
            exit = true;
        }

        // -------------------- interactive mode -----------------------------
        else if (mode == MODE_INTERACTIVE)
        {
            parse_line(reader, reg, hi, lo, exit);
        }
        
        // ---------------------- text mode ----------------------------------
        else if (mode == MODE_TEXT)
        {
            // create a pointer to the inputed string
            char * s = reader.get_s();
            // reset the i and recheck if the first command was valid
            // and only send the line into the text segment if so
            reader.reset_i();
            if (reader.readCommand())
            {
                for (int i = 0; i < TEXT_SEG_MAX_BUF; ++i)
                {
                    text[text_size][i] = s[i];
                }
                ++text_size;
            }
        }

        // reset for next iteration
        reader.reset_i();
        reader.reset_nextRegisterInCalc();
    }

    return 0;
}
