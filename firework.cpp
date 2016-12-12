#include "firework.h"

// Set our static (per class NOT per object!) variable values
const GLfloat Firework::GRAVITY		   = 0.1f;

// Constructor implementation
Firework::Firework()
{
	// We can re-initialize the same firework
	// later on without having to destroy the object and recreate it!
	hasExploded = false;
}

void Firework::initialize(GLint mouse_x, GLint mouse_y, GLfloat size)
{
	// Set initial x/y location and random speeds for each particle in the firework
	for (int i = 0; i < FIREWORK_PARTICLES; i++)
	{
		x[i] = (float)mouse_x;
		y[i] = (float)mouse_y;
		xSpeed[i] = (rand() / (float)RAND_MAX) * 8 - (rand() / (float)RAND_MAX) * 6; //-4 + (rand() / (float)RAND_MAX) * 8;
		ySpeed[i] = (rand() / (float)RAND_MAX) * 8 - (rand() / (float)RAND_MAX) * 6; //-4 + (rand() / (float)RAND_MAX) * 8;
	}

	colorSet(1.0, 0.1, 1.0);
	// Assign a random colour GLfloatand full alpha (i.e. particle is completely opaque)
	/*red   = ((float)rand() / (float)RAND_MAX);
	green = ((float)rand() / (float)RAND_MAX);
	blue  = ((float)rand() / (float)RAND_MAX);*/

	alpha = 1.0f;

	// Size of the particle (as thrown to glPointSize) - range is 1.0f to 4.0f
	if (size > 0){
		particleSize = size;
	} else{
		particleSize = 1.0f + ((float)rand() / (float)RAND_MAX) * 3.0f;
	}



	// Start the explosion animation
	hasExploded = true;
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
		ySpeed[i] -= Firework::GRAVITY;
	}

	// Fade out the particles (alpha is stored per firework, not per particle)
	if (alpha > 0.0f)
	{
		alpha -= 0.01f;
	}
	else // Firework has disappeared, re-initialization can occur later
	{
		hasExploded = false;
	}
}

	void Firework::colorSet(GLfloat newRed, GLfloat newGreen, GLfloat newBlue)
	{
		red   = newRed * ((float)rand() / (float)RAND_MAX);
		green = newGreen * ((float)rand() / (float)RAND_MAX);
		blue  = newBlue * ((float)rand() / (float)RAND_MAX);
	}
