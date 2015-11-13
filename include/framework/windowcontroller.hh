#pragma once

#include <map>
#include <list>

#include "framework/applicationcontext.hh"
#include "framework/keystate.hh"
#include "framework/readingkeyboardstate.hh"
#include "framework/readingmousestate.hh"
#include "framework/readingwindowstate.hh"
#include "framework/iwindowcontroller.hh"
#include "system/keycode.hh"
#include "system/window.hh"
#include "system/openglcontext.hh"
#include "system/mutex.hh"

namespace Framework
{
    class WindowController : public IWindowController
    {
    private:
        class KeyboardState : public ReadingKeyboardState, public IWritingKeyboardState
        {
        public:
            KeyboardState();
            virtual ~KeyboardState();

            virtual KeyState GetKeyState(System::KeyCode key);
            virtual void PressKey(System::KeyCode key);
            virtual void UnpressKey(System::KeyCode key);

        private:
            System::Mutex *m_mutex;
            std::map<System::KeyCode, KeyState> m_states;

            KeyboardState(const KeyboardState &o) = delete;
            KeyboardState(KeyboardState &&o) = delete;
        };

        class MouseState : public ReadingMouseState
        {
        public:
            MouseState();
            virtual ~MouseState();

            virtual int GetMouseX() const;
            virtual int GetMouseY() const;
            virtual int GetScrollDelta();
            virtual KeyState GetMouseButtonState(System::MouseButton mouseButton);

            virtual void MouseMove(int xPos, int yPos);
            virtual void MouseButtonDown(System::MouseButton mouseButton);
            virtual void MouseButtonUp(System::MouseButton mouseButton);
            virtual void MouseScroll(int scrollDelta);

        private:
            System::Mutex *m_mutex;

            int _xPos;
            int _yPos;
            int _scrollDelta;
            std::map<System::MouseButton, KeyState> m_states;
        };

        class WindowState : public ReadingWindowState
        {
        public:
            WindowState();
            virtual ~WindowState();

            void GetSize(unsigned int *width, unsigned int *height);
            void Resize(unsigned int width, unsigned int height);

        private:
            System::Mutex *m_mutex;
            unsigned int _width;
            unsigned int _height;
        };

    public:
        WindowController(ApplicationContext *applicationContext);

        virtual ~WindowController();

        void CreateClientWindow();
        void CreateContext();

        virtual void SwapBuffers();

        virtual void AddKeyboardEventHandler(IWritingKeyboardState *handler);
        virtual void RemoveKeyboardEventHandler(IWritingKeyboardState *handler);
        virtual ReadingKeyboardState *GetKeyStateReader();
        virtual ReadingMouseState *GetMouseReader();
        virtual ReadingWindowState *GetWindowReader();

        virtual void SetMousePosition(unsigned int posX, unsigned int posY);

        virtual void OnWindowReady();
        virtual void OnWindowClose();
        virtual void OnWindowResize(unsigned int width, unsigned int height);
        virtual void OnKeyDown(System::KeyCode key);
        virtual void OnKeyUp(System::KeyCode key);
        virtual void OnMouseMove(int xPos, int yPos);
        virtual void OnMouseButtonDown(System::MouseButton);
        virtual void OnMouseButtonUp(System::MouseButton);
        virtual void OnMouseScroll(int scrollDelta);

    private:
        System::Window *m_window;
        System::OpenGLContext *m_openGLContext;
        ApplicationContext *m_applicationContext;
        KeyboardState m_keyboardState;
        MouseState m_mouseState;
        WindowState m_windowState;
        std::list<IWritingKeyboardState*> m_keyboardEventHandlers;

        bool m_shouldDestroyWindow;
    };
}
