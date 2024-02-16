#pragma once

class Component
{
protected:

public:
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
};