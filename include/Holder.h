#ifndef HOLDER_H
#define HOLDER_H


#include <concepts>
#include <memory>


namespace Wind {
    template<typename T> requires std::destructible<T> && std::move_constructible<T>
        class Holder {

            private:

                std::shared_ptr<T> _object;


            public:

                Holder(T&& obj) : _object(obj) {}


                Holder& operator=(const Holder& other) = delete;
                Holder& operator=(const Holder&& other) = default;


                Holder(const Holder& other) = delete;
                    

                std::weak_ptr<T> makeCopy() {
                    std::weak_ptr<T> w = _object;
                    return w;

                }

                bool operator==(Holder& other) {
                    return this->_object == other._object;

                }

                T& operator->() {
                    return *_object.get();
                }

                T& operator*() {
                    return *_object.get();
                }


                T& get() {
                    return *_object.get();
                }


        };

}

#endif /*HOLDER_H*/
