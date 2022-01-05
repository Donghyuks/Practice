#pragma once

// 컴포넌트 틀.
class Component
{
public:
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
};

