/**
 * Includes
 */
#include <GL/glfw.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <math.h>

#include "lib.h"

#define PI 3.14
using namespace std;

/**
 * Code
 */


class Camera {
	protected:
		Vector3 persForward;
		Vector3 persTo;
		Vector3 pos;

	public:
		Quaternion look;
		float moveMultiplier, lookMultiplier;

		static Camera* getCamera() {
			static Camera* camera = new Camera;

			return camera;
		}

		Camera() {
			// Initialize Camera
			Quaternion look;
			look.fromEuler(0, 0, 1);

			persForward = Vector3(0, 0, 0);
			persTo = Vector3(0, 0, 0);
			pos = Vector3(0, 2, 0);

			moveMultiplier = 0.1;
			lookMultiplier = 1;
		}

		void updateCamera() {
			// Initialization
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			/**
			 * Persistent Movements
			 */

			if( persForward.module() != 0 ) {
				Vector3 movForward = Vector3(persForward);
			
				// Debug
				// movForward.print();
			
				movForward = look * movForward;
				
				Vector3 v;
				look.toEuler(v);
				
				// Debug
				// v.print();
				// movForward.print();
				// cout << endl;

				pos = pos + movForward * moveMultiplier;
			}

			if( persTo.module() != 0 ) {
				pos = pos + persTo * moveMultiplier;
			}

			/**
			 * Apply Transformations
			 */

			float angle;
			Vector3 axis;
			look.getAxisAngle(&axis, &angle);
			
			angle *= 180/M_PI;
			
			// Debug
			// cout << "Screen rotated " << angle << " on ";
			// axis.print();
			// cout << endl;
			
			// Camera Vector
			glRotatef(angle, axis.x, axis.y, axis.z);
			
			glTranslatef(pos.x, pos.y, pos.z);
		}

		void moveBy(float x, float y, float z) {
			// Scaling
			pos.x -= moveMultiplier * x;
			pos.y -= moveMultiplier * y;
			pos.z -= moveMultiplier * z;
		}

		void persistentMoveBy(float x, float y, float z) {
			persTo = persTo + Vector3(x, y, z);
		}

		void persistentMoveForward(float x, float y, float z) {
			persForward += Vector3(x, y, z) * -1;
		}

		void moveTo(float x, float y, float z) {
			pos = Vector3(x, y, z);
		}

		void lookAt(float x, float y, float z) {
			x *= lookMultiplier * M_PI;
			y *= lookMultiplier * M_PI;
			z *= lookMultiplier * M_PI;
			
			//look.fromEuler(z, x, y);

			Quaternion a,b;
			a.fromAxisAngle( Vector3(0, 1, 0), -x );
			b.fromAxisAngle( Vector3(1, 0, 0), -y );

			look = a * b;
		}
};
