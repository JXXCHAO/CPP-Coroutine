#define __cpp_lib_coroutine

#include <iostream>
#include <coroutine>
//#include <exception>

template<typename T>
struct Generator {
    struct promise_type;

    struct SuspendWithValue{
        promise_type *promise;

        constexpr bool await_ready() const noexcept { return false; }

        constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}

        T await_resume() const noexcept {
            return promise->value;
        }
    };
    struct promise_type {

        T value;


        std::suspend_always initial_suspend() { return {}; }

        std::suspend_never final_suspend() noexcept { return {}; }

        SuspendWithValue yield_value(T t) {
            value = t;
            return {.promise=this};
        }


        void unhandled_exception() {

        }

        Generator<T> get_return_object() {
            return Generator<T>{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        void return_void() {

        }


    };

    std::coroutine_handle<promise_type> handle;

    T next() {
        handle.resume();
        return handle.promise().value;
    }

    T next(T value) {
        handle.promise().value = value;
        handle.resume();
        return handle.promise().value;
    }

    ~Generator() {
        handle.destroy();
    }
};


Generator<int> sequence() {
    int i = 0;
    while (true) {
        int j = co_yield i++;
        i = j + 5;
    }
}

auto CallCoroutine() {
    auto gen = sequence();
    for (int i = 0

            ; i < 10; ++i) {
        std::cout << gen.next(i * 10) << std::endl;
    }
    return gen.handle;
}


int main() {
    auto handle = CallCoroutine();
//    handle.promise().value = 1000;
//    handle.resume();
//    std::cout << handle.promise().value << std::endl;



//    std::cout << "=========================================" << std::endl;
//    auto gen2 = []() -> Generator<int64_t> {
//        co_yield 0;
//        co_yield 1;
//        int a = 0;
//        int b = 1;
//        while (true) {
//            co_yield a + b;
//            b = a + b;
//            a = b - a;
//        };
//    }();
//
//    for (int i = 0; i < 10; ++i) {
//        std::cout << gen2.next() << std::endl;
//    }

    return 0;
}
