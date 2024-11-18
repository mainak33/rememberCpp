export module lessons;
import printCode;
import <iostream>;
import <functional>;
import <string>;
import <type_traits>;
import <memory>;

template <size_t V, typename... T>
decltype(auto) getParameterPackVals(T&&... Args) noexcept {
    return std::get<V>(std::forward_as_tuple(std::forward<T>(Args)...));
}
// Doc says For :
// template< class Fn, class... ArgTypes >
// struct is_invocable; 
// template< class R, class Fn, class... ArgTypes >
// struct is_invocable_r;
// If Fn, R or any type in the parameter pack ArgTypes is not a complete type, (possibly cv-qualified) void, or an array of unknown bound, the behavior is undefined. 
// So compound expression is needed for convertibleToFunction concept below:

template <class InputInvocable, class TReturn, class ... TArguments>
concept convertibleToFunction = 
    (
        !std::is_void_v<TReturn> && 
        std::is_invocable_r_v< TReturn, InputInvocable, TArguments ... >  
        && std::is_convertible_v<InputInvocable, std::function<TReturn(TArguments...)>>
    )
    ||
    (
        std::is_void_v<TReturn> && 
        std::is_invocable_v<InputInvocable, TArguments ... >
        && std::is_convertible_v<InputInvocable, std::function<void(TArguments...)>>
    );

export namespace lessons{
    class BaseLesson;

    using BaseLessonWrapper = std::reference_wrapper<BaseLesson>;
    using BaseLessonVector = std::vector<BaseLessonWrapper>;
    BaseLessonVector&
    cppLessons() {
        static BaseLessonVector cppLessonsObj;
        if(0 == cppLessonsObj.size()){
            cppLessonsObj.reserve(30);
        }
        return cppLessonsObj;
    }
    
    class BaseLesson {
            std::string name;
        public :
            bool isVoidLesson = false;
            explicit BaseLesson(
                const std::string &name
            ):
            name{std::move(name)}{
                
            }

            virtual std::string getName() const noexcept{
                return name;
            }

            //virtual void printLessonToConsole() {};
    };



    template <class TReturn, class ... TArguments>
    class GenericLesson : public BaseLesson {
        public:
            std::string game;
            const std::function<TReturn(TArguments...)> printCallback;
            explicit GenericLesson(
                std::string&& name,
                convertibleToFunction<TReturn, TArguments...> auto && printCallback
            ):
            BaseLesson{ std::move(name) },
            game{name},
            printCallback{std::move(printCallback)}{
                if constexpr (
                    std::is_void_v<TReturn> && 
                    (   sizeof...(TArguments) == 0 || 
                        (sizeof...(TArguments) == 1 && (std::is_void_v<TArguments> && ...))
                    )
                ) BaseLesson::isVoidLesson = true;// && is== void && TArguments == void) 

                
                try{
                    auto & tempref = dynamic_cast<BaseLesson&>(*this);
                    cppLessons().emplace_back(tempref);
                }
                catch (std::bad_cast) {

                    std::cout << "Error in upcasting to Baselesson" << "\n";
                    exit(-1);
                }

            };

            //template <class TReturn, class ... TArguments>
            TReturn printLessonToConsole(TArguments && ...args) {
                printCode::printDelimiter('_');

                if constexpr (std::is_void_v<TReturn>){
                    std::invoke(this->printCallback, std::forward<TArguments>(args)...);
                    printCode::printDelimiter('_');
                    return;
                }else{
                    TReturn returnObj = this->printCallback(args...);
                    printCode::printDelimiter('_');
                    return returnObj;
                }
            }
    };

    template class GenericLesson<void>;
    template class GenericLesson<int,int>;
    template class GenericLesson<int>;
    using VoidLesson = lessons::GenericLesson<void>;
    GenericLesson<int> LIntRet{"LIntRet",[]()->int{return 0;}};
    struct ErrorFlag{
        bool isError;
    };
    GenericLesson<ErrorFlag> LErrRet{ "LIntRet",[]()->ErrorFlag {std::cout<<"aaaaa"; return {true}; }};


    


    //template void lessons::GenericLesson<void>::printLessonToConsole();
    //using print = VoidLesson::printLessonToConsole<void>();
    //template class GenericLesson<void>::printLessonToConsole<void>;
    //using VoidVoidLesson = GenericLesson<void, void>;
    //int lint(int a ) {return a;};
    //void lvoid(void) { return ; };
   
    ////isVoid <void, void> auto runFuction = llll;
    //convertibleToFunction<void> auto runFuction = lvoid;
    //convertibleToFunction<int, int> auto runFuctionInt = lint;
    //lessons::GenericLesson<void> Avoid{ "Concepts Lesson 1",lvoid };
    //lessons::GenericLesson<int, int> Aint{ "Concepts Lesson 1",lint };
}


//template<class Fn, class... ArgTypes >
//concept C = std::is_nothrow_invocable_v< Fn, ArgTypes... >;
//C<int> auto fn = [](int a) noexcept {return a;};
//using fntype = decltype(fn);
//static_assert(std::is_nothrow_invocable_v<fntype, int>);