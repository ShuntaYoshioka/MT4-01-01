#include <Novice.h>
#include <cmath>
#include <cstring>

const char kWindowTitle[] = "01-03";

struct Quaternion {
    float x;
    float y;
    float z;
    float w;
};

Quaternion IdentityQuaternion() {
    return { 0.0f, 0.0f, 0.0f, 1.0f };
}

Quaternion Conjugate(const Quaternion& q) {
    return { -q.x, -q.y, -q.z, q.w };
}


float Norm(const Quaternion& q) {
    return std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

Quaternion Normalize(const Quaternion& q) {
    float n = Norm(q);
    return { q.x / n, q.y / n, q.z / n, q.w / n };
}

Quaternion Inverse(const Quaternion& q) {
    Quaternion conj = Conjugate(q);
    float n2 = Norm(q);
    n2 = n2 * n2; // norm^2

    return { conj.x / n2, conj.y / n2, conj.z / n2, conj.w / n2 };
}

Quaternion Multiply(const Quaternion& a, const Quaternion& b) {
    Quaternion result;

    result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;

    return result;
}

void PrintQuaternion(int x, int y, const Quaternion& q, const char* label) {
    Novice::ScreenPrintf(x, y, "%s", label);
    Novice::ScreenPrintf(x, y + 20, "x: %.3f", q.x);
    Novice::ScreenPrintf(x, y + 40, "y: %.3f", q.y);
    Novice::ScreenPrintf(x, y + 60, "z: %.3f", q.z);
    Novice::ScreenPrintf(x, y + 80, "w: %.3f", q.w);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    Novice::Initialize(kWindowTitle, 1280, 720);

    char keys[256] = {};
    char preKeys[256] = {};

    Quaternion q1 = { 2.0f, 3.0f, 4.0f, 1.0f };
    Quaternion q2 = { 1.0f, 3.0f, 5.0f, 2.0f };

    Quaternion identity = IdentityQuaternion();
    Quaternion conj = Conjugate(q1);
    Quaternion inv = Inverse(q1);
    Quaternion normal = Normalize(q1);
    Quaternion mul1 = Multiply(q1, q2);
    Quaternion mul2 = Multiply(q2, q1);
    float norm = Norm(q1);

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();

        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        PrintQuaternion(0, 0, identity, "Identity");
        PrintQuaternion(200, 0, conj, "Conjugate");
        PrintQuaternion(400, 0, inv, "Inverse");
        PrintQuaternion(600, 0, normal, "Normalize");
        PrintQuaternion(800, 0, mul1, "Multiply q1 * q2");
        PrintQuaternion(1000, 0, mul2, "Multiply q2 * q1");

        Novice::ScreenPrintf(0, 200, "Norm(q1): %.3f", norm);

        Novice::EndFrame();

        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}
