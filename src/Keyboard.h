#pragma once
#include <queue>
#include <bitset>
#include <optional>
class Keyboard{
    friend class Window;
public:
    class Event{
    public:
        enum class Type{
            kPress,
            kRelease
        };
    private:
        Type type;
        unsigned char code;
    public:
        Event(Type type, unsigned char code) noexcept: type(type), code(code){

        }
        bool IsPress() const noexcept{
            return type == Type::kPress;
        }
        bool IsRelease() const noexcept{
            return type == Type::kRelease;
        }
        unsigned char GetCode() const noexcept{
            return code;
        }
    };

public:
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard& ) = delete;

    bool KeyIsPressed(unsigned char keycode) const noexcept;
    std::optional<Event> ReadKey() noexcept;
    bool KeyIsEmpty() const noexcept;
    void FlushKey() noexcept;

    std::optional<char> ReadChar() noexcept;
    bool CharIsEmpty() const noexcept;
    void FlushChar() noexcept;
    void Flush() noexcept;

    void EnableAutorepeat() noexcept;
    void DisableAutorepeat() noexcept;
    bool AutorepeatIsEnabled() const noexcept;

private:
    void OnKeyPressed(unsigned char keycode) noexcept;
    void OnKeyReleased(unsigned char keycode) noexcept;
    void OnChar(char charactor) noexcept;
    void ClearState() noexcept;
    template<typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;
    bool autorepeatEnabled = false;
    std::bitset<nKeys> keystates; // 使用bit位保存每个key的状态
    std::queue<Event> keybuffer; // key message的事件队列
    std::queue<char> charbuffer; // char message的字符队列
};