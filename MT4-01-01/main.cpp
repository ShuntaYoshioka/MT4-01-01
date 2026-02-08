#include <Novice.h>
#include <cmath>
#include <cstdio>

const char kWindowTitle[] = "01-05";
const int kRowHeight = 20;

struct Vector3 {
    float x, y, z;
};

struct Quaternion {
    float x, y, z, w;
};

float Dot(const Quaternion& q0, const Quaternion& q1) {
    return q0.x * q1.x +
        q0.y * q1.y +
        q0.z * q1.z +
        q0.w * q1.w;
}

float Norm(const Quaternion& q) {
    return std::sqrt(Dot(q, q));
}

Quaternion Normalize(const Quaternion& q) {
    float n = Norm(q);
    return { q.x / n, q.y / n, q.z / n, q.w / n };
}

Quaternion Multiply(const Quaternion& a, const Quaternion& b) {
    return {
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    };
}

Quaternion MakeRotateAxisAngleQuaternion(Vector3 axis, float angle) {
    float length = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    axis.x /= length;
    axis.y /= length;
    axis.z /= length;

    float half = angle * 0.5f;
    float s = std::sin(half);

    return {
        axis.x * s,
        axis.y * s,
        axis.z * s,
        std::cos(half)
    };
}

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
    Quaternion qStart = q0;
    Quaternion qEnd = q1;

    float dot = Dot(qStart, qEnd);

    if (dot < 0.0f) {
        qStart = { -qStart.x, -qStart.y, -qStart.z, -qStart.w };
        dot = -dot;
    }

    if (dot > 0.9995f) {
        Quaternion result = {
            qStart.x + t * (qEnd.x - qStart.x),
            qStart.y + t * (qEnd.y - qStart.y),
            qStart.z + t * (qEnd.z - qStart.z),
            qStart.w + t * (qEnd.w - qStart.w)
        };
        return Normalize(result);
    }

    float theta = std::acos(dot);
    float sinTheta = std::sin(theta);

    float scale0 = std::sin((1.0f - t) * theta) / sinTheta;
    float scale1 = std::sin(t * theta) / sinTheta;

    Quaternion result = {
        scale0 * qStart.x + scale1 * qEnd.x,
        scale0 * qStart.y + scale1 * qEnd.y,
        scale0 * qStart.z + scale1 * qEnd.z,
        scale0 * qStart.w + scale1 * qEnd.w
    };

    return result;
}

void QuaternionScreenPrintf(int x, int y, const Quaternion& q, const char* label) {
    Novice::ScreenPrintf(x, y, "%s : %.2f %.2f %.2f %.2f",
        label, q.x, q.y, q.z, q.w);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    Novice::Initialize(kWindowTitle, 1280, 720);

    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    Quaternion rotation0 =
        MakeRotateAxisAngleQuaternion({ 0.71f, 0.71f, 0.0f }, 0.3f);

    Quaternion rotation1 =
        MakeRotateAxisAngleQuaternion({ 0.71f, 0.0f, 0.71f }, 3.141592f);

    Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
    Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
    Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
    Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
    Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);

    while (Novice::ProcessMessage() == 0) {

        Novice::BeginFrame();

        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        QuaternionScreenPrintf(0, kRowHeight * 0, interpolate0, "interpolate0 Slerp0.0");
        QuaternionScreenPrintf(0, kRowHeight * 1, interpolate1, "interpolate1 Slerp0.3");
        QuaternionScreenPrintf(0, kRowHeight * 2, interpolate2, "interpolate2 Slerp0.5");
        QuaternionScreenPrintf(0, kRowHeight * 3, interpolate3, "interpolate3 Slerp0.7");
        QuaternionScreenPrintf(0, kRowHeight * 4, interpolate4, "interpolate4 Slerp1.0");

        Novice::EndFrame();

        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    Novice::Finalize();
    return 0;
}
