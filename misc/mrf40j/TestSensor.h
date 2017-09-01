//#include "arduino.h"
#include "ISerializable.h"

class TestSensor : ISerializable {
private:
    long totalTime = 0;
    long lastTime = 0;
    long deltaTime = 0;

public:
    TestSensor() { lastTime = millis(); }

    void loop() {
        totalTime = millis();
        deltaTime = totalTime - lastTime;
        lastTime = totalTime;
    }

public:
    int get_size() const override { return 3*sizeof(long); }

    void serialize(char* buffer) const override {
        memcpy(&buffer[0*sizeof(long)], totalTime);
        memcpy(&buffer[1*sizeof(long)], lastTime);
        memcpy(&buffer[2*sizeof(long)], deltaTime);
    }

    void deserialize(char* buffer) override {
        memcpy(totalTime, &buffer[0*sizeof(long)]);
        memcpy( lastTime, &buffer[1*sizeof(long)]);
        memcpy(deltaTime, &buffer[2*sizeof(long)]);
    }
}