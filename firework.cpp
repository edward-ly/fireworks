#include "firework.h"

// Set our static (per class NOT per object!) variable values
const GLfloat Firework::GRAVITY		= 0.05f;
const GLfloat Firework::baselineYSpeed = -4.0f;
const GLfloat Firework::maxYSpeed	  = -4.0f;

// Constructor implementation
Firework::Firework()
{
	// We can re-initialize the same firework
	// later on without having to destroy the object and recreate it!
	hasExploded = false;
}

void Firework::initialize(GLint mouse_x, GLint mouse_y)
{
	// Pick random x/y speeds for each particle making up the firework
	// Note: Each particle in the firework must have the exact same values for the firework to rise as a single point!
	float xSpeedVal = -2 + ((float)rand() / (float)RAND_MAX) * 4.0f;
	float ySpeedVal = baselineYSpeed + ((float)rand() / (float)RAND_MAX) * maxYSpeed;
	//cout << ySpeedVal << endl;

	// Set initial x/y location and speeds for each particle in the firework
	for (int i = 0; i < FIREWORK_PARTICLES; i++)
	{
		x[i] = (float)mouse_x;
		y[i] = (float)mouse_y;
		xSpeed[i] = xSpeedVal;
		ySpeed[i] = ySpeedVal;
	}

	// Assign a random colour and full alpha (i.e. particle is completely opaque)
	red   = ((float)rand() / (float)RAND_MAX);
	green = ((float)rand() / (float)RAND_MAX);
	blue  = ((float)rand() / (float)RAND_MAX);
	alpha = 1.0f;

	// Size of the particle (as thrown to glPointSize) - range is 1.0f to 4.0f
	particleSize = 1.0f + ((float)rand() / (float)RAND_MAX) * 3.0f;

	// Start the explosion animation
	hasExploded = true;
	Firework::explode();
}

// Function to make a firework explode
void Firework::explode()
{
	for (int i = 0; i < FIREWORK_PARTICLES; i++)
	{
		// Dampen the horizontal speed by 1% per frame
		xSpeed[i] *= 0.99;

		// Move the particle
		x[i] += xSpeed[i];
		y[i] += ySpeed[i];

		// Apply gravity to the particle's speed
		ySpeed[i] += Firework::GRAVITY;
	}

	// Fade out the particles (alpha is stored per firework, not per particle)
	if (alpha > 0.0f)
	{
		alpha -= 0.01f;
	}
	else // Once the alpha hits zero, stop animation and wait for re-initialization
	{
		hasExploded = false;
	}
}
