#pragma once
/**
* Copyright (c) 2015-2015 dustpg   mailto:dustpg@gmail.com
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __cplusplus
#error "C++ Only"
#endif
// C
#include <cassert>

// mruby
#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/proc.h"
#include "mruby/variable.h"
#include "mruby/string.h"
// C++
#include <tuple>

// rubygm::helper namespace
namespace RubyGM { namespace Helper {
    // to match ruby-style, use low-case char
    // type helper with c++ tuple
    template <typename T> struct type_helper : public type_helper<decltype(&T::operator())> {};
    // type helper
    template <typename ClassType, typename ReturnType, typename... Args>
    struct type_helper<ReturnType(ClassType::*)(Args...) const> {
        // number of arguments
        enum : size_t { arity = sizeof...(Args) };
        // return type
        using result_type = ReturnType;
        // arg type
        template <size_t i> struct arg { using type = typename std::tuple_element<i, std::tuple<Args...>>::type; };
    };
    // call c++ function
    template<size_t ArgNum> struct call_helper {
        // call
        template<typename TypeHelper, typename T, typename RubyArgType, typename... Args>
        static auto call(T lam, RubyArgType* list, Args... args) noexcept { 
            constexpr size_t NEXT = ArgNum - 1;
            constexpr size_t INDEX = TypeHelper::arity - ArgNum;
            return call_helper<NEXT>::call<TypeHelper>(
                lam, list, args..., ruby_arg<TypeHelper::arg<INDEX>::type>::get(list[INDEX])
                );
        }
    };
    // base arg
    template<> struct call_helper<0> { 
        // call
        template<typename TypeHelper, typename T, typename RubyArgType, typename... Args>
        static auto call(T lam, RubyArgType*, Args...args) noexcept { return lam(args...); }
    };
    // type helper for pointer type to obj type
    template<typename T> struct type_helper_ptr { using type = T; };
    // type helper for pointer type to obj type
    template<typename T> struct type_helper_ptr<T*> { using type = typename type_helper_ptr<T>::type; };
    // ruby binder
    auto ruby_binder() noexcept { assert(!"bad overload"); return 0ui32; };
    // ruby arg to c++
    template<typename T> struct ruby_arg { };
    // ruby arg to c++: for void
    template<> struct ruby_arg<void> {
        // set mruby
        template<typename ...Args>
        static auto set(mrb_state* , Args...) noexcept { return mrb_nil_value(); }
        // get mruby
        static auto get(const mrb_value& v) noexcept { return ; }
    };
    // ruby arg to c++: for bool
    template<> struct ruby_arg<bool> {
        // set mruby
        static auto set(mrb_state* , bool b) noexcept { return mrb_bool_value(b); }
        // get mruby
        static auto get(const mrb_value& v) noexcept { return mrb_test(v); }
    };
    // mruby arg to c++: for float
    template<> struct ruby_arg<mrb_float> {
        // set mruby
        static auto set(mrb_state *mrb, mrb_float f) noexcept { return mrb_float_value(mrb, f); }
        // get
        static auto get(const mrb_value& v) noexcept { 
            return mrb_float_p(v) ? static_cast<mrb_float>(mrb_float(v)) : static_cast<mrb_float>(mrb_fixnum(v)); 
        }
    };
    // mruby arg to c++: for int32_t
    template<> struct ruby_arg<int32_t> {
        // get
        static auto get(const mrb_value& v) noexcept { 
            return mrb_fixnum_p(v) ?  static_cast<int32_t>(mrb_fixnum(v)) : static_cast<int32_t>(mrb_float(v)); 
        }
    };
    // mruby arg to c++: for const char*
    template<> struct ruby_arg<const char*> {
        // get
        static auto get(const mrb_value& v) noexcept { return RSTRING_PTR(v); }
    };
    // mruby arg to c++: for void*
    template<> struct ruby_arg<void*> {
        // get
        static auto get(const mrb_value& v) noexcept { return mrb_cptr(v); }
    };
    // -----------------------
    // ADD YOUR OWN TYPE HERE
    // -----------------------
    // mruby binder
    class mruby_binder {
    public:
        // class binder
        template<typename CppClass>
        class class_binder {
            // method helper
            template<size_t ArgNum> struct method_helper {
                // bind
                template<typename T> 
                static inline auto bind(const class_binder<CppClass>& binder, const char* method_name, T method) noexcept {
                    return method_helper_ex<type_helper<T>::arg<0>::type>::bind(binder, const char* method_name, T method);
                }
            };
            // method helper for zero arg
            template<> struct method_helper<0> {
                // bind
                template<typename T> 
                static inline auto bind(const class_binder<CppClass>& binder, const char* method_name, T method) noexcept {
                    // no-arg -> [rb]class-method call [cpp]static-class-function
                    // real ctor lambda to avoid capture
                    static const auto real_method(method);
                    // define
                    ::mrb_define_class_method(binder.get_mruby(), binder.get_class(), method_name, [](mrb_state* mrb, mrb_value self) noexcept {
                        return ruby_arg<type_helper<T>::result_type>::set(mrb, real_method());
                    }, MRB_ARGS_NONE());
                }
            };
            /*// method helper ex -- obj-ref
            template<typename T> inline auto method_helper_ex<CppClass&>(const char* method_name,T method) noexcept {
                using traits = type_helper<T>;
                // real ctor lambda to avoid capture
                static const auto real_method(method);
                ::mrb_define_method(mstate, cla, method_name, [](mrb_state* mrb, mrb_value self) noexcept {
                    auto& obj = reinterpret_cast<CppClass*>(DATA_PTR(self));
                    mrb_value* args; int narg;
                    mrb_get_args(mrb, "*", &args, &narg);
                    return ruby_arg<type_helper<T>::result_type>::set(mrb, call_helper<traits::arity>::call<traits>(real_method, args));
                }, MRB_ARGS_REQ(traits::arity));
            }*/
        public:
            // get class
            auto get_class() const noexcept { return classr; }
            // get mruby
            auto get_mruby() const noexcept { return mstate; }
            // ctor
            class_binder(mrb_state* s, RClass* c) noexcept : mstate(s), classr(c) { assert(mstate && classr && "bad arguments"); };
            // copy ctor
            class_binder(const class_binder<CppClass>& b) noexcept : mstate(b.mstate), classr(b.classr) { assert(mstate && classr && "bad arguments"); };
            // bind
            template<typename T> auto bind(const char* method_name, T method) {
#ifdef _DEBUG
                static bool isfirst = true;
                assert(isfirst && "cannot call twice with same template");
                isfirst = false;
#endif
                // helper
                using traits = type_helper<T>;
                method_helper<traits::arity>::bind(*this, method_name, method);
            }
        private:
            // state of mruby
            mrb_state*      mstate = nullptr;
            // class ruby
            RClass*         classr = nullptr;
        };
    public:
        // ctor
        mruby_binder(mrb_state* state) noexcept : mstate(state) { assert(mstate && "bad argument"); };
        // copy ctor
        mruby_binder(const mruby_binder& b) noexcept : mstate(b.mstate) { assert(mstate && "bad argument"); };
        // bind class
        template<typename T> inline auto bind_class(const char* class_name, T ctor) noexcept {
            return this->bind_class(class_name, ctor, mstate->kernel_module);
        }
        // bind class with outer
        template<typename T> inline auto bind_class(const char* class_name, T ctor, RClass* outer) noexcept {
#ifdef _DEBUG
            static bool isfirst = true;
            assert(isfirst && "cannot call twice with same template");
            isfirst = false;
#endif
            // define class
            auto cla = ::mrb_define_class_under(mstate, outer, class_name, mstate->object_class);
            assert(cla && "error from mruby or bad action");
            MRB_SET_INSTANCE_TT(cla, MRB_TT_DATA);
            using traits = type_helper<T>;
            using class_type = type_helper_ptr<traits::result_type>::type;
            // data type
            static mrb_data_type this_data_type = {
                class_name,
                [](mrb_state *mrb, void *ptr) { (void)mrb; delete static_cast<class_type*>(ptr); }
            };
            // real ctor lambda to avoid capture
            static const auto real_ctor(ctor);
            // define initialize method
            auto initialize_this = [](mrb_state *mrb, mrb_value self) noexcept {
                DATA_TYPE(self) = &this_data_type;
                mrb_value* args; int narg;
                mrb_get_args(mrb, "*", &args, &narg);
                //assert(narg == traits::arity && "bad arguments");
                DATA_PTR(self) = call_helper<traits::arity>::call<traits>(real_ctor, args);
                return self;
            };
            ::mrb_define_method(mstate, cla, "initialize", initialize_this, MRB_ARGS_REQ(traits::arity));
            return class_binder<class_type>(mstate, cla);
        }
    private:
        // state of mruby
        mrb_state*              mstate = nullptr;
    };
    // overload for mruby binder
    auto ruby_binder(mrb_state* data) noexcept { return mruby_binder(data); }
}}