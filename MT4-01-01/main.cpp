#include <Novice.h>
#include <cmath>
#include <cstring>

const char kWindowTitle[] = "DirectionToDirection";
const int kRowHeight = 20;

struct Vector3 {
    float x, y, z;
};

struct Matrix4x4 {
    float m[4][4];
};

// ================= Vector =================
float Length(const Vector3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
    float len = Length(v);
    return { v.x / len, v.y / len, v.z / len };
}

float Dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Cross(const Vector3& a, const Vector3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// ================= Matrix =================
Matrix4x4 MakeIdentity() {
    Matrix4x4 m{};
    m.m[0][0] = 1; m.m[1][1] = 1; m.m[2][2] = 1; m.m[3][3] = 1;
    return m;
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    float c = std::cos(angle);
    float s = std::sin(angle);
    float t = 1.0f - c;

    Matrix4x4 m{};

    m.m[0][0] = t * x * x + c;
    m.m[0][1] = t * x * y + s * z;
    m.m[0][2] = t * x * z - s * y;
    m.m[0][3] = 0;

    m.m[1][0] = t * x * y - s * z;
    m.m[1][1] = t * y * y + c;
    m.m[1][2] = t * y * z + s * x;
    m.m[1][3] = 0;

    m.m[2][0] = t * x * z + s * y;
    m.m[2][1] = t * y * z - s * x;
    m.m[2][2] = t * z * z + c;
    m.m[2][3] = 0;

    m.m[3][0] = 0;
    m.m[3][1] = 0;
    m.m[3][2] = 0;
    m.m[3][3] = 1;

    return m;
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {
    Vector3 f = Normalize(from);
    Vector3 t = Normalize(to);

    float dot = Dot(f, t);

    if (dot < -0.9999f) {
        Vector3 axis;
        if (std::abs(f.x) < 0.999f)
            axis = Normalize(Cross(f, { 1,0,0 }));
        else
            axis = Normalize(Cross(f, { 0,1,0 }));
        return MakeRotateAxisAngle(axis, 3.14159265f);
    }

    Vector3 axis = Normalize(Cross(f, t));
    float angle = std::acos(dot);

    return MakeRotateAxisAngle(axis, angle);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& m, const char* label) {
    Novice::ScreenPrintf(x, y, "%s", label);
    for (int i = 0; i < 4; i++) {
        Novice::ScreenPrintf(
            x, y + 20 + i * 20,
            "%6.3f %6.3f %6.3f %6.3f",
            m.m[i][0], m.m[i][1], m.m[i][2], m.m[i][3]
        );
    }
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    Novice::Initialize(kWindowTitle, 1280, 720);

    char keys[256] = {};
    char preKeys[256] = {};

    Vector3 from0 = Normalize({ 1.0f, 0.7f, 0.5f });
    Vector3 to0 = from0;

    Vector3 from1 = Normalize({ -0.6f, 0.9f, 0.2f });
    Vector3 to1 = Normalize({ 0.4f, 0.7f, -0.5f });

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();

        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        Matrix4x4 rotateMatrix0 = DirectionToDirection(
            Normalize({ 1.0f, 0.0f, 0.0f }),
            Normalize({ -1.0f, 0.0f, 0.0f })
        );

        Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
        Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

        MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");
        MatrixScreenPrintf(0, kRowHeight * 5, rotateMatrix1, "rotateMatrix1");
        MatrixScreenPrintf(0, kRowHeight * 10, rotateMatrix2, "rotateMatrix2");

        Novice::EndFrame();

        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) break;
    }

    Novice::Finalize();
    return 0;
}
