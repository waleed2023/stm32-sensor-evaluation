#include <math.h>
#include "angle.h"

#define RAD2DEG (180.0f / M_PI)
#define FILTER_ALPHA 0.1f  // Glättungsfaktor (zwischen 0.0 und 1.0)

static float pitchOffset = 0.0f;       // zur Kalibrierung
static float smoothedPitch = 0.0f;     // Für Tiefpass

// Initialisierung für Kalibrierung (optional aufrufen)
void Set_PitchOffset(float offset) {
    pitchOffset = offset;
}

// Gibt den gefilterten Pitch-Winkel zurück
float Calculate_Pitch(int16_t ax, int16_t ay, int16_t az) {
    float fx = (float)ax;
    float fy = (float)ay;
    float fz = (float)az;

    float rawPitch = atan2f(fx, sqrtf(fy * fy + fz * fz)) * RAD2DEG;
    rawPitch -= pitchOffset;

    smoothedPitch = (1.0f - FILTER_ALPHA) * smoothedPitch + FILTER_ALPHA * rawPitch;
    return smoothedPitch;
}
