#ifndef HOLDER_H
#define HOLDER_H
#include <algorithm>
#include <concepts>
#include <memory>
#include <utility>


namespace Wind {
    template<typename T> requires std::destructible<T> && std::move_constructible<T>
        class Holder {

            private:

                std::unique_ptr<T> _object;


            public:

                Holder(T&& obj) : _object(std::make_unique<T>(std::move(obj))) { }


                Holder() : _object(nullptr) {}


                Holder(Holder&& other) : _object(std::make_unique<T>(std::move(other._object))) {
                other._object = nullptr;
                }

//
//                Holder(T& obj) : _object(obj){}

                //Holder (T obj) : _object(obj){}


                Holder& operator=(const Holder& other) = delete;
                Holder& operator=(const Holder&& other) {
                    _object = std::make_unique<T>(std::move(other._object));
                    other._object = nullptr;
                };


                //Holder(const Holder& other) = delete;
                    


                bool operator==(Holder& other) {
                    return this->_object == other._object;

                }



                T& get() {
                    return *_object.get();
                }



        };

}

#endif /*HOLDER_H*/
