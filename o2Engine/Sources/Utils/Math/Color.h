#pragma once

#include "Utils/CommonTypes.h"
#include "Utils/Math/Math.h"

namespace o2
{
	class Color4
	{
	public:
		int r, g, b, a;

		inline Color4();
		inline Color4(int r, int g, int b, int a = 255);
		inline Color4(float r, float g, float b, float a = 1.0f);
		
		inline bool operator==(const Color4& color) const;
		inline bool operator!=(const Color4& color) const;

		inline Color4 operator+(const Color4& v) const;
		inline Color4 operator+=(const Color4& v);

		inline Color4 operator-(const Color4& v) const;
		inline Color4 operator-=(const Color4& v);

		inline Color4 operator*(float v) const;
		inline Color4 operator*=(float v);

		inline Color4 operator/(float v) const;
		inline Color4 operator/=(float v);

		inline Color4 operator*(const Color4& v) const;
		inline Color4 operator*=(const Color4& v);

		inline Color4 operator/(const Color4& v) const;
		inline Color4 operator/=(const Color4& v);

		inline float RF() const;
		inline float GF() const;
		inline float BF() const;
		inline float AF() const;

		inline void SetRF(float value);
		inline void SetGF(float value);
		inline void SetBF(float value);
		inline void SetAF(float value);

		inline float GetF(int i) const;
		inline int&  Get(int i) const;

		inline void   Normalize();
		inline Color4 Normalized() const;

		inline ULong ARGB() const;
		inline ULong ABGR() const;
		inline ULong RGBA() const;

		inline void SetARGB(ULong color);
		inline void SetABGR(ULong color);
		inline void SetRGBA(ULong color);

		inline void SetHSL(float hue, float saturation, float lightness);
		inline void ToHSL(float& hue, float& saturation, float& lightness);

		inline Color4 BlendByAlpha(const Color4& other);

		inline static Color4 White();
		inline static Color4 Black();
		inline static Color4 Red();
		inline static Color4 Blue();
		inline static Color4 Green();
		inline static Color4 Gray();
		inline static Color4 SomeColor(int idx);
	};
	
	Color4::Color4():
		r(255), g(255), b(255), a(255)
	{
	}

	Color4::Color4(int r, int g, int b, int a):
		r(r), g(g), b(b), a(a)
	{
	}

	Color4::Color4(float r, float g, float b, float a)
	{
		this->r = (int)(r*255.0f);
		this->g = (int)(g*255.0f);
		this->b = (int)(b*255.0f);
		this->a = (int)(a*255.0f);
	}

	bool Color4::operator==(const Color4& color) const
	{
		return a == color.a && r == color.r && g == color.g && b == color.b;
	}

	bool Color4::operator!=(const Color4& color) const
	{
		return a != color.a || r != color.r || g != color.g || b != color.b;
	}

	Color4 Color4::operator+(const Color4& v) const
	{
		return Color4(r + v.r, g + v.g, b + v.b, a + v.a);
	}

	Color4 Color4::operator+=(const Color4& v)
	{
		r += v.r; g += v.g; b += v.b; a += v.a;
		return *this;
	}

	Color4 Color4::operator-(const Color4& v) const
	{
		return Color4(r - v.r, g - v.g, b - v.b, a - v.a);
	}

	Color4 Color4::operator-=(const Color4& v)
	{
		r -= v.r; g -= v.g; b -= v.b; a -= v.a;
		return *this;
	}

	Color4 Color4::operator*(float v) const
	{
		return Color4((int)((float)r*v), (int)((float)g*v), (int)((float)b*v), (int)((float)a*v));
	}

	Color4 Color4::operator*=(float v)
	{
		*this = (*this)*v;
		return *this;
	}

	Color4 Color4::operator/(float v) const
	{
		return *this*(1.0f/v);
	}

	Color4 Color4::operator/=(float v)
	{
		*this = *this/v;
		return *this;
	}

	Color4 Color4::operator*(const Color4& v) const
	{
		return Color4(r*v.r/255, g*v.g/255, b*v.b/255, a*v.a/255);
	}

	Color4 Color4::operator/(const Color4& v) const
	{
		return Color4((int)(255.0f*(RF()/v.RF())),
					  (int)(255.0f*(GF()/v.GF())),
					  (int)(255.0f*(BF()/v.BF())),
					  (int)(255.0f*(AF()/v.AF())));
	}

	Color4 Color4::operator/=(const Color4& v)
	{
		*this = *this/v;
		return *this;
	}

	Color4 Color4::operator*=(const Color4& v)
	{
		*this = *this*v;
		return *this;
	}

	float Color4::RF() const
	{
		return (float)r/255.0f;
	}

	float Color4::GF() const
	{
		return (float)g/255.0f;
	}

	float Color4::BF() const
	{
		return (float)b/255.0f;
	}

	float Color4::AF() const
	{
		return (float)a/255.0f;
	}

	void Color4::SetRF(float value)
	{
		r = (int)(value*255.0f);
	}

	void Color4::SetGF(float value)
	{
		g = (int)(value*255.0f);
	}

	void Color4::SetBF(float value)
	{
		b = (int)(value*255.0f);
	}

