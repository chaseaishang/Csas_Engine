//
// Created by chaseaishang on 23-4-16.
//

#pragma once

// From glfw3.h
//GLFW_MOUSE_BUTTON_LEFT
namespace CsasEngine
{
    typedef enum class KeyCode : uint16_t
    {
        // From glfw3.h
        Space               = 32,
        Apostrophe          = 39, /* ' */
        Comma               = 44, /* , */
        Minus               = 45, /* - */
        Period              = 46, /* . */
        Slash               = 47, /* / */

        D0                  = 48, /* 0 */
        D1                  = 49, /* 1 */
        D2                  = 50, /* 2 */
        D3                  = 51, /* 3 */
        D4                  = 52, /* 4 */
        D5                  = 53, /* 5 */
        D6                  = 54, /* 6 */
        D7                  = 55, /* 7 */
        D8                  = 56, /* 8 */
        D9                  = 57, /* 9 */

        Semicolon           = 59, /* ; */
        Equal               = 61, /* = */

        A                   = 65,
        B                   = 66,
        C                   = 67,
        D                   = 68,
        E                   = 69,
        F                   = 70,
        G                   = 71,
        H                   = 72,
        I                   = 73,
        J                   = 74,
        K                   = 75,
        L                   = 76,
        M                   = 77,
        N                   = 78,
        O                   = 79,
        P                   = 80,
        Q                   = 81,
        R                   = 82,
        S                   = 83,
        T                   = 84,
        U                   = 85,
        V                   = 86,
        W                   = 87,
        X                   = 88,
        Y                   = 89,
        Z                   = 90,

        LeftBracket         = 91,  /* [ */
        Backslash           = 92,  /* \ */
        RightBracket        = 93,  /* ] */
        GraveAccent         = 96,  /* ` */

        World1              = 161, /* non-US #1 */
        World2              = 162, /* non-US #2 */

        /* Function keys */
        Escape              = 256,
        Enter               = 257,
        Tab                 = 258,
        Backspace           = 259,
        Insert              = 260,
        Delete              = 261,
        Right               = 262,
        Left                = 263,
        Down                = 264,
        Up                  = 265,
        PageUp              = 266,
        PageDown            = 267,
        Home                = 268,
        End                 = 269,
        CapsLock            = 280,
        ScrollLock          = 281,
        NumLock             = 282,
        PrintScreen         = 283,
        Pause               = 284,
        F1                  = 290,
        F2                  = 291,
        F3                  = 292,
        F4                  = 293,
        F5                  = 294,
        F6                  = 295,
        F7                  = 296,
        F8                  = 297,
        F9                  = 298,
        F10                 = 299,
        F11                 = 300,
        F12                 = 301,
        F13                 = 302,
        F14                 = 303,
        F15                 = 304,
        F16                 = 305,
        F17                 = 306,
        F18                 = 307,
        F19                 = 308,
        F20                 = 309,
        F21                 = 310,
        F22                 = 311,
        F23                 = 312,
        F24                 = 313,
        F25                 = 314,

        /* Keypad */
        KP0                 = 320,
        KP1                 = 321,
        KP2                 = 322,
        KP3                 = 323,
        KP4                 = 324,
        KP5                 = 325,
        KP6                 = 326,
        KP7                 = 327,
        KP8                 = 328,
        KP9                 = 329,
        KPDecimal           = 330,
        KPDivide            = 331,
        KPMultiply          = 332,
        KPSubtract          = 333,
        KPAdd               = 334,
        KPEnter             = 335,
        KPEqual             = 336,

        LeftShift           = 340,
        LeftControl         = 341,
        LeftAlt             = 342,
        LeftSuper           = 343,
        RightShift          = 344,
        RightControl        = 345,
        RightAlt            = 346,
        RightSuper          = 347,
        Menu                = 348
    } Key;

    inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
    {
        os << static_cast<int32_t>(keyCode);
        return os;
    }
}

