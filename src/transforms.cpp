#include "transforms.h"

#include "CGL/matrix3x3.h"
#include "CGL/vector2D.h"
#include "CGL/vector3D.h"

namespace CGL { 

Vector2D operator*(const Matrix3x3 &m, const Vector2D &v) {
	Vector3D mv = m * Vector3D(v.x, v.y, 1);
	return Vector2D(mv.x / mv.z, mv.y / mv.z);
}

// Part 4: Fill these in
Matrix3x3 translate(float dx, float dy) {
	return Matrix3x3( );
}

Matrix3x3 scale(float sx, float sy) {
	return Matrix3x3( );
}

// The input argument is in degrees counterclockwise
Matrix3x3 rotate(float deg) {
	return Matrix3x3( );
}

}