
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
164
165
166
167
168
169
170
171
172
173
174
175
176
177
178
179
180
181
182
183
184
185
186
187
188
189
190
191
192
193
194
195
196
197
198
199
200
201
202
203
204
205
206
207
208
209
210
211
212
213
214
215
216
217
218
219
220
221
222
223
224
225
226
227
228
229
230
231
232
233
234
235
236
237
238
239
240
241
242
243
244
245
246
247
248
249
250
251
252
253
254
255
256
257
258
259
260
261
262
263
264
265
266
267
268
269
270
271
272
273
274
275
276
277
278
279
280
281
282
283
284
285
286
287
288
289
290
291
292
293
294
295
296
297
298
299
300
301
302
303
304
305
306
307
308
309
310
311
312
313
314
315
316
317
318
319
320
321
322
323
324
325
326
327
328
329
330
331
332
333
334
335
336
337
338
339
340
341
342
343
344
345
346
347
348
349
350
351
352
353
354
355
356
357
358
359
360
361
362
363
364
365
366
367
368
369
370
371
372
373
374
375
376
377
378
379
380
381
382
383
384
385
386
387
388
389
390
391
392
393
394
395
396
397
398
399
400
401
402
403
404
405
406
407
408
409
410
411
412
413
414
415
416
417
418
419
420
421
422
423
424
425
426
427
428
429
430
431
432
433
434
435
436
437
438
439
440
441
442
443
444
445
446
447
448
449
450
451
452
453
454
455
456
457
458
459
460
461
462
463
464
465
466
467
468
469
470
471
472
473
474
475
476
477
478
479
480
481
// Uncomment for Win32 systems
// Search for and include the opengl32 and glu (GL-Utility) libraries
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
 
#include <iostream>
#include <ctime>
#include <time.h>		// Needed to use random numbers
//#include <windows.h>	// *** IMPORTANT: Uncomment for Win32 systems - This must come -BEFORE- gl.h in the include list! ***
#include <GL/gl.h>		// OpenGL headers
#include <SDL/SDL.h>		// Simple DirectMedia Layer headers
using namespace std;
 
// Define our initial screen width, height, and colour depth
GLint SCREEN_WIDTH  = 800;
GLint SCREEN_HEIGHT = 600;
GLint SCREEN_BPP	=  16;
 
// Define our SDL surface
SDL_Surface *surface;
 
const int FIREWORKS = 15;		   // Number of fireworks
const int FIREWORK_PARTICLES = 50;  // Number of particles per firework
 
class Firework
{
	public:
		GLfloat x[FIREWORK_PARTICLES];
		GLfloat y[FIREWORK_PARTICLES];
		GLfloat xSpeed[FIREWORK_PARTICLES];
		GLfloat ySpeed[FIREWORK_PARTICLES];
 
		GLfloat red;
		GLfloat blue;
		GLfloat green;
		GLfloat alpha;
 
		GLint framesUntilLaunch;
 
		GLfloat particleSize;
		GLboolean hasExploded;
 
		static const GLfloat baselineYSpeed;
		static const GLfloat maxYSpeed;
 
		static const GLfloat GRAVITY;
 
		Firework(); // Constructor declaration
		void initialise();
		void move();
		void explode();
};
 
const GLfloat Firework::GRAVITY = 0.05f;
const GLfloat Firework::baselineYSpeed = -4.0f;
const GLfloat Firework::maxYSpeed = -4.0f;
 
// Constructor implementation
Firework::Firework()
{
	initialise();
}
 
void Firework::initialise()
{
	// Pick an initial x location and  random x/y speeds
	float xLoc = (rand() / (float)RAND_MAX) * SCREEN_WIDTH;
	float xSpeedVal = -2 + (rand() / (float)RAND_MAX) * 4.0f;
	float ySpeedVal = baselineYSpeed + ((float)rand() / (float)RAND_MAX) * maxYSpeed;
 
	// Set initial x/y location and speeds
	for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
	{
		x[loop] = xLoc;
		y[loop] = SCREEN_HEIGHT + 10.0f; // Push the particle location down off the bottom of the screen
		xSpeed[loop] = xSpeedVal;
		ySpeed[loop] = ySpeedVal;
	}
 
	// Assign a random colour and full alpha (i.e. particle is completely opaque)
	red   = (rand() / (float)RAND_MAX);
	green = (rand() / (float)RAND_MAX);
	blue  = (rand() / (float)RAND_MAX);
	alpha = 1.0f;
 
	// Firework will launch after a random amount of frames between 0 and 400
	framesUntilLaunch = ((int)rand() % 400);
 
	// Size of the particle (as thrown to glPointSize) - range is 1.0f to 4.0f
	particleSize = 1.0f + ((float)rand() / (float)RAND_MAX) * 3.0f;
 
	// Flag to keep trackof whether the firework has exploded or not
	hasExploded = false;
 
	cout << "Initialised a firework." << endl;
}
 
