// ============================================================================
// David Rawlings
// ============================================================================
// ciss360 a03q06
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

const int MAX_BUF = 1024;

enum Command
{
    LI,
    ADD,
    SUB
};

class Reader
{
public:

    Reader()
        : i(0), nextRegisterInCalc(0)
    {}

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
        else if (s[i] == 'a' && s[i + 1] == 'd' && s[i + 2] == 'd')
        {
            command = ADD;
            i += 3;
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
        
        if (s[i] == '$' && s[i + 1] == 's')
        {
            if (s[i + 2] == '0')
            {
                registerInCalc[nextRegisterInCalc] = 0;
                ++nextRegisterInCalc;
                i += 3;
                return true;
            }
            else if (s[i + 2] == '1')
            {
                registerInCalc[nextRegisterInCalc] = 1;
                ++nextRegisterInCalc;
                i += 3;
                return true;
            }
            else if (s[i + 2] == '2')
            {
                registerInCalc[nextRegisterInCalc] = 2;
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

int main()
{
    int s[3] = {0, 0, 0};
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
        if (int(reader.s[0]) == 0)
            break;

        // The first part of the all input will be the same
        
        // read next item expecting li, add, or sub
        if (!reader.readCommand())
            std::cout << "ERROR: expected li, add, or sub" << std::endl;
        // read next item expecting a register
        if (!reader.readRegister())
            std::cout << "ERROR: expected a register after command"
                      << std::endl;
        // read next item expecting a comma
        if (!reader.readComma())
            std::cout << "ERROR: expected a ','" << std::endl;

        // Now comes the part of the input that will vary depending on what
        // command is used
        
        if (reader.command == LI)
        {
            // read next item expecting an int
            if (!reader.readInt())
                std::cout << "ERROR: expected an int following li command"
                          << std::endl;
            
            s[reader.registerInCalc[0]] = reader.intInCalc;
        }

        if (reader.command == ADD)
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
            s[reader.registerInCalc[0]] = s[reader.registerInCalc[1]]
                + s[reader.registerInCalc[2]];
        }

        if (reader.command == SUB)
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
            s[reader.registerInCalc[0]] = s[reader.registerInCalc[1]]
                - s[reader.registerInCalc[2]];
        }

        // print out contents of registers
        for (int i = 0; i < 3; ++i)
        {
            std::cout << "$s" << i << ": " << s[i] << std::endl;
        }

        // reset for next iteration
        reader.i = 0;
        reader.nextRegisterInCalc = 0;
    }

    std::cout << "END" << std::endl;

    return 0;
}
