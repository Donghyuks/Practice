#pragma once

template<class T>
class Vector2
{
private:
	T x, y;

public:
	Vector2(T _x = 0.0f, T _y = 0.0f);
	~Vector2();

	float getMagnitude();
	void Normalize();
	void Reverse();
	T GetXVector();
	T GetYVector();

	Vector2<T> getNormalize();
	Vector2<T> Multiple(float _Mul);
	Vector2<T> operator+=(Vector2<T>& _vec2);
	Vector2<T> operator+(Vector2<T>& _vec2);
	float Dot(Vector2<T> _Target);
};

template<class T>
Vector2<T> Vector2<T>::Multiple(float _Mul)
{
	this->x = this->x * _Mul;
	this->y = this->y * _Mul;

	return *this;
}

template<class T>
Vector2<T> Vector2<T>::operator+(Vector2<T>& _vec2)
{
	Vector2<T> sum;
	sum.x = x + _vec2.x;
	sum.y = y + _vec2.y;

	return sum;
}

template<class T>
Vector2<T> Vector2<T>::operator+=(Vector2<T>& _vec2)
{
	this->x += _vec2.x;
	this->y += _vec2.y;

	return *this;
}

template<class T>
T Vector2<T>::GetYVector()
{
	return y;
}

template<class T>
T Vector2<T>::GetXVector()
{
	return x;
}

template<class T>
void Vector2<T>::Reverse()
{
	x = -x;
	y = -y;
}

template<class T>
float Vector2<T>::Dot(Vector2<T> _Target)
{
	return x * _Target.x + y * _Target.y;
}

template<class T>
Vector2<T> Vector2<T>::getNormalize()
{
	Normalize();
	return Vector2(x, y);
}

template<class T>
void Vector2<T>::Normalize()
{
	double mag = getMagnitude();
	if (mag == 0)
	{
		x = 0;
		y = 0;
	}
	else
	{
		x /= mag;
		y /= mag;
	}
}

template<class T>
float Vector2<T>::getMagnitude()
{
	return sqrt((double)(x * x) + (double)(y * y));
}

template<class T>
Vector2<T>::~Vector2()
{

}

template<class T>
Vector2<T>::Vector2(T _x, T _y)
{
	x = _x;
	y = _y;
}
