#include <iostream>
#include "Voltex.h"
#include "MD.h"

int main() {
	{
		std::unique_ptr<MD> md = std::make_unique<MD>();
		md->Run();
	}
	
	return 0;
}