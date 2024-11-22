//import scopeLessons;
import conceptsLessons;
import <functional>;
import <string>;
import <iostream>;
import lessons;

#include <cassert>
#include "osUtils.h"
#include "ioUtils.h"
#include "svUtils.h"

template<class R, class ...Args>
R test(std::function<R(Args ...)> fn, Args &&... args) {
    return fn(std::forward<Args>(args)...);
  
};
int invoke(std::function<int()> fn) {
    return fn();
};

template<class Func>
int invokeTemplate1(Func fn) {
    return fn();
}

template<class T1, class T2>
T1 invokeTemplate2(std::function<T1(T2)> fn, T1 t1) {
    return fn(t1);
};

template<class T1, class ...TARGS>
T1 invokeTemplateN(std::function<T1(TARGS...)> fn, TARGS... targs) {
    return fn(targs...);
};

template<class T1, class ...TARGS>
T1 invokeTemplateNForwarded(std::function<T1(TARGS...)>&& fn, TARGS &&... targs) {
    return std::forward<std::function<T1(TARGS...)>>(fn)(std::forward<TARGS>(targs)...);
};

template< class FnType, class T1, class ...TARGS>
concept isInvocabletoResult = 
    std::invocable<FnType, TARGS...> &&
    requires (FnType&& fn, TARGS &&... targs){
        { std::invoke(std::forward<FnType>(fn), std::forward<TARGS>(targs)...) } -> std::convertible_to<T1>;
    };

template<typename FnType>
concept isInvocabletoint = isInvocabletoResult<FnType, int, int>;



template<class T1, class ...TARGS>
T1 invokeTemplateInvoke(isInvocabletoResult< T1, TARGS...> auto fn, TARGS &&... targs)
{
    return std::invoke(std::forward<decltype(fn)>(fn), std::forward<TARGS>(targs)...);
};

using std::cin;
using std::cout;
using osUtils::OS;
using osUtils::clearScreen;
using ioUtils::getNumberInRange;
using ioUtils::IntegerString;
using DigitInString = IntegerString::DigitInString;
int main(int argc, char* argv[]) {
    
    for (auto &lessonRef : lessons::cppLessons()) {
        auto & lesson = lessonRef.get();
        cout << lesson.getName() << '\n';
        if (lesson.isVoidLesson){
            try{
                lessons::VoidLesson& rLesson = dynamic_cast<lessons::VoidLesson &>(lesson);
                cout << "Is a VoidLesson" <<  '\n';
                rLesson.printLessonToConsole();
            }catch(std::bad_cast){
                cout << "Error in downcasting to Voidlesson" << "\n";
            }
        }
    }

    if (OS::is(OS::NAME::WINDOWS64))
        cout << "Is a Win 64 bit OS \n";
    if (OS::is(OS::NAME::WINDOWS))
        cout << "Is a WinOS \n";
    if (!OS::is(OS::NAME::LINUX))
        cout << "Is not a Linux OS \n";
    
    clearScreen();
    //assert(IntegerString {"-0000007699806578356817" } > IntegerString{ "+000007" });
    IntegerString{"0"};
   /* std::string x{"76356817"};
    ioUtils::IntegerString intstr{"-0000007699806578356817"};
    DigitInString dig = intstr.mostSignificantDigit();
    cout << dig.digit << '\n';*/
    
    /*int x = getNumberInRange<int>(1, 6, "Choose an option from 1 to 5");
    cout << "Valid number : " << x << '\n';
    IntegerString xi = getNumberInRange<IntegerString>({"1"}, {"3"}, "Choose an option from 1 or 2");
    cout << "Valid option : " << xi << '\n';
    return 0;*/

    svUtils::wrapToLength(
        cout,
        "string_view lines there is a a very long line \n.Something elsecanbedone thisisaverylongcontiguouswordwithoutadelimiter then thereare three newlines\n \n\nfollowedbysomelongibberish ",
        {
            "   ",
            20,
            " ",
            0
        }
    );
}