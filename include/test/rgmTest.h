#pragma once
/**
* Copyright (c) 2015-2016 dustpg   mailto:dustpg@gmail.com
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

#include <cstdint>

// rubygm namespace
namespace RubyGM { 
#ifdef RUBYGM_TEST_MODE
    // test class
    class CGMTestModule {
    public:
        // ctor
        CGMTestModule() noexcept {}
        // dtor
        ~CGMTestModule() noexcept;
        // init
        void Init_test(const wchar_t* name, int id, void* ptr) noexcept;
        // set name
        void SetName_test(const wchar_t*) noexcept;
        // set name from utf-8
        void SetName_test(const char*) noexcept;
        // get name
        auto GetName_test() const noexcept -> const wchar_t*;
    private:
        // check name
        bool check_name__test() noexcept;
    private:
        // test name
        void*           m_pName_test = nullptr;
    };
#else
    // test class
    class CGMTestModule {
    public:
        // ctor
        CGMTestModule() noexcept = default;
        // dtor
        ~CGMTestModule() noexcept = default;
    };
#endif
}

