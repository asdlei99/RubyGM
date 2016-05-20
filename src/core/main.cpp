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
#include <core/drawable/rgmBitmap.h>
#include <core/drawable/rgmTextlayout.h>
#include <game/rgmGame.h>
#include <core/drawable/rgmGlyph.h>


// RubyGM namespace
namespace RubyGM {
    // asset
    // game scene
    struct IGMScene {
        // update
        virtual void Update() noexcept = 0;
        // ctor
        virtual ~IGMScene() {}
    };
    // create secen
    auto CreateScene()->IGMScene*;
    // yield
    void FiberYield();
    // update fiber
    void impl_update_fiber() {
        Drawable::Default def;
        if (0) {
            auto sprite = Game::NewSprite(RubyGM::DEFAULT_STATUS);
            sprite->SetX(128.f * 2.f);
            sprite->SetY(128.f);
            sprite->SetZoomX(5.f);
            sprite->SetZoomY(5.f);
            sprite->SetOX(40.f);
            sprite->SetOY(20.f);
            auto sprite1 = sprite->AddChild(RubyGM::DEFAULT_STATUS);
            auto sprite2 = sprite->AddChild(RubyGM::DEFAULT_STATUS);
            RubyGM::FiberYield();
            sprite2->SetY(20.f);
            {
                Drawable::TextStatus ts(
                    std::move(Game::GetFontAsset(0)),
                    def
                );
                ts.text = L"Hello, 世界!";
                ts.textlen = static_cast<uint32_t>(std::wcslen(ts.text));
                if (auto sp = Drawable::Textlayout::CreateSP(ts)) {
                    TextMetrics ms;
                    sp->GetMetrics(ms);
                    sp->SetFontSize(TextRange{ 0, 4 }, 5.f);
                    sp->SetFontColor(TextRange{ 0, 5 }, ColorF{ 1.f,0.,0.f,1.f });
                    sp->SetFontFeature(TextRange{ 1, 4 }, "smcp");
                    sprite1->SetDrawable(sp);
                    {
                        auto sf = sprite2->ComputeScaleFactorEz1();
                        auto bsp = sp->RasterizationSP(sf);
                        sprite2->SetDrawable(bsp);
                        sp->SetFontColor(TextRange{ 3, 4 }, ColorF{ 1.f,0.,0.8f,1.f });
                    }
                }
            }
            while (true) {
                RubyGM::FiberYield();
                auto time = Game::GetDetalTime();
                sprite->SetSkewX(sprite->GetSkewX() + time*30.f);
            }
        }





        //main_call();
        auto mod = [](float x, float y) { return x - float(int(x / y)); };
        auto sprite = Game::NewSprite(RubyGM::DEFAULT_STATUS);
        sprite->SetX(300.f);
        sprite->SetY(300.f);
        sprite->SetZoomX(2.f);
        //sprite->SetClipRect(RubyGM::RectF{ 0.f, 0.f, 320.f, 320.f });
        //sprite->SetStrictClip(true);
        //sprite->SetZoomY(2.f);
        //sprite->SetOX(50.f);
        auto sprite1 = sprite->AddChild(DEFAULT_STATUS);
        auto sprite2 = sprite->AddChild(DEFAULT_STATUS);
        sprite2->SetOX(10.f);
        sprite2->SetOY(20.f);
        {
            Drawable::TextStatus ts(
                std::move(Game::GetFontAsset(0)),
                def
            );
            ts.color.b = 1.f;
            ts.width = 64.f;
            ts.text = L"Hello, 世界!";
            ts.textlen = (std::wcslen(ts.text));
            auto layout = Drawable::Textlayout::CreateSP(ts);
            if (layout) {
                TextMetrics ms;
                layout->GetMetrics(ms);
                layout->SetFontSize(TextRange{ 0, 4 }, 5.f);
                layout->SetFontColor(TextRange{ 0, 5 }, ColorF{ 1.f,0.,0.f,1.f });
                layout->SetFontFeature(TextRange{ 1, 4 }, "smcp");
                //sprite1->SetDrawable(std::move(sp));
            }
            {
                auto&ast = Game::CreateBitmapAssetFromFile(L"asset/FC4-screenshot.jpg");
                Drawable::BitmapStatus bs(std::move(ast), def);
                auto sp = Drawable::Bitmap::CreateSP(bs);
                if (sp) {
                    sp->des_rect.right = 128.f;
                    auto bilibili = sp->GetWidth() * sp->GetHeight();
                    sp->des_rect.bottom = sp->des_rect.right / bilibili;
                }
            }
            {
                Drawable::GlyphStatus bs(def);
                bs.layout = layout.Ptr();
                auto sp = Drawable::Glyph::CreateSP(bs);
                sprite->SetDrawable(std::move(sp));
            }
        }
        float v = -1.f;
        float s = .01f;
        sprite->SetRotation(30.f);
        while (true) {
            RubyGM::FiberYield();
            auto time = Game::GetDetalTime() * 0.5f;
            //sprite->SetRotation(sprite->GetRotation() + time * 60.f);
            sprite1->SetSkewX(sprite1->GetSkewX() + time*60.f);
            //sprite2->SetSkewX(-sprite1->GetSkewX());
            v += s * time * 5.f;
            if (v >= 4.f) s = -1.f;
            else if (v <= 1.f) s = 1.f;
            sprite->SetZoomX(v);
            sprite->SetZoomY(v);
        }
    }
    // update fiber
    void UpdateFiber() noexcept {
        try { impl_update_fiber(); }
        catch (...) {}
    }
}


auto RubyGM::CreateScene() -> IGMScene * {
    return nullptr;
}