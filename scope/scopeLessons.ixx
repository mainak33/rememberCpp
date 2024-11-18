export module scopeLessons;

import <iostream>;
import lessons;
import printCode;
using lessons::VoidLesson;
extern template VoidLesson;

using printCode::printCodeBlock;
using printCode::printTextLine;
using lessons::VoidLesson;
const auto &localPrintCodeBlock = printCodeBlock;
export namespace scopeLessons {
    void L1run() {
        localPrintCodeBlock(R"(
            // Nested functions are not allowed in C++ :
            /*
            void outerFunction() {
                // Compile Error: Nested functions not allowed
                void innerFunction(){
                    i++;
                }
            }
            */
        )");
    }

    VoidLesson L1(" L1 : Illegal Nested Functions", L1run);

    void L2run() {
        printCodeBlock(R"(
           // Lambdas may be used inside functions 
            /* This is ok :
            void outerFunction() {
                // Lambda allowed
                auto innerScope = [&i](){i++;};
            }
            */
        )");

        printCodeBlock(R"(
            // Inside outerFunction:

            int i = 5;
            auto innerScope = [&i](){i++;};
    
            // Lambda must be called to modify value
            std::cout << "i before calling innerScope =" << i << '\n';
            innerScope();
            std::cout << "i after calling innerScope =" << i << '\n';
        )");
    
        printTextLine("evaluates to :");

        // Inside outerFunction:

        int i = 5;
        auto innerScope = [&i]() {i++; };

        // Lambda must be called to modify value
        std::cout << "i before calling innerScope =" << i << '\n';
        innerScope();
        std::cout << "i after calling innerScope =" << i << '\n';
    }

    VoidLesson L2(" L2 : Lambdas in Functions", L2run);
}