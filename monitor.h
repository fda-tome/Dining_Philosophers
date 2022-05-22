void pickup(int i);
void putdown(int i);
void initialization_code();

struct monitor{
    void (*pick)(int);
    void (*put)(int);
    void (*init_code)();
};
    
