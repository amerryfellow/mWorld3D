#include <stdlib.h>
#include <stdio.h>

using namespace std;

void multMatrixVector(double matrix[9], float vector[]) {
	cout << "multMatrixVector" << endl << "--- Vector was: {" << vector[0] << ", " << vector[1] << ", " << vector[2] << "}" << endl;
	cout << "--- Multiply by: { " << matrix[0];
	for(int a=1;a<9;a++)
		cout << ", " << matrix[a];

	cout << "}" << endl;

	float result[3];
	for(int i=0; i<3; i++) {
		float temp = 0;
		for(int j=i;j<i+3;j++)
			temp += (float) matrix[j] * vector[i];

		result[i] = temp;
	}

	vector[0] = result[0];
	vector[1] = result[1];
	vector[2] = result[2];

	cout << "--- Vector is:  {" << vector[0] << ", " << vector[1] << ", " << vector[2] << "}" << endl;
}

template<class T>
inline const T& min_(const T& a, const T& b) {
	return a < b ? a : b;
}

template<class T>
inline const T& min_(const T& a, const T& b, const T& c) {
	return a < b ? min_(a, c) : min_(b, c);
}

template<class T>
inline const T& max_(const T& a, const T& b) {
	return a < b ? b : a;
}

template<class T>
inline const T& max_(const T& a, const T& b, const T& c) {
	return a < b ? max_(b, c) : max_(a, c);
}

template <class T>
inline const T clamp (const T& value, const T& low, const T& high) {
	return min_ (max_(value,low), high);
}

class Vector3 {
	public:
		float x, y, z, w;

		Vector3( float xx, float yy, float zz){
			x = xx;
			y = yy;
			z = zz;
		}

		Vector3() {
			x = y = z = w = 0;
		}

		void normalise() {
			float mod = this->module();
			x = x/mod;
			y = y/mod;
			z = z/mod;
		}

		float module() {
			return sqrt( x*x + y*y + z*z );
		}

		void toString( char* buf ) {
			sprintf(buf, "( %f, %f, %f )\0", x, y, z);
		}

		void print() {
			cout << "( " << x << ", " << y << ", " << z << " )";
		}

		Vector3 operator* (const float &num) const;
		Vector3 operator+ (const Vector3 &vec) const;
		Vector3 operator+= (const Vector3 &vec);
};

Vector3 Vector3::operator* (const float &num) const {
	return Vector3(num * x, num * y, num * z);
}

Vector3 Vector3::operator+= (const Vector3 &vec) {
	return Vector3(
			x += vec.x,
			y += vec.y,
			z += vec.z
			);

}

Vector3 Vector3::operator+ (const Vector3 &vec) const {
	return Vector3(x + vec.x, y + vec.y, z + vec.z);
}

class Matrix4 {
	public:
		float matrix[16];

		Matrix4( float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8, float x9, float x10, float x11, float x12, float x13, float x14, float x15, float x16) {
			matrix[0] = x1;
			matrix[1] = x2;
			matrix[2] = x3;
			matrix[3] = x4;
			matrix[4] = x5;
			matrix[5] = x6;
			matrix[6] = x7;
			matrix[7] = x8;
			matrix[8] = x9;
			matrix[9] = x10;
			matrix[10] = x11;
			matrix[11] = x12;
			matrix[12] = x13;
			matrix[13] = x14;
			matrix[14] = x15;
			matrix[15] = x16;
		}

		Matrix4() {
			for(int i=0; i<16;i++)
				matrix[i] = 0;
		}

		void normalise() {

		}
};

class Quaternion {
	public:
		float x, y, z, w;

		Quaternion( float xx, float yy, float zz, float ww) {
			x = xx;
			y = yy;
			z = zz;
			w = ww;
		}

		Quaternion () {
			x = y = z = w = 0;
		}

		Quaternion operator*(const Quaternion&) const;
		Vector3 operator*(const Vector3&) const;