// From glfw3.h
#define CSAS_KEY_SPACE           ::CsasEngine::Key::Space
#define CSAS_KEY_APOSTROPHE      ::CsasEngine::Key::Apostrophe    /* ' */
#define CSAS_KEY_COMMA           ::CsasEngine::Key::Comma         /* , */
#define CSAS_KEY_MINUS           ::CsasEngine::Key::Minus         /* - */
#define CSAS_KEY_PERIOD          ::CsasEngine::Key::Period        /* . */
#define CSAS_KEY_SLASH           ::CsasEngine::Key::Slash         /* / */
#define CSAS_KEY_0               ::CsasEngine::Key::D0
#define CSAS_KEY_1               ::CsasEngine::Key::D1
#define CSAS_KEY_2               ::CsasEngine::Key::D2
#define CSAS_KEY_3               ::CsasEngine::Key::D3
#define CSAS_KEY_4               ::CsasEngine::Key::D4
#define CSAS_KEY_5               ::CsasEngine::Key::D5
#define CSAS_KEY_6               ::CsasEngine::Key::D6
#define CSAS_KEY_7               ::CsasEngine::Key::D7
#define CSAS_KEY_8               ::CsasEngine::Key::D8
#define CSAS_KEY_9               ::CsasEngine::Key::D9
#define CSAS_KEY_SEMICOLON       ::CsasEngine::Key::Semicolon     /* ; */
#define CSAS_KEY_EQUAL           ::CsasEngine::Key::Equal         /* = */
#define CSAS_KEY_A               ::CsasEngine::Key::A
#define CSAS_KEY_B               ::CsasEngine::Key::B
#define CSAS_KEY_C               ::CsasEngine::Key::C
#define CSAS_KEY_D               ::CsasEngine::Key::D
#define CSAS_KEY_E               ::CsasEngine::Key::E
#define CSAS_KEY_F               ::CsasEngine::Key::F
#define CSAS_KEY_G               ::CsasEngine::Key::G
#define CSAS_KEY_H               ::CsasEngine::Key::H
#define CSAS_KEY_I               ::CsasEngine::Key::I
#define CSAS_KEY_J               ::CsasEngine::Key::J
#define CSAS_KEY_K               ::CsasEngine::Key::K
#define CSAS_KEY_L               ::CsasEngine::Key::L
#define CSAS_KEY_M               ::CsasEngine::Key::M
#define CSAS_KEY_N               ::CsasEngine::Key::N
#define CSAS_KEY_O               ::CsasEngine::Key::O
#define CSAS_KEY_P               ::CsasEngine::Key::P
#define CSAS_KEY_Q               ::CsasEngine::Key::Q
#define CSAS_KEY_R               ::CsasEngine::Key::R
#define CSAS_KEY_S               ::CsasEngine::Key::S
#define CSAS_KEY_T               ::CsasEngine::Key::T
#define CSAS_KEY_U               ::CsasEngine::Key::U
#define CSAS_KEY_V               ::CsasEngine::Key::V
#define CSAS_KEY_W               ::CsasEngine::Key::W
#define CSAS_KEY_X               ::CsasEngine::Key::X
#define CSAS_KEY_Y               ::CsasEngine::Key::Y
#define CSAS_KEY_Z               ::CsasEngine::Key::Z
#define CSAS_KEY_LEFT_BRACKET    ::CsasEngine::Key::LeftBracket   /* [ */
#define CSAS_KEY_BACKSLASH       ::CsasEngine::Key::Backslash     /* \ */
#define CSAS_KEY_RIGHT_BRACKET   ::CsasEngine::Key::RightBracket  /* ] */
#define CSAS_KEY_GRAVE_ACCENT    ::CsasEngine::Key::GraveAccent   /* ` */
#define CSAS_KEY_WORLD_1         ::CsasEngine::Key::World1        /* non-US #1 */
#define CSAS_KEY_WORLD_2         ::CsasEngine::Key::World2        /* non-US #2 */

