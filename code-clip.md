``` cpp
/// <summary>
/// Runs this instance.
/// </summary>
/// <returns></returns>
void RubyGM::CGMFramework::Run() noexcept {
    ::ShowWindow(m_hwnd, SW_SHOW);
    auto update_fiber = [](void*) { RubyGM::UpdateFiber(); };
    auto fiber0 = ::ConvertThreadToFiberEx(nullptr, FIBER_FLAG_FLOAT_SWITCH);
    auto fiber1 = ::CreateFiberEx(0, 0, FIBER_FLAG_FLOAT_SWITCH, update_fiber, fiber0);
    assert(m_hVBlank);
    MSG msg;
    // 循环
    while (true) {
        // 处理事件
        switch (::MsgWaitForMultipleObjects(1, &m_hVBlank, FALSE, INFINITE, QS_ALLINPUT))
        {
        case WAIT_OBJECT_0:
            // 渲染对象
            ::SwitchToFiber(fiber1);
            this->Render();
            break;
        case WAIT_OBJECT_0 + 1:
            // 处理消息
            while (::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                // 退出?
                if (msg.message == WM_QUIT) goto force_break;
                ::TranslateMessage(&msg);
                ::DispatchMessageW(&msg);
            }
            break;
        default:
            goto force_break;
        }
    }
    // 收尾处理
force_break:
    ::DeleteFiber(fiber1);
}

```
