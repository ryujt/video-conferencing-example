#pragma once

#include <ryulib/base.hpp>

using namespace ryulib;

class VideoUnit {
public:
	void start()
	{

	}

	void stop()
	{

	}

	void play(const void* data, int size)
	{

	}

	void setOnData(DataEvent event) { onData = event; }

private:
	DataEvent onData = nullptr;
};