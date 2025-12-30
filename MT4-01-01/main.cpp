#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "MT4";

struct Vector3 {
	float x, y, z;
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

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	float c = std::cos(angle);
	float s = std::sin(angle);
	float t = 1.0f - c;

	Matrix4x4 m = {};

	m.m[0][0] = t * x * x + c;
	m.m[0][1] = t * x * y + s * z;
	m.m[0][2] = t * x * z - s * y;
	m.m[0][3] = 0.0f;

	m.m[1][0] = t * x * y - s * z;
	m.m[1][1] = t * y * y + c;
	m.m[1][2] = t * y * z + s * x;
	m.m[1][3] = 0.0f;

	m.m[2][0] = t * x * z + s * y;
	m.m[2][1] = t * y * z - s * x;
	m.m[2][2] = t * z * z + c;
	m.m[2][3] = 0.0f;

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;

	return m;
}

// ================= WinMain =================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 axis = Normalize({ 1.0f, 1.0f, 1.0f });
	float angle = 0.44f;

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);

		MatrixScreenPrintf(0, 0, rotateMatrix, "rotateMatrix");

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
