#include <iostream>
#include <ryulib/CameraList.hpp>

int main()
{
	CameraList cameralist;
	cameralist.update();
	for (int i = 0; i < cameralist.size(); i++) {
		printf("%d: %s", i, cameralist.getName(i).c_str());
	}

	return 0;
}
