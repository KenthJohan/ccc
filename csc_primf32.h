#pragma once




static void primf32_make_rectangle (float v[], unsigned stride, float x, float y, float w, float h)
{
	v[0] = x + 0; v[1] = y + h; v += stride;
	v[0] = x + w; v[1] = y + 0; v += stride;
	v[0] = x + 0; v[1] = y + 0; v += stride;
	v[0] = x + w; v[1] = y + 0; v += stride;
	v[0] = x + 0; v[1] = y + h; v += stride;
	v[0] = x + w; v[1] = y + h; v += stride;
}


static void primf32_make_rectangle4_xywh (float v[], unsigned stride, float x, float y, float w, float h, float v2, float v3)
{
	v[0] = x + 0; v[1] = y + h;	v[2] = v2; v[3] = v3; v += stride;
	v[0] = x + w; v[1] = y + 0;	v[2] = v2; v[3] = v3; v += stride;
	v[0] = x + 0; v[1] = y + 0;	v[2] = v2; v[3] = v3; v += stride;
	v[0] = x + w; v[1] = y + 0;	v[2] = v2; v[3] = v3; v += stride;
	v[0] = x + 0; v[1] = y + h;	v[2] = v2; v[3] = v3; v += stride;
	v[0] = x + w; v[1] = y + h;	v[2] = v2; v[3] = v3; v += stride;
}







static void primf32_make6_rectangle4_ab (float v[], unsigned stride, float ax, float ay, float bx, float by, float v2, float v3)
{
	v[0] = ax; v[1] = by;	v[2] = v2; v[3] = v3; v += stride;
	v[0] = bx; v[1] = ay;	v[2] = v2; v[3] = v3; v += stride;
	v[0] = ax; v[1] = ay;	v[2] = v2; v[3] = v3; v += stride;
	v[0] = bx; v[1] = ay;	v[2] = v2; v[3] = v3; v += stride;
	v[0] = ax; v[1] = by;	v[2] = v2; v[3] = v3; v += stride;
	v[0] = bx; v[1] = by;	v[2] = v2; v[3] = v3; v += stride;
}