/* Function keys */
#define CSAS_KEY_ESCAPE          ::CsasEngine::Key::Escape
#define CSAS_KEY_ENTER           ::CsasEngine::Key::Enter
#define CSAS_KEY_TAB             ::CsasEngine::Key::Tab
#define CSAS_KEY_BACKSPACE       ::CsasEngine::Key::Backspace
#define CSAS_KEY_INSERT          ::CsasEngine::Key::Insert
#define CSAS_KEY_DELETE          ::CsasEngine::Key::Delete
#define CSAS_KEY_RIGHT           ::CsasEngine::Key::Right
#define CSAS_KEY_LEFT            ::CsasEngine::Key::Left
#define CSAS_KEY_DOWN            ::CsasEngine::Key::Down
#define CSAS_KEY_UP              ::CsasEngine::Key::Up
#define CSAS_KEY_PAGE_UP         ::CsasEngine::Key::PageUp
#define CSAS_KEY_PAGE_DOWN       ::CsasEngine::Key::PageDown
#define CSAS_KEY_HOME            ::CsasEngine::Key::Home
#define CSAS_KEY_END             ::CsasEngine::Key::End
#define CSAS_KEY_CAPS_LOCK       ::CsasEngine::Key::CapsLock
#define CSAS_KEY_SCROLL_LOCK     ::CsasEngine::Key::ScrollLock
#define CSAS_KEY_NUM_LOCK        ::CsasEngine::Key::NumLock
#define CSAS_KEY_PRINT_SCREEN    ::CsasEngine::Key::PrintScreen
#define CSAS_KEY_PAUSE           ::CsasEngine::Key::Pause
#define CSAS_KEY_F1              ::CsasEngine::Key::F1
#define CSAS_KEY_F2              ::CsasEngine::Key::F2
#define CSAS_KEY_F3              ::CsasEngine::Key::F3
#define CSAS_KEY_F4              ::CsasEngine::Key::F4
#define CSAS_KEY_F5              ::CsasEngine::Key::F5
#define CSAS_KEY_F6              ::CsasEngine::Key::F6
#define CSAS_KEY_F7              ::CsasEngine::Key::F7
#define CSAS_KEY_F8              ::CsasEngine::Key::F8
#define CSAS_KEY_F9              ::CsasEngine::Key::F9
#define CSAS_KEY_F10             ::CsasEngine::Key::F10
#define CSAS_KEY_F11             ::CsasEngine::Key::F11
#define CSAS_KEY_F12             ::CsasEngine::Key::F12
#define CSAS_KEY_F13             ::CsasEngine::Key::F13
#define CSAS_KEY_F14             ::CsasEngine::Key::F14
#define CSAS_KEY_F15             ::CsasEngine::Key::F15
#define CSAS_KEY_F16             ::CsasEngine::Key::F16
#define CSAS_KEY_F17             ::CsasEngine::Key::F17
#define CSAS_KEY_F18             ::CsasEngine::Key::F18
#define CSAS_KEY_F19             ::CsasEngine::Key::F19
#define CSAS_KEY_F20             ::CsasEngine::Key::F20
#define CSAS_KEY_F21             ::CsasEngine::Key::F21
#define CSAS_KEY_F22             ::CsasEngine::Key::F22
#define CSAS_KEY_F23             ::CsasEngine::Key::F23
#define CSAS_KEY_F24             ::CsasEngine::Key::F24
#define CSAS_KEY_F25             ::CsasEngine::Key::F25

/* Keypad */
#define CSAS_KEY_KP_0            ::CsasEngine::Key::KP0
#define CSAS_KEY_KP_1            ::CsasEngine::Key::KP1
#define CSAS_KEY_KP_2            ::CsasEngine::Key::KP2
#define CSAS_KEY_KP_3            ::CsasEngine::Key::KP3
#define CSAS_KEY_KP_4            ::CsasEngine::Key::KP4
#define CSAS_KEY_KP_5            ::CsasEngine::Key::KP5
#define CSAS_KEY_KP_6            ::CsasEngine::Key::KP6
#define CSAS_KEY_KP_7            ::CsasEngine::Key::KP7
#define CSAS_KEY_KP_8            ::CsasEngine::Key::KP8
#define CSAS_KEY_KP_9            ::CsasEngine::Key::KP9
#define CSAS_KEY_KP_DECIMAL      ::CsasEngine::Key::KPDecimal
#define CSAS_KEY_KP_DIVIDE       ::CsasEngine::Key::KPDivide
#define CSAS_KEY_KP_MULTIPLY     ::CsasEngine::Key::KPMultiply
#define CSAS_KEY_KP_SUBTRACT     ::CsasEngine::Key::KPSubtract
#define CSAS_KEY_KP_ADD          ::CsasEngine::Key::KPAdd
#define CSAS_KEY_KP_ENTER        ::CsasEngine::Key::KPEnter
#define CSAS_KEY_KP_EQUAL        ::CsasEngine::Key::KPEqual

#define CSAS_KEY_LEFT_SHIFT      ::CsasEngine::Key::LeftShift
#define CSAS_KEY_LEFT_CONTROL    ::CsasEngine::Key::LeftControl
#define CSAS_KEY_LEFT_ALT        ::CsasEngine::Key::LeftAlt
#define CSAS_KEY_LEFT_SUPER      ::CsasEngine::Key::LeftSuper
#define CSAS_KEY_RIGHT_SHIFT     ::CsasEngine::Key::RightShift
#define CSAS_KEY_RIGHT_CONTROL   ::CsasEngine::Key::RightControl
#define CSAS_KEY_RIGHT_ALT       ::CsasEngine::Key::RightAlt
#define CSAS_KEY_RIGHT_SUPER     ::CsasEngine::Key::RightSuper
#define CSAS_KEY_MENU            ::CsasEngine::Key::Menu


