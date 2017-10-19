#ifndef BACKDROP_H_
#define BACKDROP_H_

#include <string>
#include "Graphics.h"

struct Backdrop
{
	virtual void draw(Graphics &graphics) const = 0;
};

struct FixedBackdrop : public Backdrop
{
	FixedBackdrop(const std::string &path, Graphics &graphics);
	void draw(Graphics &graphics) const;

private:

	Graphics::SurfaceID surfaceID_;
};

#endif