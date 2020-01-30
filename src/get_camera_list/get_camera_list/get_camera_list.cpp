#include <iostream>
#include <ryulib/CameraList.hpp>

int main()
{
	CameraList cameralist;
	cameralist.update();
	printf("cameralist.size(): %d \n", cameralist.size());
	for (int i = 0; i < cameralist.size(); i++) {
		printf("%d: %s \n", i, cameralist.getName(i).c_str());
	}

	return 0;
}