	void Color4::SetAF(float value)
	{
		a = (int)(value*255.0f);
	}

	float Color4::GetF(int i) const
	{
		return (float)(*(int*)(this + sizeof(int)*i))/255.0f;
	}

	int& Color4::Get(int i) const
	{
		return (*(int*)(this + sizeof(int)*i));
	}

	void Color4::Normalize()
	{
		r = Math::Clamp(r, 0, 255);
		g = Math::Clamp(g, 0, 255);
		b = Math::Clamp(b, 0, 255);
		a = Math::Clamp(a, 0, 255);
	}

	Color4 Color4::Normalized() const
	{
		return Color4(Math::Clamp(r, 0, 255),
					  Math::Clamp(g, 0, 255),
					  Math::Clamp(b, 0, 255),
					  Math::Clamp(a, 0, 255));
	}

	ULong Color4::ARGB() const
	{
		return (ULong)((a << 24) | (r << 16) | (g << 8) | b);
	}

	ULong Color4::ABGR() const
	{
		return (ULong)((a << 24) | (b << 16) | (g << 8) | r);
	}

	ULong Color4::RGBA() const
	{
		return (ULong)((r << 24) | (g << 16) | (b << 8) | a);
	}

	void Color4::SetARGB(ULong color)
	{
		a = (int)(color >> 24);
		r = (int)((color << 8) >> 24);
		g = (int)((color << 16) >> 24);
		b = (int)((color << 24) >> 24);
	}

	void Color4::SetABGR(ULong color)
	{
		a = (int)(color >> 24);
		b = (int)((color << 8) >> 24);
		g = (int)((color << 16) >> 24);
		r = (int)((color << 24) >> 24);
	}

	void Color4::SetRGBA(ULong color)
	{
		r = (int)(color >> 24);
		g = (int)((color << 8) >> 24);
		b = (int)((color << 16) >> 24);
		a = (int)((color << 24) >> 24);
	}

	void Color4::SetHSL(float hue, float saturation, float lightness)
	{
		if (saturation == 0)
		{
			r = (int)(lightness*255.0f);
			g = (int)(lightness*255.0f);
			b = (int)(lightness*255.0f);
		}
		else
		{
			auto hue2rgb = [](float p, float q, float t)
			{
				if (t < 0.0f) t += 1.0f;
				if (t > 1.0f) t -= 1.0f;
				if (t < 1.0f/6.0f) return p + (q - p)*6.0f * t;
				if (t < 1.0f/2.0f) return q;
				if (t < 2.0f/3.0f) return p + (q - p)*(2.0f/3.0f - t)*6.0f;
				return p;
			};

			float q = lightness < 0.5f ? lightness*(1.0f + saturation) : lightness + saturation - lightness*saturation;
			float p = 2.0f*lightness - q;

			r = (int)(hue2rgb(p, q, hue + 1.0f/3.0f)*255.0f);
			g = (int)(hue2rgb(p, q, hue)*255.0f);
			b = (int)(hue2rgb(p, q, hue - 1.0f/3.0f)*255.0f);
		}
	}

	void Color4::ToHSL(float& hue, float& saturation, float& lightness)
	{
		float r = this->r/255.0f;
		float g = this->g/255.0f;
		float b = this->b/255.0f;

		float max = Math::Max(r, Math::Max(g, b)), min = Math::Min(r, Math::Min(g, b));
		lightness = (max + min)*0.5f;

		if (Math::Equals(max, min))
			hue = saturation = 0;
		else
		{
			float d = max - min;
			saturation = lightness > 0.5f ? d / (2.0f - max - min) : d / (max + min);

			if (Math::Equals(max, r))
				hue = (g - b) / d + (g < b ? 6 : 0); 
			else if (Math::Equals(max, g))
				hue = (b - r) / d + 2; 
			else if (Math::Equals(max, b))
				hue = (r - g) / d + 4; 

			hue /= 6.0f;
		}
	}

	Color4 Color4::BlendByAlpha(const Color4& other)
	{
		float a1 = AF(), a2 = other.AF();
		float inva1 = 1.0f - a1;

		return Color4(int(a1*(float)r + a2*inva1*(float)other.r),
					  int(a1*(float)g + a2*inva1*(float)other.g),
					  int(a1*(float)b + a2*inva1*(float)other.b),
					  int(255.0f*(a1 + a2*inva1)));
	}

	Color4 Color4::White()
	{
		return Color4(255, 255, 255, 255);
	}

	Color4 Color4::Black()
	{
		return Color4(0, 0, 0, 255);
	}

	Color4 Color4::Red()
	{
		return Color4(255, 0, 0, 255);
	}

	Color4 Color4::Blue()
	{
		return Color4(0, 0, 255, 255);
	}

	Color4 Color4::Green()
	{
		return Color4(0, 255, 0, 255);
	}

	Color4 Color4::Gray()
	{
		return Color4(150, 150, 150, 255);
	}

	Color4 Color4::SomeColor(int idx)
	{
		return Color4((idx*33)%255, (idx*58)%255, (idx*77)%255, 255);
	}
}
 