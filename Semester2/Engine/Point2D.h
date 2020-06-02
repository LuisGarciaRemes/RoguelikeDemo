#pragma once
class Point2D
{
public:
	Point2D()
	{
		m_x = 0.0f;
		m_y = 0.0f;
	}
	Point2D(float i_x, float i_y)
	{
		m_x = i_x;
		m_y = i_y;
	}

	~Point2D()
	{
	}

	//get
	inline float x() const { return m_x; }
	inline float y() const { return m_y; }


	//set
	inline void x(float i_x) { m_x = i_x; }
	inline void y(float i_y) { m_y = i_y; }

	inline const Point2D operator+ (const Point2D & i_other) const
	{
		return Point2D(m_x + i_other.m_x, m_y + i_other.m_y);
	}

	inline const Point2D operator* (const float & i_other) const
	{
		return Point2D(m_x * i_other, m_y * i_other);
	}

	inline const Point2D operator/ (const float & i_other) const
	{
		return Point2D(m_x / i_other, m_y / i_other);
	}

	inline const Point2D operator- (const Point2D & i_other) const
	{
		return Point2D(m_x - i_other.m_x, m_y - i_other.m_y);
	}

	inline void operator-= (const Point2D & i_other)
	{
		m_x = m_x - i_other.m_x , m_y = m_y - i_other.m_y;
	}
	inline void operator+= (const Point2D & i_other)
	{
		m_x = m_x + i_other.m_x , m_y = m_y + i_other.m_y;
	}
	inline const bool operator== (const Point2D & i_other)
	{
		return (m_x == i_other.m_x && m_y == i_other.m_y) ? true : false;
	}

private:
	float m_x, m_y;		
};