		void fromEuler(float x, float y, float z) {
			float sinx = sin(x/2);
			float siny = sin(y/2);
			float sinz = sin(z/2);
			float cosx = cos(x/2);
			float cosy = cos(y/2);
			float cosz = cos(z/2);

			this->x = cosx * cosy * cosz + sinx * siny * sinz;
			this->y = sinx * cosy * cosz - cosx * siny * sinz;
			this->z = cosx * siny * cosz + sinx * cosy * sinz;
			this->w = cosx * cosy * sinz - sinx * siny * cosz;

			this->normalise();
		}
/*

		void fromEuler(float pitch, float yaw, float roll) {
			// Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
			// and multiply those together.
			// the calculation below does the same, just shorter

			float p = pitch / 2.0;
			float y = yaw / 2.0;
			float r = roll / 2.0;

			float sinp = sin(p);
			float siny = sin(y);
			float sinr = sin(r);
			float cosp = cos(p);
			float cosy = cos(y);
			float cosr = cos(r);

			this->x = sinr * cosp * cosy - cosr * sinp * siny;
			this->y = cosr * sinp * cosy + sinr * cosp * siny;
			this->z = cosr * cosp * siny - sinr * sinp * cosy;
			this->w = cosr * cosp * cosy + sinr * sinp * siny;

			this->normalise();
		}
*/
		Quaternion getConjugate() {
			return Quaternion(-x, -y, -z, w);
		}

		void normalise() {
			// Don't normalize if we don't have to
			float mag2 = w * w + x * x + y * y + z * z;
			float TOLERANCE = 0.001;

			if (fabs(mag2) > TOLERANCE && fabs(mag2 - 1.0f) > TOLERANCE) {
				float mag = sqrt(mag2);
				w /= mag;
				x /= mag;
				y /= mag;
				z /= mag;
			}
		}

		void getAxisAngle(Vector3 *axis, float *angle) {
			float scale = sqrt(x * x + y * y + z * z);
			axis->x = x / scale;
			axis->y = y / scale;
			axis->z = z / scale;
			*angle = acos(w) * 2.0f;
		}

		// Convert from Axis Angle
		void fromAxisAngle(const Vector3 &v, float angle) {
			float sinAngle;
			angle *= 0.5f;
			Vector3 vn(v);
			vn.normalise();

			sinAngle = sin(angle);

			x = (vn.x * sinAngle);
			y = (vn.y * sinAngle);
			z = (vn.z * sinAngle);
			w = cos(angle);
		}

		Matrix4 getMatrix( ) const {
			float x2 = x * x;
			float y2 = y * y;
			float z2 = z * z;
			float xy = x * y;
			float xz = x * z;
			float yz = y * z;
			float wx = w * x;
			float wy = w * y;
			float wz = w * z;

			// This calculation would be a lot more complicated for non-unit length quaternions
			// Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
			//   OpenGL
			return Matrix4 ( 1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
					2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
					2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
		}

		void print() {
			cout << "( " << x << ", " << y << ", " << z << ", " << w << " )";
		}

		void toEuler(Vector3& euler) const {
			const float sqw = w*w;
			const float sqy = y*y;
			const float sqz = z*z;

			// heading = rotation about z-axis
			//euler.z = float (atan2(2.0 * (x*y +z*w),(sqx - sqy - sqz + sqw)));
			euler.x = float (atan2(2.0 * (x*y +z*w), 1-2 * (sqy + sqz)));

			// bank = rotation about x-axis
			//euler.x = float (atan2(2.0 * (y*z +x*w),(-sqx - sqy + sqz + sqw)));
			euler.z = float (atan2(2.0 * (y*z +x*w), 1-2 * (sqz + sqw)));

			// attitude = rotation about y-axis
			//euler.y = asinf( clamp(-2.0f * (x*z - y*w), -1.0f, 1.0f) );
			euler.y = asinf( -2.0f * (x*z - y*w) );
		}

};
/*
   Quaternion Quaternion::operator* (const Quaternion &rq) const {
// the constructor takes its arguments as (x, y, z, w)
return Quaternion(w * rq.x + x * rq.w + y * rq.z - z * rq.y,
w * rq.y + y * rq.w + z * rq.x - x * rq.z,
w * rq.z + z * rq.w + x * rq.y - y * rq.x,
w * rq.w - x * rq.x - y * rq.y - z * rq.z);
}*/

// multiplication operator
Quaternion Quaternion::operator* (const Quaternion& other) const {
	Quaternion tmp;

	tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
	tmp.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y);
	tmp.y = (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z);
	tmp.z = (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x);

	return tmp;
}


// Multiplying a quaternion q with a vector v applies the q-rotation to v
Vector3 Quaternion::operator* (const Vector3 &vec) const {
	Vector3 vn(vec);
	vn.normalise();

	Quaternion vecQuat, resQuat, coniug;
	vecQuat.x = vn.x;
	vecQuat.y = vn.y;
	vecQuat.z = vn.z;
	vecQuat.w = 0.0f;

	coniug = Quaternion(-x, -y, -z, w);
	resQuat = vecQuat * coniug;
	resQuat = *this * resQuat;

	return (Vector3(resQuat.x, resQuat.y, resQuat.z));
}
