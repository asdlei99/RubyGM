#pragma warning(disable: 4290)
#pragma warning(disable: 4200)

#include <core/rubygm.h>
#include <core/rgmmgr.h>
#include <mruby/compile.h>
#include "../../../BindER/bindenvruby.h"

class Foo {
public:
    Foo(int a) :data(size_t(a)) { std::printf("[%s] - %d\r\n", __FUNCTION__, a); }
    ~Foo() { std::printf("[%s]\r\n", __FUNCTION__); }
public:
    // bar
    auto bar(int a , int b, int c) const {
        printf("[%s]%d ? %d\r\n", __FUNCTION__, a, a*b + c); return 0.f;
    }// bar
    static auto baz(int a , int b , int c) {
        printf("[%s]%d ? %d\r\n", __FUNCTION__, a, a*b + c); return 987;
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
p Fiber.class
a = 0
a = Foo.new 0, 1
p Foo.baa.class
p Foo.baz(9).class
p a.bar 1,2,3
a = Foo2.new "sad", 120, 0.0, 0, 1, 3.1416
puts ""
puts ""
rubygm_main {
  p 987123
}
)rb";

int test() noexcept;

// call binder
auto binder_mruby(mrb_state* mruby) {
    int random_data = 0;
    auto binder = BindER::ruby_binder(mruby);
    {
        // ctor
        auto classbinder = binder.bind_class("Foo", [](int a, int b) {
            return new(std::nothrow) Foo(a+b);
        });
        // first argument is binded-class pointer/reference -> instance-method
        classbinder.bind("bar", [](Foo& obj, int a, int b, int c) noexcept { return obj.bar(a, b, c); });
        // first argument not binded-class pointer -> class-method
        classbinder.bind("baz", [](int b) noexcept { return Foo::baz(b, 5, 7); });
        // limited lambda working(do not capture value-passed-object because of static lambda)
        classbinder.bind("baa", [&]() noexcept { return Foo::baz(random_data, 5, 7); });
    }
    {
        auto classbinder = binder.bind_class("Foo2", [](const char* , int32_t , float , int32_t , int32_t , float ) {
            return new(std::nothrow) Foo2();
        });
    }
}


// main
int main(int argc, char* argv[]) {
    // 构造单例
    RubyGM::CGMSingleton<RubyGM::CGMManager>::s_instance.Create();
    // 初始化
    if (SUCCEEDED(GMManager.Initialize())) {
        test();
        //
        GMManager.mruby = ::mrb_open();
        auto& mruby = GMManager.mruby;
        if (mruby) {
            binder_mruby(mruby);
            ::mrb_load_string(mruby, rbscript);
            ::mrb_close(mruby);
            mruby = nullptr;
        }
        std::getchar();
        // 反初始化
        GMManager.Uninitialize();
    }
    // 析构单例
    RubyGM::CGMSingleton<RubyGM::CGMManager>::s_instance.Destory();
    return 0;
}
