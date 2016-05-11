#pragma warning(disable: 4290)
#pragma warning(disable: 4200)

#include <core/rubygm.h>
#include <mruby/compile.h>
#include "../../../BindER/bindenvruby.h"

class Foo2;

class Foo {
public:
    Foo(int a) :data(size_t(a)) {
        std::printf("[%s] - %d\r\n", __FUNCTION__, a);
    }
    ~Foo() { 
        std::printf("[%s] 0x%p\r\n", __FUNCTION__, this);
    }
public:
    // bar
    auto bar(int a , int b, int c) const {
        printf("[%s]%d ? %d\r\n", __FUNCTION__, a, a*b + c); return 0.f;
    }
    // bar
    static auto baz(int a , int b, int c) {
        printf("[%s]%d ? %d\r\n", __FUNCTION__, a, a*b + c); return 987;
    }
    // bar
    void set_foo(Foo2* f) {
        this->foo2 = f;
    }
private:
    // data
    size_t data = 0;
public:
    Foo2   *foo2 = nullptr;
};


class Foo2 {
public:
    Foo2(int s) : data(s) { 
        std::printf("[%s]\r\n", __FUNCTION__); 
    }
    ~Foo2() { std::printf("[%s]\r\n", __FUNCTION__); }
    Foo2(const Foo2&f) : data(f.data) {
    }
private:
    // data
    size_t data = 0;
};


namespace BindER {
    // mruby arg to c++: for Foo2*
    template<> struct ruby_arg<Foo2*> {
        // get
        static auto get(const mrb_value& v) noexcept { 
            return reinterpret_cast<Foo2*>(DATA_PTR(v));; 
        }
        // set mruby
        template<typename Lam>
        static auto set(mrb_state* ms, Lam lam, const mrb_value* org_arg) noexcept {
            lam();
#ifndef COPY_FOO2
            return mrb_nil_value();
#else
            auto ptr = lam();
            if (!ptr) return mrb_nil_value();
            auto new_ptr = new Foo2(*ptr);
            if (!new_ptr) return mrb_nil_value();
            auto data = BindER::mrb_data_object_alloc_helper<Foo2>(ms, new_ptr);
            return ::mrb_obj_value(data);
#endif
        }
    };
}


// ruby script
static const char* const rbscript = u8R"rb(
p "Hello, world!"

a = Foo.new 6565, 1
asd = Foo2.new("sad", 120, 0.0, 0, 1, 3.1416)
p asd, "sad"
p a.foo = asd


p 233
a = Foo.new 6565
p 2333

)rb";

// call binder
auto binder_mruby(mrb_state* mruby) {
    int random_data = 0;
    auto binder = BindER::ruby_binder(mruby);
    {
        // ctor
        auto foobinder = binder.bind_class("Foo", [](int a, int b) {
            return new(std::nothrow) Foo(a+b);
        });
        // first argument is binded-class pointer -> instance-method
        foobinder.bind("bar", [](Foo* obj, int a, int b, int c) noexcept { return obj->bar(a, b, c); });
        // first argument not binded-class pointer -> class-method
        foobinder.bind("baz", [](int b) noexcept { return Foo::baz(b, 5, 7); });
        // limited lambda working(do not capture value-passed-custom-type-object because of static lambda)
        foobinder.bind("baa", [&]() noexcept { return Foo::baz(random_data, 5, 7); });
        // return first parameter given
        foobinder.bind("foo=", [](Foo* obj, Foo2* f2) noexcept { 
            obj->set_foo(f2); return BindER::original_parameter<0>();
        });
    }
    {
        auto classbinder = binder.bind_class("Foo2", 
            [](const char* , int32_t b, float , int32_t , int32_t , float ) {
            return new(std::nothrow) Foo2(b);
        });
    }
}

int msgbox_error(const char*);

int msgbox_error(mrb_state* mrb) {
    ::mrb_print_backtrace(mrb);
    auto obj = ::mrb_funcall(mrb, ::mrb_obj_value(mrb->exc), "inspect", 0);
    return ::msgbox_error(RSTRING_PTR(obj));
}

// main
int main_call() {
    auto mruby = ::mrb_open();
    if (mruby) {
        binder_mruby(mruby);
        ::mrb_load_string(mruby, rbscript);
        ::msgbox_error(mruby);
        ::mrb_close(mruby);
        mruby = nullptr;
    }
    return 0;
}


#include <core/graphics/rgmSprite.h>
#include <core/drawable/rgmLine.h>
#include <core/drawable/rgmTextlayout.h>
#include <game/rgmGame.h>

// RubyGM namespace
namespace RubyGM {
    // yield
    void FiberYield();
    // update fiber
    void UpdateFiber() {
        //main_call();
        auto sprite = Game::NewSprite(RubyGM::DEFAULT_STATUS);
        sprite->SetX(300.f);
        sprite->SetY(300.f);
        sprite->SetZoomX(2.f);
        sprite->SetZoomY(2.f);
        sprite->SetOX(50.f);
        SprteStatus ss;
        sprite->Get(ss);
        auto sprite1 = Game::NewSprite(ss);
        {
            Drawable::Default def;
            Drawable::TextStatus ts(
                std::move(Game::GetFontAsset(0)),
                def
            );
            ts.color.b = 1.f;
            Drawable::LineStatus ls(def);
            ls.point1 = { 120.f, 30.f };
            ls.stroke_width = 5.f;
            ts.text = L"Hello, 世界!";
            ts.textlen = std::wcslen(ts.text);
            {
                auto sp = Drawable::Textlayout::CreateSP(ts);
                sprite1->SetDrawable(std::move(sp));
            }
            {
                auto sp = Drawable::Line::CreateSP(ls);
                sprite->SetDrawable(std::move(sp));
            }
        }
        float v = -1.f;
        float s = .01f;
        while (true) {
            RubyGM::FiberYield();
            auto time = Game::GetDetalTime();
            sprite->SetRotation(sprite->GetRotation() + time * 60.f);
            sprite1->SetRotation(sprite->GetRotation());
            v += s * time * 5.f;
            if (v >= 4.f) s = -1.f;
            else if (v <= 0.f) s = 1.f;
            sprite->SetZoomX(v);
            sprite->SetZoomY(v);
            sprite1->SetZoomX(v);
            sprite1->SetZoomY(v);
        }
    }
}

