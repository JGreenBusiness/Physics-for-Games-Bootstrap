#include "EightBallPool.h"
#include "PhysicsApp.h"

int main() {
	
	// allocation

	auto app = new EightBallPool();//new PhysicsApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}