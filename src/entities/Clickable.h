#pragma once
#include "GameObject.h"
#include <functional>
#include <vector>

class Clickable : public GameObject
{
public:

	void AddCallBack(std::function<void()> f) {
		callbacks.push_back(f);
	}

	void ResetCallbacks() {
		callbacks.clear();
	}

	void OnClick(){
		for (auto f : callbacks) {
			f();
		}
	}

private:
	std::vector<std::function<void()>> callbacks;

};