void Firework::move()
{
	for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
	{
		// Once the firework is ready to launch start moving the particles
		if (framesUntilLaunch <= 0)
		{
			x[loop] += xSpeed[loop];
 
			y[loop] += ySpeed[loop];
 
			ySpeed[loop] += Firework::GRAVITY;
		}
	}
	framesUntilLaunch--;
 
	// Once a fireworks speed turns positive (i.e. at top of arc) - blow it up!
	if (ySpeed[0] > 0.0f)
	{
		for (int loop2 = 0; loop2 < FIREWORK_PARTICLES; loop2++)
		{
			// Set a random x and y speed beteen -4 and + 4
			xSpeed[loop2] = -4 + (rand() / (float)RAND_MAX) * 8;
			ySpeed[loop2] = -4 + (rand() / (float)RAND_MAX) * 8;
		}
 
		cout << "Boom!" << endl;
		hasExploded = true;
	}
}
 
void Firework::explode()
{
	for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
	{
		// Dampen the horizontal speed by 1% per frame
		xSpeed[loop] *= 0.99;
 
		// Move the particle
		x[loop] += xSpeed[loop];
		y[loop] += ySpeed[loop];
 
		// Apply gravity to the particle's speed
		ySpeed[loop] += Firework::GRAVITY;
	}
 
	// Fade out the particles (alpha is stored per firework, not per particle)
	if (alpha > 0.0f)
	{
		alpha -= 0.01f;
	}
	else // Once the alpha hits zero reset the firework
	{
		initialise();
	}
}
 
// Create our array of fireworks
Firework fw[FIREWORKS];
 
// Function to release/destroy our resources and exit
int quit(int returnCode)
{
	// Clean up the window
	SDL_Quit();
 
	// Exit appropriately
	exit(returnCode);
}
 
 
// Function to set some initial OpenGL state-machine properties
int initGL()
{
	//  Enable smooth shading (i.e. gouraud shading)
	glShadeModel(GL_SMOOTH);
 
	// Set our clear colour to black with full opacity. Syntax is (r, g, b, alpha).
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 
	// Disable depth testing (because we're working in 2D!)
	glDisable(GL_DEPTH_TEST);
 
	// Enable blending (we need this to be able to use an alpha component)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
	// Set the accumulation buffer clearing colour to black at 0,0f alpha
	glClearAccum(0.0f, 0.0f, 0.0f, 1.0f);
 
	glPointSize(5.0f);
 
	return 0;
}
 
// Function to draw our OpenGL scene
int drawScene()
{
	// Set our clear mode to clear the screen -only-
	glClear(GL_COLOR_BUFFER_BIT);
 
	// Take the contents of the current accumulation buffer and copy it to the colour buffer with each pixel multiplied by a factor
	// i.e. we clear the screen, draw the last frame again (which we saved in the accumulation buffer), then draw our stuff at its new location on top of that
	glAccum(GL_RETURN, 0.95f);
 
	// Clear the accumulation buffer (don't worry, we re-grab the screen into the accumulation buffer after drawing our current frame!)
	glClear(GL_ACCUM_BUFFER_BIT);
 
	// Set ModelView matrix mode and reset to the default identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 
	// Displacement trick for exact pixelisation
	glTranslatef(0.375, 0.375, 0);
 
	// Draw fireworks
	//cout << "Firework count is: " << Firework::fireworkCount << endl;
	for (int loop = 0; loop < FIREWORKS; loop++)
	{
		for (int particleLoop = 0; particleLoop < FIREWORK_PARTICLES; particleLoop++)
		{
 
			// Set the point size of the firework particles (this needs to be called BEFORE opening the glBegin(GL_POINTS) section!)
			glPointSize(fw[loop].particleSize);
 
			glBegin(GL_POINTS);
 
				// Set colour to yellow on way up, then whatever colour firework should be when exploded
				if (fw[loop].hasExploded == false)
				{
					glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
				}
				else
				{
					glColor4f(fw[loop].red, fw[loop].green, fw[loop].blue, fw[loop].alpha);
				}
 
				// Draw the point
				glVertex2f(fw[loop].x[particleLoop], fw[loop].y[particleLoop]);
			glEnd();
		}
 
		// Move the firework appropriately depending on its explosion state
		if (fw[loop].hasExploded == false)
		{
			fw[loop].move();
		}
		else
		{
			fw[loop].explode();
		}
	}
 
	// Swap active and visual pages to display our output to the screen
	SDL_GL_SwapBuffers();
 
	// Take the contents of the current draw buffer and copy it to the accumulation buffer with each pixel modified by a factor
	// The closer the factor is to 1.0f, the longer the trails... Don't exceed 1.0f - you get garbage.
	glAccum(GL_ACCUM, 0.9f);
 
	// Return clean state
	return 0;
 
} // End of drawScene function
 
// Function to initialise variables before use
void initVars()
{
	// Seed random number generator
	srand(time(NULL));
}
 
