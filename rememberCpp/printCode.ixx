export module printCode; 

import <iostream>;
import <ostream>;
import <iomanip>;
import <string>;

export namespace printCode{
    const int maxCodeLineLength = 80;

    void repeatedCharacterOutput(char c, int nRepeat, bool newline = true)
    {
        std::cout << std::setfill(c) << std::setw(nRepeat); 
        if(newline) std::cout << '\n';
    }

    void printDelimiter(char c = '*') {
        repeatedCharacterOutput(c, maxCodeLineLength, true);
    }

    void printTextLine(const std::string& line) {
        std::cout << line << '\n';
    }
    void printCodeInline(const std::string& code) {
        std::cout << code ;
    }

    void printCodeBlock(const std::string & code) {
        //printCodeBlockDelimiter();
        std::cout << code << '\n';
        //printCodeBlockDelimiter();
    }
}