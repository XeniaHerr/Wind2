#include <functional>

class Action {

    public:


//        void operator()() {
//        f_();
//        }

        void call() {
            f_();
        }

        template<typename Func, typename...Args> requires std::invocable<Func, Args...>
            void registerFunc(Func&& F, Args&&... args){

                f_ = [func = std::bind(std::forward<Func>(F), std::forward<Args>(args)...)] mutable {
                    func();
                };

            }

        template<typename Func, typename...Args> requires std::invocable<Func, Args...>
            Action(Func&& F, Args&&... args){

                f_ = [func = std::bind(std::forward<Func>(F), std::forward<Args>(args)...)] mutable {
                    func();
                };

            }


        void registerFunc(std::function<void()> f) {
            f_ = f;
        }


        Action() : Action([](){return false;}){}

    private:

        std::function<void()> f_;
};
