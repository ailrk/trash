#include <iostream>
#include <memory>

struct Add1 {
    int call(int x) const { return x + 1; }
};

struct Sub1 {
    int call(int x) const { return x - 1; }
};

////////////////////////////////////////////////////////////////////
// Interface
struct ICallback {
    virtual int call(int) const = 0;
};

// vtable layer
template <typename T> struct WrappingCallback : ICallback {
    T *cb_;
    explicit WrappingCallback(const T *cb)
        : cb_(cb) {}
    int call(int x) const override { return cb_->call(x); }
};

// exitential type.
struct Callback {
    std::unique_ptr<ICallback> ptr_;

    template <typename T> Callback(T t) {
        ptr_ = std::make_unique<WrappingCallback<T>>(std::move(t));
    }
    int operator()(int x) const { return ptr_->call(x); }
};

int run_twice(const Callback &callback) { return callback(1) + callback(2); }

int main(void) {
    int x = run_twice(Sub1{});
    int y = run_twice(Add1{});
    return 0;
}
