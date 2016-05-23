#ifdef RUBYGM_TEST_MODE
#include <core/rubygm.h>
#include <test/rgmTest.h>
#include <Core/luistring.h>


// rubygm
namespace RubyGM {
    // string
    inline auto&String(void* ptr) noexcept {
        return *reinterpret_cast<LongUI::CUIString*>(ptr);
    }
    // string
    inline const auto&String(const void* ptr) noexcept {
        return *reinterpret_cast<const LongUI::CUIString*>(ptr);
    }
}


/// <summary>
/// Init_tests the specified name.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="id">The identifier.</param>
/// <param name="ptr">The PTR.</param>
/// <returns></returns>
void RubyGM::CGMTestModule::Init_test(
    const wchar_t* name, int id, void * ptr) noexcept {
    if (!m_pName_test) {
        m_pName_test = new(std::nothrow) LongUI::CUIString;
    }
    if (m_pName_test) {
        const auto fmt = L"%ls #%4d-0x%p";
        RubyGM::String(m_pName_test).Format(fmt, name, id, ptr);
    }
}

/// <summary>
/// Sets the name.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
void RubyGM::CGMTestModule::SetName_test(const wchar_t* name) noexcept {
    if (this->check_name__test()) {
        RubyGM::String(m_pName_test) = name;
    }
}

/// <summary>
/// Sets the name.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
void RubyGM::CGMTestModule::SetName_test(const char* name) noexcept {
    if (this->check_name__test()) {
        RubyGM::String(m_pName_test).FromUtf8(name);
    }
}

/// <summary>
/// Gets the name.
/// </summary>
/// <returns></returns>
auto RubyGM::CGMTestModule::GetName_test() const noexcept -> const wchar_t* {
    if (m_pName_test) return L"unnamed";
    return RubyGM::String(m_pName_test).c_str();
}

/// <summary>
/// Check_name__tests this instance.
/// </summary>
/// <returns></returns>
bool RubyGM::CGMTestModule::check_name__test() noexcept {
    if (m_pName_test) return true;
    static int test_id = 0;
    this->Init_test(L"unnamed", test_id, this);
    test_id++;
    return !!m_pName_test;
}

/// <summary>
/// Finalizes an instance of the <see cref="CGMTestModule"/> class.
/// </summary>
/// <returns></returns>
RubyGM::CGMTestModule::~CGMTestModule() noexcept {
    auto* ptr = &RubyGM::String(m_pName_test);
    if (ptr) delete ptr;
    m_pName_test = nullptr;
}





#endif