#include <linearAlgebra.hpp>

#ifdef NATIVE
#include <iostream>
#endif

void setup() {
    #ifndef NATIVE
    Serial.begin(115200);
    #endif
    
    Vector<int> v(5); // warning: by default the vector is allocate but not initialized, it contains random values

    #ifndef NATIVE
    Serial.print("empty vector of 5 elements : v=" + to_string(v) + "\n");
    #else
    std::cout << "empty vector of 5 elements : v=" << to_string(v) << std::endl;
    #endif

    

    Vector<int> v2(5);
    v2.fill(2);
    v2[0] -= 1;
    v += v2;

    #ifndef NATIVE
    Serial.print("v += " + to_string(v2) + " => v=" + to_string(v) + "\n");
    #else
    std::cout << "v += " << to_string(v2) << " => v=" << to_string(v) << std::endl;
    #endif
}

void loop() {}


#ifdef NATIVE
int main()
{
    setup();
    while (true) loop();    
    return 0;
}
#endif