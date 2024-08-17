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


                Holder(std::unique_ptr<T> source) : _object(std::move(source)) {}


                Holder(Holder&& other) : _object(std::move(other._object)) {
                other._object = nullptr;
                }

//


                Holder& operator=(const Holder& other) = delete;
                Holder& operator=(const Holder&& other) {
                    _object = std::make_unique<T>(std::move(other._object));
                    other._object = nullptr;
                };


                //Holder(const Holder& other) = delete;
                    


                bool operator==(Holder& other) {
                    return this->_object == other._object;

                }



                inline constexpr T& get() const {
                    return *_object.get();
                }

                inline constexpr T* getPointer() const {
                    return _object.get();
                }



        };

}

#endif /*HOLDER_H*/
