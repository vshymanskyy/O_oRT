#pragma once

#undef assert
#define assert(x)

struct rgba {

	channel r, g, b, a;

	rgba() {}

	rgba (const rgba &c)
		: r(c.r), g(c.g), b(c.b), a(c.a) {}

	rgba (const channel R, const channel G, const channel B, const channel A = 1.0f)
		: r(R), g(G), b(B), a(A)
	{
		assert(WellFormed());
	}

	rgba (const channel& Gray)
		: r(Gray), g(Gray), b(Gray), a(1.0f)
	{
		assert(WellFormed());
	}

	rgba (const channel& Gray, const channel& A)
		: r(Gray), g(Gray), b(Gray), a(A)
	{
		assert(WellFormed());
	}


	rgba operator =(const rgba &c) {
		a=c.a;
		r=c.r;
		g=c.g;
		b=c.b;
		return *this;
	}

	rgba operator =(const channel &v) {
		a=1.0f;
		r=v;
		g=v;
		b=v;
		return *this;
	}

	//bool operator ==(const rgba &c)const{ return (r==c.r)&&(g==c.g)&&(b==c.b); }
	//bool operator !=(const rgba &c)const{ return (r!=c.r)||(g!=c.g)||(b!=c.b); }

	rgba& operator +=(const rgba& c) { r+=c.r;g+=c.g;b+=c.b; assert(WellFormed()); return *this;}
	rgba& operator -=(const rgba& c) { r-=c.r;g-=c.g;b-=c.b; assert(WellFormed()); return *this;}
	rgba& operator *=(const rgba& c) { r*=c.r;g*=c.g;b*=c.b; assert(WellFormed()); return *this;}
	rgba& operator /=(const rgba& c) { r/=c.r;g/=c.g;b/=c.b; assert(WellFormed()); return *this;}
	rgba operator +(const rgba& c)const{ return rgba(r+c.r,g+c.g,b+c.b); }
	rgba operator -(const rgba& c)const{ return rgba(r-c.r,g-c.g,b-c.b); }
	rgba operator *(const rgba& c)const{ return rgba(r*c.r,g*c.g,b*c.b); }
	rgba operator /(const rgba& c)const{ return rgba(r/c.r,g/c.g,b/c.b); }
	rgba& operator +=(const channel& c) { r+=c;g+=c;b+=c; assert(WellFormed()); return *this;}
	rgba& operator -=(const channel& c) { r-=c;g-=c;b-=c; assert(WellFormed()); return *this;}
	rgba& operator *=(const channel& c) { r*=c;g*=c;b*=c; assert(WellFormed()); return *this;}
	rgba& operator /=(const channel& c) { r/=c;g/=c;b/=c; assert(WellFormed()); return *this;}
	rgba operator +(const channel& c)const{ return rgba(r+c,g+c,b+c); }
	rgba operator -(const channel& c)const{ return rgba(r-c,g-c,b-c); }
	rgba operator *(const channel& c)const{ return rgba(r*c,g*c,b*c); }
	rgba operator /(const channel& c)const{ return rgba(r/c,g/c,b/c); }

	bool WellFormed();
	rgba Abs() const;
	rgba Trim() const;
	float GetMax() const;
	rgba Blended(const rgba &c, channel alpha) const;

	channel Luminance() const{
		return
			r * 0.2126f
			+ g * 0.715160f
			+ b * 0.072169f;
	}

	rgba& Blend(const rgba &c, channel alpha) {
		*this += (c - *this)*alpha;
		return *this;
	}

	rgba& Blend(const rgba &c, const rgba &alpha) {
		*this += (c - *this)*alpha;
		return *this;
	}

	rgba& Exp() {
		r = exp(r);
		g = exp(g);
		b = exp(b);
		return *this;
	}

	rgba& Opposite() {
		r = 1.0f - r;
		g = 1.0f - g;
		b = 1.0f - b;
		return *this;
	}

// 	rgba correct_exposure(const float aperture);
// 	rgba correct_gamma(const float gamma);

};
