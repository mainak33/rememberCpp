export module conceptsLessons;

import lessons;
import printCode;
import <string>;
import <type_traits>;
import <typeinfo>;
import <functional>;
import <format>;

using lessons::VoidLesson;
extern template VoidLesson;

template<class T>
constexpr bool isGreaterThan0 = T > 0;

template<int T>
concept Positive_Incorrectly_Defined =
    requires{
    T > 0; // requires T > 0 (a requirement clause) would correct this issue
};

using namespace printCode;
export namespace conceptsLessons{

    template <size_t Position, typename UnsignedIntegerType>
    concept isValidBitPositionForType =
        std::unsigned_integral<UnsignedIntegerType> &&
        Position <= sizeof(UnsignedIntegerType) * size_t { 8 }&&
        Position > 0;

    //--------------------------------------------------------------------------
    template<int T>
    concept Positive_Incorrectly_Defined =
        requires{T > 0;};

    template <int I>
    requires Positive_Incorrectly_Defined <I>
    void printIncorrect()
    {
        std::cout << std::format("printIncorrect : T is {} \n", I);
    }

    template<int T>
    concept Positive =
        requires{
            requires T > 0;
        };

    template <int I>
        requires Positive<I>
    void printCorrect()
    {
        std::cout << std::format("printCorrect : T is {} \n", I);
    }

    template<int T>
    concept Positive_2 = T > 0;

    template <int I>
        requires Positive_2<I>
    void printCorrect_2()
    {
        std::cout << std::format("printCorrect_2 : T is {} \n", I);
    }

    template <int I>
    requires requires {requires I > 0;}
    void printCorrect_3()
    {
        std::cout << std::format("printCorrect_3 : T is {} \n", I);
    }

    template <int I>
    requires (I > 0)
    void printCorrect_4()
    {
        std::cout << std::format("printCorrect_4 : T is {} \n", I);
    }


    //----------------------------------------------------------------------------
    void L1run(void) {
        printCodeBlock(R"(
We want to define a concept which allows only positive numbers:
We may naively try the following:
    template<int T>
    concept Positive_Incorrectly_Defined =
        requires{
            T > 0;
        };

To test the concept we use the function:
    template <int I>
    requires Positive_Incorrectly_Defined <I>
    void printIncorrect()
    {
        std::cout << std::format("printIncorrect : T is {} \n", I);
    }
Invoking printIncorrect<-1>() yields :
 )");
        printIncorrect<-1>();

        printCodeBlock(R"(
This is because the requires sequence which in this case is a simple expression only evaluates whether the expression compiles. 
This can be fixed by using a requirement clause instead :
template<int T>
concept Positive =
    requires{
        requires T > 0;
};

template <int I>
    requires Positive<I>
void printCorrect()
{
    std::cout << std::format("printCorrect : T is {} \n", I);
}
//printCorrect<-1>(); // will fail to compile
printCorrect<1>(); will yields:
)");
        //printCorrect<-1>();// will fail to compile
        printCorrect<1>();
        
        printCodeBlock(R"(
The actual concept may be simplified to a boolean expression T > 0 :
template<int T>
    concept Positive_2 = T > 0;

template <int I>
    requires Positive_2<I>
void printCorrect_2()
{
    std::cout << std::format("printCorrect_2 : T is {} \n", I);
}
//printCorrect_2<-1>() will not compile;
printCorrect_2<1>(); will yield:
)");
        //printCorrect_2<-1>();
        printCorrect_2<1>();

        printCodeBlock(R"(
This is sometimes referred to as the "requires requires" pattern when using concept inline directly in the template function :
Thiis is ofte used with more complicated requirement expression which together define a concept (requires {requirement seqeunce})
template <int I>
requires requires {requires I > 0;}
void printCorrect_3()
{
    std::cout << std::format("printCorrect_3 : T is {} \n", I);
}

The most compact expression which works is:
template <int I>
requires (I > 0)
void printCorrect_4()
{
    std::cout << std::format("printCorrect_4 : T is {} \n", I);
}
//printCorrect_3<-1>(); // will not compile
printCorrect_3<1>();
        
//printCorrect_4<-1>(); // will not compile
printCorrect_4<1>();
    )");
       
//printCorrect_3<-1>(); // will not compile
printCorrect_3<1>();
        
//printCorrect_4<-1>(); // will not compile
printCorrect_4<1>();
}

    lessons::VoidLesson L1{ "Requirement Clauses ", L1run };

    
    template <class InputInvocable, class TReturn, class ... TArguments>
    concept convertibleToFunction =
        requires (InputInvocable && invocable, TArguments &&... targs) {
            { std::is_invocable_r_v<TReturn, InputInvocable, TArguments ...> };
            { std::is_convertible_v<InputInvocable, std::function<TReturn(TArguments...)>> };
            { std::invoke(std::forward<InputInvocable>(invocable), std::forward<TArguments>(targs)...) } -> std::convertible_to<TReturn>;
    };

    void invoker1(std::function<void(void)> f1) {
        return f1();
    };

    template <class ...Args>
    void invokerT1(std::function<void(Args...)> f1, Args...args) {
        return f1(args...);
    };

    template <class ...Args>
    void invokerT2(std::function<void(Args...)> && f2, Args && ...args) {
        return std::forward<std::function<void(Args &&...)>>(f2)(std::forward<Args>(args)...);
    };


    template <class ...Args>
    void invokerT3(std::invocable<Args...> auto f3, Args ...args) {
        return std::invoke(f3,args...);
    };    


    void testf(int a) {};
        

    void L2run(void){
        printCodeBlock(R"(

        
        )");
        
        invoker1([]() {});
      
        // Need the {} inside function call to create a std::function
        //invokerT1<> ([]() {}); //Does not compile
        invokerT1<>({ []() {} }); // Compiles

        // Need the {} inside function call to create a std::function (for non-void template type as well)
        //invokerT1<int> ([](int a){}, 5); //Does not compile
        invokerT1<int>({ [](int a) {} }, 5);
        invokerT1<int>(testf, 5);
        
        // Both variations compile with perfect forwarding
        int ain = 5;
        invokerT2<int>([](int a) {}, 5);
        invokerT2<int&>([](int a) {}, ain);
        invokerT2<int>({[](int a) {}}, 5); 
        invokerT2<int>(testf, 5); 
        invokerT2({[](int a) {}}, ain); 
        invokerT2<int&>(testf, ain); 
        
        
        // std::invoke does the job for us
        std::invocable<int> auto fn = [](int a) {};
        invokerT3<int>([](int a) {} , 5); 
        invokerT3<int>([](int a) {}, ain); 
        //invokerT3<int>({[](int a) {}}, ain); 
        invokerT3<int>(testf , 5);
        invokerT3<>([](){});
    };
    
    lessons::VoidLesson L2{ "Concepts Lesson 1", L2run };

   /* initialize(){
        L5{"Concepts Lesson 1", L1run};
    }*/

    //lessons::GenericLesson<void, void> A1{"Concepts Lesson 1", L1run};

    
}