#include <core/rubygm.h>
#include <mruby/compile.h>

class Foo {
public:
    Foo(int a) :data(size_t(a)) { std::printf("[%s] - %d\r\n", __FUNCTION__, a); }
    ~Foo() { std::printf("[%s]\r\n", __FUNCTION__); }
public:
    // bar
    auto bar(int a , int b, int c) {
        printf("[%s]%d ? %d\r\n", __FUNCTION__, a, a*b + c); return 0.f;
    }// bar
    static auto baz(int a , int b , int c) {
        printf("[%s]%d ? %d\r\n", __FUNCTION__, a, a*b + c); return 9.f;
    }
private:
    // data
    size_t data = 0;
};


class Foo2 {
public:
    Foo2() { std::printf("[%s]\r\n", __FUNCTION__); }
    ~Foo2() { std::printf("[%s]\r\n", __FUNCTION__); }
private:
    // data
    size_t data = 0;
};




// ruby script
static const char* const rbscript = u8R"rb(
puts "Hello, World!"
a = 0
a = Foo.new 0, 1
Foo.baa
a = Foo2.new "sad", 120, 0.0, 0, 1, 3.1416
)rb";

// main
int main(int argc, char* argv[]) {
    auto mruby = ::mrb_open();
    if (mruby) {
        auto binder = RubyGM::Helper::ruby_binder(mruby);
        {
            auto classbinder = binder.bind_class("Foo", [](int a, int b) {
                auto c = a == b;
                c = false;
                return new(std::nothrow) Foo(a);
            });
            // first argument is binded-class pointer/reference -> instance-method
            //classbinder.bind("bar", [](Foo& obj, int a , int b, int c) noexcept { return obj.bar(a, b, c); });
            // LAST argument not binded-class pointer -> class-method
            //classbinder.bind("baz", [](int b) noexcept { return Foo::baz(b, 5, 7); });
            // limited lambda working(do not capture object because of static lambda)
            classbinder.bind("baa", [=]() noexcept { return Foo::baz(argc, 5, 7); });
        }
        {
            auto classbinder = binder.bind_class("Foo2", [](const char* v, int32_t a, float b, int32_t c, int32_t d, float f) {
                return new(std::nothrow) Foo2();
            });
        }
        ::mrb_load_string(mruby, rbscript);
        ::mrb_close(mruby);
        mruby = nullptr;
    }
    std::getchar();
    return 0;
}