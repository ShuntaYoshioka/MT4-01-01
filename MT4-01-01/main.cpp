#include <Novice.h>
#include <cmath>
#include <cstring>

const char kWindowTitle[] = "01-04";
const int kRowHeight = 20;

struct Vector3 {
    float x, y, z;
};

struct Quaternion {
    float x, y, z, w;
};

struct Matrix4x4 {
    float m[4][4];
};

float Length(const Vector3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
    float len = Length(v);
    return { v.x / len, v.y / len, v.z / len };
}

Vector3 Transform(const Vector3& v, const Matrix4x4& m) {
    Vector3 r;
    r.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
    r.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
    r.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
    return r;
}

Quaternion Conjugate(const Quaternion& q) {
    return { -q.x, -q.y, -q.z, q.w };
}

Quaternion Multiply(const Quaternion& a, const Quaternion& b) {
    Quaternion r;
    r.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    r.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    r.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    r.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    return r;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
    Vector3 n = Normalize(axis); // ★必須

    float half = angle * 0.5f;
    float s = std::sin(half);

    return {
        n.x * s,
        n.y * s,
        n.z * s,
        std::cos(half)
    };
}

Vector3 RotateVector(const Vector3& v, const Quaternion& q) {
    Quaternion p = { v.x, v.y, v.z, 0.0f };
    Quaternion qConj = Conjugate(q);

    Quaternion result = Multiply(Multiply(q, p), qConj);

    return { result.x, result.y, result.z };
}

Matrix4x4 MakeRotateMatrix(const Quaternion& q) {
    Matrix4x4 m{};

    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;
    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;

    m.m[0][0] = 1 - 2 * (yy + zz);
    m.m[0][1] = 2 * (xy + wz);
    m.m[0][2] = 2 * (xz - wy);
    m.m[0][3] = 0;

    m.m[1][0] = 2 * (xy - wz);
    m.m[1][1] = 1 - 2 * (xx + zz);
    m.m[1][2] = 2 * (yz + wx);
    m.m[1][3] = 0;

    m.m[2][0] = 2 * (xz + wy);
    m.m[2][1] = 2 * (yz - wx);
    m.m[2][2] = 1 - 2 * (xx + yy);
    m.m[2][3] = 0;

    m.m[3][0] = 0;
    m.m[3][1] = 0;
    m.m[3][2] = 0;
    m.m[3][3] = 1;

    return m;
}

void QuaternionScreenPrintf(int x, int y, const Quaternion& q, const char* name) {
    Novice::ScreenPrintf(x, y, "%s", name);
    Novice::ScreenPrintf(x, y + 20, "x: %.2f", q.x);
    Novice::ScreenPrintf(x, y + 40, "y: %.2f", q.y);
    Novice::ScreenPrintf(x, y + 60, "z: %.2f", q.z);
    Novice::ScreenPrintf(x, y + 80, "w: %.2f", q.w);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& m, const char* name) {
    Novice::ScreenPrintf(x, y, "%s", name);
    for (int i = 0; i < 4; i++) {
        Novice::ScreenPrintf(
            x, y + 20 + i * 20,
            "%.2f %.2f %.2f %.2f",
            m.m[i][0], m.m[i][1], m.m[i][2], m.m[i][3]
        );
    }
}

void VectorScreenPrintf(int x, int y, const Vector3& v, const char* name) {
    Novice::ScreenPrintf(x, y, "%s", name);
    Novice::ScreenPrintf(x, y + 20, "x: %.2f y: %.2f z: %.2f", v.x, v.y, v.z);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    Novice::Initialize(kWindowTitle, 1280, 720);

    char keys[256]{};
    char preKeys[256]{};

    Quaternion rotation =
        MakeRotateAxisAngleQuaternion(
            Normalize({ 1.0f, 0.4f, -0.2f }),
            0.45f
        );

    Vector3 pointY = { 2.1f, -0.9f, 1.3f };

    Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);

    Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
    Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();

        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        QuaternionScreenPrintf(0, kRowHeight * 0, rotation, "rotation");
        MatrixScreenPrintf(0, kRowHeight * 6, rotateMatrix, "rotateMatrix");
        VectorScreenPrintf(0, kRowHeight * 12, rotateByQuaternion, "rotateByQuaternion");
        VectorScreenPrintf(0, kRowHeight * 16, rotateByMatrix, "rotateByMatrix");

        Novice::EndFrame();

        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}
