#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept{
    return keystates[keycode];
}
Keyboard::Event Keyboard::ReadKey() noexcept{
    if(keybuffer.size() > 0u){
        Keyboard::Event e = keybuffer.front();
        keybuffer.pop();
        return e;
    }else{
        return Keyboard::Event();
    }
}
bool Keyboard::KeyIsEmpty() const noexcept{
    return keybuffer.empty();
}
void Keyboard::FlushKey() noexcept{
    keybuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept{
    if(charbuffer.size() > 0u){
        unsigned char e = charbuffer.front();
        charbuffer.pop();
        return e;
    }else{
        return 0;
    }
}
bool Keyboard::CharIsEmpty() const noexcept{
    return charbuffer.empty();
}
void Keyboard::FlushChar() noexcept{
    charbuffer = std::queue<char>();
}
void Keyboard::Flush() noexcept{
    FlushKey();
    FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept{
    autorepeatEnabled = true;
}
void Keyboard::DisableAutorepeat() noexcept{
    autorepeatEnabled = false;
}
bool Keyboard::AutorepeatIsEnabled() const noexcept{
    return autorepeatEnabled;
}


void Keyboard::OnKeyPressed(unsigned char keycode) noexcept{
    keystates[keycode] = true;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Type::kPress,keycode));
    TrimBuffer(keybuffer);
}
void Keyboard::OnKeyReleased(unsigned char keycode) noexcept{
    keystates[keycode] = false;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Type::kRelease,keycode));
    TrimBuffer(keybuffer);
}
void Keyboard::OnChar(char charactor) noexcept{
    charbuffer.push(charactor);
    TrimBuffer(charbuffer);
}
void Keyboard::ClearState() noexcept{
    keystates.reset();
}
template<typename T>
static void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept{
    while(buffer.size() > bufferSize){
        buffer.pop();
    }
}