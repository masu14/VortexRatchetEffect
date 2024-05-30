#include <iostream>
#include "Voltex.h"
#include "MD.h"

int main() {
	MD *md = new MD();
	md->Run();
	delete md;
	return 0;
}