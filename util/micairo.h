#ifndef MICAIRO_INCLUDED
#define MICAIRO_H_INCLUDED

#include "miauto.h"

#include <cairo.h>
#include <vector>

struct CairoSave
{
    cairo_t *ptr;
    CairoSave(cairo_t *cr)
        :ptr(cr)
    { cairo_save(ptr); }
    ~CairoSave()
    { cairo_restore(ptr); }
};

struct CairoSaveTransform
{
    cairo_t *ptr;
	cairo_matrix_t mtx;

    CairoSaveTransform(cairo_t *cr)
        :ptr(cr)
    { cairo_get_matrix(ptr, &mtx); }
    ~CairoSaveTransform()
    { cairo_set_matrix(ptr, &mtx); }
};

struct CairoPath
{
	cairo_path_t path;
	std::vector<cairo_path_data_t> data;

	CairoPath()
	{
		path.status = CAIRO_STATUS_SUCCESS;
		path.data = NULL;
		path.num_data = 0;
	}
	void reset()
	{
		path.data = NULL;
		path.num_data = 0;
		data.clear();
	}
	operator cairo_path_t *()
	{
		if (data.empty())
		{
			path.data = NULL;
			path.num_data = 0;
		}
		else
		{
			path.data = &data[0];
			path.num_data = data.size();
		}
		return &path;
	}
	void move_to(double x, double y)
	{
		size_t cur = data.size(); 
		data.resize(cur + 2);
		cairo_path_data_t &h = data[cur];
		h.header.type = CAIRO_PATH_MOVE_TO;
		h.header.length = 2;
		cairo_path_data_t &p = data[cur+1];
		p.point.x = x;
		p.point.y = y;
	}
	void line_to(double x, double y)
	{
		size_t cur = data.size(); 
		data.resize(cur + 2);
		cairo_path_data_t &h = data[cur];
		h.header.type = CAIRO_PATH_LINE_TO;
		h.header.length = 2;
		cairo_path_data_t &p = data[cur+1];
		p.point.x = x;
		p.point.y = y;
	}
	void curve_to(double x1, double y1, double x2, double y2, double x3, double y3)
	{
		size_t cur = data.size(); 
		data.resize(cur + 4);
		cairo_path_data_t &h = data[cur];
		h.header.type = CAIRO_PATH_CURVE_TO;
		h.header.length = 4;
		cairo_path_data_t &p1 = data[cur+1];
		p1.point.x = x1;
		p1.point.y = y1;
		cairo_path_data_t &p2 = data[cur+2];
		p2.point.x = x2;
		p2.point.y = y2;
		cairo_path_data_t &p3 = data[cur+3];
		p3.point.x = x3;
		p3.point.y = y3;
	}
	void close_path()
	{
		size_t cur = data.size(); 
		data.resize(cur + 2);
		cairo_path_data_t &h = data[cur];
		h.header.type = CAIRO_PATH_CLOSE_PATH;
		h.header.length = 1;
	}
};

struct CairoTraits
{
    typedef cairo_t *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void AddRef(type obj)
    { cairo_reference(obj); }
    static void Delete(type obj)
    { cairo_destroy(obj); }
};
typedef miutil::AutoObject<CairoTraits> CairoPtr;

struct CairoSurfaceTraits
{
    typedef cairo_surface_t *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void AddRef(type obj)
    { cairo_surface_reference(obj); }
    static void Delete(type obj)
    { cairo_surface_destroy(obj); }
};
typedef miutil::AutoObject<CairoSurfaceTraits> CairoSurfacePtr;

struct CairoPatternTraits
{
    typedef cairo_pattern_t *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void AddRef(type obj)
    { cairo_pattern_reference(obj); }
    static void Delete(type obj)
    { cairo_pattern_destroy(obj); }
};
typedef miutil::AutoObject<CairoPatternTraits> CairoPatternPtr;
   
struct CairoFontFaceTraits
{
    typedef cairo_font_face_t *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void AddRef(type obj)
    { cairo_font_face_reference(obj); }
    static void Delete(type obj)
    { cairo_font_face_destroy(obj); }
};
typedef miutil::AutoObject<CairoFontFaceTraits> CairoFontFacePtr;

struct CairoFontOptionsTraits
{
    typedef cairo_font_options_t *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void Delete(type obj)
    { cairo_font_options_destroy(obj); }
};
typedef miutil::AutoObject<CairoFontOptionsTraits> CairoFontOptionsPtr;

struct CairoPathTraits
{
    typedef cairo_path_t *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void Delete(type obj)
    { cairo_path_destroy(obj); }
};
typedef miutil::AutoObject<CairoPathTraits> CairoPathPtr;

#endif //MICAIRO_H_INCLUDED
