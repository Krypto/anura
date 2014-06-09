/*
	Copyright (C) 2003-2013 by Kristina Simpson <sweet.kristas@gmail.com>
	
	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	   1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgement in the product documentation would be
	   appreciated but is not required.

	   2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	   3. This notice may not be removed or altered from any source
	   distribution.
*/

#pragma once

#include <stack>

#include "../Color.hpp"
#include "Geometry.hpp"
#include "Material.hpp"
#include "Util.hpp"
#include "VGraph.hpp"

namespace KRE
{
	class Canvas;
	typedef std::shared_ptr<Canvas> CanvasPtr;

	// A 2D canvas class for drawing on. Not in the renderable pipelines.
	// Canvas writes are done in the order in the code.
	// Intended for making things like UI's.
	class Canvas
	{
	public:
		virtual ~Canvas();

		void setDimensions(unsigned w, unsigned h);

		unsigned width() const { return width_; }
		unsigned height() const { return height_; }

		// Blit's a texture from co-ordinates given in src to the screen co-ordinates dst
		virtual void blitTexture(const TexturePtr& tex, const rect& src, float rotation, const rect& dst, const ColorPtr& color=nullptr) const = 0;
		// Blit a texture to the given co-ordinates on the display. Assumes the whole texture is being used.
		void blitTexture(const TexturePtr& tex, float rotation, const rect& dst, const ColorPtr& color=nullptr) const;

		// Blit's a material from internal co-ordinates to destination screen co-ordinates.
		virtual void blitTexture(const MaterialPtr& mat, float rotation, const rect& dst, const ColorPtr& color=nullptr) const = 0;
		virtual void blitTexture(const MaterialPtr& mat, const rect& src, float rotation, const rect& dst, const ColorPtr& color=nullptr) const = 0;

		virtual void drawRect(const rect& r, const ColorPtr& fill_color, const ColorPtr& stroke_color=nullptr, float rotate=0) const = 0;
		virtual void drawLine(const point& p1, const point& p2, const Color& color) const = 0;
		virtual void drawCircle(const point& centre, double radius, const Color& color) const = 0;

		void drawVectorContext(const Vector::ContextPtr& context);

		static CanvasPtr getInstance();

		struct ColorManager
		{
			ColorManager(CanvasPtr& canvas, const ColorPtr& color) : canvas_(canvas) {
				canvas->color_stack_.push(color);
			}
			~ColorManager() {
				canvas_->color_stack_.pop();
			}
			CanvasPtr& canvas_;
		};

		ColorPtr getColor() const {
			if(color_stack_.empty()) {
				return ColorPtr();
			}
			return color_stack_.top();
		}

	protected:
		Canvas();
	private:
		DISALLOW_COPY_AND_ASSIGN(Canvas);
		unsigned width_;
		unsigned height_;
		virtual void handleDimensionsChanged() = 0;
		std::stack<ColorPtr> color_stack_;
	};
}
