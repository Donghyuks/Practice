#pragma once

// ������Ʈ Ʋ.
class Component
{
public:
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
};

