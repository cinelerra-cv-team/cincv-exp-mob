
/*
 * CINELERRA
 * Copyright (C) 2008 Adam Williams <broadcast at earthling dot net>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * Created by Keith Gudger 10/21/09 from flash.C
 */

/** \file dip2black.C
 *  \brief code for Dip2Black, a transition that "dips to black!"
 *
 *  Derived from PluginVClient, based on file flash.h
 */

#include "dip2black.h"
#include "edl.inc"
#include "overlayframe.h"
#include "language.h"
#include "picon_png.h"
#include "vframe.h"
#include <stdint.h>
#include <math.h>


PluginClient* new_plugin(PluginServer *server)
{
	return new Dip2BlackMain(server);
}





Dip2BlackMain::Dip2BlackMain(PluginServer *server)
 : PluginVClient(server)
{
}

Dip2BlackMain::~Dip2BlackMain()
{
}

char* Dip2BlackMain::plugin_title() { return N_("Dip2Black"); }
int Dip2BlackMain::is_video() { return 1; }
int Dip2BlackMain::is_transition() { return 1; }
int Dip2BlackMain::uses_gui() { return 0; }

NEW_PICON_MACRO(Dip2BlackMain)

/** \def DIP2BLACK(type, temp_type, components, max, chroma_offset) 
 *  slowly dips input stream to black, based on \a fraction.  
 *  When fraction is "1", output should be completely black.
 *  After reaching one, macro restores output stream to normal brightness.
 *  \param type is the type of the date in the input / output streams
 *  \param temp_type is supposed to be a larger type to get around
 *  "signedness" of the incoming type
 *  \param components tells us whether there are 3 or 4 parts, in other
 *  words whether there is an alpha layer (the fourth part).
 *  \param max is the maximum integer for the type.
 *  \param chroma_offset tells us the middle of the chroma field
 *  for the YUV color models.  
 *  \note to make the 3 element color model match the alpha channel model,
 *  the square root of the \a fraction is used.
 */
#define DIP2BLACK(type, temp_type, components, max, chroma_offset) \
{ \
	float round_factor = (sizeof(type) < 4) ? 0.5 : 0; \
	if(components<4) fraction = sqrt(fraction); \
	temp_type foreground = (temp_type)(fraction * max + round_factor); \
	temp_type transparency = max - foreground ; \
	temp_type chroma_foreground = 0 ; \
	if(chroma_offset) chroma_foreground = foreground * chroma_offset /max ;\
 \
	for(int i = 0; i < h; i++) \
	{ \
		type *in_row = (type*)incoming->get_rows()[i]; \
		type *out_row = (type*)outgoing->get_rows()[i]; \
		if(is_before) \
		{ \
			for(int j = 0; j < w; j++) \
			{ \
				*out_row = transparency * *out_row / max ; \
				out_row++; \
				*out_row = chroma_foreground + transparency * *out_row /max ; \
				out_row++; \
				*out_row = chroma_foreground + transparency * *out_row /max ; \
				out_row++; \
				if(components == 4) \
				{ \
					*out_row = transparency * *out_row /max; \
					out_row++; \
				} \
			} \
		} \
		else \
		{ \
			for(int j = 0; j < w; j++) \
			{ \
				*out_row = transparency * *in_row /max ; \
				out_row++; \
				in_row++; \
				*out_row = chroma_foreground + transparency * *in_row /max ; \
				out_row++; \
				in_row++; \
				*out_row = chroma_foreground + transparency * *in_row /max ; \
				out_row++; \
				in_row++; \
				if(components == 4) \
				{ \
					*out_row = transparency * *in_row /max; \
					out_row++; \
					in_row++; \
				} \
			} \
		} \
	} \
}

int Dip2BlackMain::process_realtime(VFrame *incoming, VFrame *outgoing)
{
	int half = PluginClient::get_total_len() / 2;
	int position = half - labs(PluginClient::get_source_position() - half);
	float fraction = (float)position / half; // how close to black
	int is_before = PluginClient::get_source_position() < half;
	int w = incoming->get_w();
	int h = incoming->get_h();

	switch(incoming->get_color_model())
	{
		case BC_RGB888:
			DIP2BLACK(unsigned char, int, 3, 0xff, 0x0);
			break;
		case BC_RGB_FLOAT:
			DIP2BLACK(float, float, 3, 1.0, 0x0);
			break;
		case BC_RGBA8888:
			DIP2BLACK(unsigned char, int, 4, 0xff, 0x0);
			break;
		case BC_RGBA_FLOAT:
			DIP2BLACK(float, float, 4, 1.0, 0x0);
			break;
		case BC_YUV888:
			DIP2BLACK(unsigned char, int, 3, 0xff, 0x80);
			break;
		case BC_YUVA8888:
			DIP2BLACK(unsigned char, int, 4, 0xff, 0x80);
			break;
		case BC_RGB161616:
			DIP2BLACK(uint16_t, int, 3, 0xffff, 0x0);
			break;
		case BC_RGBA16161616:
			DIP2BLACK(uint16_t, int, 4, 0xffff, 0x0);
			break;
		case BC_YUV161616:
			DIP2BLACK(uint16_t, int, 3, 0xffff, 0x8000);
			break;
		case BC_YUVA16161616:
			DIP2BLACK(uint16_t, int, 4, 0xffff, 0x8000);
			break;
	}

	return 0;
}