// Function to reset our viewport after a window resize
int resizeWindow(int width, int height)
{
	// Create a variable for our height/width ratio
	GLfloat ratio;
 
	// Protect against a divide by zero error when resizing
	if (height == 0)
	{
		height = 1;
	}
	if (width == 0)
	{
		width = 1;
	}
 
	// Calculate our new (post-resize) window ratio
	if (width > height)
	{
		ratio = (GLfloat)width / (GLfloat)height;
		ratio = (GLfloat)height / (GLfloat)width;
	}
	else
	{
		ratio = (GLfloat)height / (GLfloat)width;
	}
 
	// Setup our viewport to be the entire size of the window
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
 
	// Change to the projection matrix and reset the matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
 
	// Change to orthographic (2D) projection and switch to the modelview matrix
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 
	return 0;
}
 
 
// Function to handle key press events
void handleKeyPress(SDL_keysym *keysym)
{
	switch(keysym->sym)
	{
		case SDLK_ESCAPE: // ESC key pressed? Then quit program
			quit(0);
			break;
 
		case SDLK_F1: // F1 key pressed? Then toggle fullscreen mode
			SDL_WM_ToggleFullScreen(surface);
			break;
 
		default:
			// Do nothing by default!
			break;
 
	} // End of switch statement
}
 
// Function to set up the screen
void setupScreen(int& videoFlags)
{
	// Create a constant pointer to our SDL_VideoInfo details
	const SDL_VideoInfo *videoInfo;
 
	// Initialise SDL - quit out if we fail
	if (SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		fprintf( stderr, "Video initialization failed: %s\n", SDL_GetError() );
		quit(1);
	}
 
	// Get our video information
	videoInfo = SDL_GetVideoInfo();
 
	// Quit if we couldn't get any videoInfo values back from our SDL_GetVideoInfo call
	if (!videoInfo)
	{
		cerr << "Video query failed: " << SDL_GetError() << endl;
		quit(1);
	}
 
	// Setup our SDL to use OpenGL with double buffering, a hardware palette and enable window resizing
	videoFlags  = SDL_OPENGL;		  // Enable OpenGL in SDL
	videoFlags |= SDL_GL_DOUBLEBUFFER; // Enable double buffering
	videoFlags |= SDL_HWPALETTE;	   // Store the palette in hardware
	videoFlags |= SDL_RESIZABLE;	   // Enable window resizing
 
	// Check if our SDL surfaces can be stored in hardware memory or not
	if (videoInfo->hw_available)
	{
		videoFlags |= SDL_HWSURFACE;
	}
	else
	{
		videoFlags |= SDL_SWSURFACE;
	}
 
	// Do hardware video blits if possible
	if (videoInfo->blit_hw)
	{
		videoFlags |= SDL_HWACCEL;
	}
 
	// Set up OpenGL for double buffering in SDL
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
 
	// Get our SDL surface to draw to
	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags);
 
	// Quit out if we can't get a SDL surface
	if (!surface)
	{
		cerr << "Video mode set failed: " << SDL_GetError() << endl;
		quit(1);
	}
 
} // End of setupScreen function
 
 
// Function to handle all SDL events appropriately
void controlLoop(int& videoFlags, SDL_Event& event)
{
	// Static var to keep track of whether we should now exit or not
	static bool done = false;
 
	// Keep going until done is true...
	while (!done)
	{
		// Handle the events in the queue
		while ( SDL_PollEvent(&event) )
		{
			// Depending on what event has occured, handle it appropriately
			switch (event.type) {
 
				// If the window is resized then call the appropriate resizeWindow function
				case SDL_VIDEORESIZE:
					surface = SDL_SetVideoMode(event.resize.w, event.resize.h, 16, videoFlags );
					if (!surface)
					{
						cerr << "Could not get a surface after resize: " << SDL_GetError() << endl;
						quit(1);
					}
					resizeWindow(event.resize.w, event.resize.h);
					break;
 
				// If a key is pressed then handle the keypress appropriately
				case SDL_KEYDOWN:
					handleKeyPress(&event.key.keysym);
					break;
 
				case SDL_QUIT:
					done = true;
					break;
 
				// By default, do nothing...
				default:
					// Nothing to see here... move along.
					break;
 
			} // End of switch statement
 
		} // End of while SDL_PollEvent condition
 
		// Call the drawScene function to draw our objects to the screen!
		drawScene();
 
		// Cheap-as-chips limit framerate to approximately 60fps
		SDL_Delay(16);
 
	} // End of main while (!done) loop
 
} // End of controlLoop function
 
 
// Our main function
int main(int argc, char **argv)
{
	// Flags to pass to SDL_SetVideoMode - made global for use in setupScreen and controlLoop functions
	int videoFlags;
 
	// SDL_Event type variable to keep track of events - used in our controlLoop function
	SDL_Event event;
 
	// Set up our screen
	setupScreen(videoFlags);
 
	// Set the window and tray caption
	SDL_WM_SetCaption("Simple OpenGL Fireworks with Trails | r3dux.org", "Fireworks w/ Trails");
 
	// Initialise our variables
	initVars();
 
	// Initialise OpenGL
	initGL();
 
	// Resize the initial window
	resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
 
	// Call our control loop - this the main loop that keeps on running until user quits program with Esc or by closing the display window
	controlLoop(videoFlags, event);
 
	// Clean ourselves up and exit
	quit(0);
 
	// We will never get here because our quit function will exit or us - but we'll put this return in for the sake of correctness!
	return 0;
}