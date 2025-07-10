#include <angle.h>
#include <math.h>



// winkel Berechnung
float Calculate_Pitch(int16_t ax, int16_t ay, int16_t az) {
    float fx = (float)ax;
    float fy = (float)ay;
    float fz = (float)az;
    float pitch = atan2f(fx, sqrtf(fy * fy + fz * fz)) * 180.0f / 3.14159f;
    //printf("Accelerometer X: %.2f, Y: %.2f, Z: %.2f -> Pitch: %.2f°\n", fx, fy, fz, pitch);
    return pitch;
}
