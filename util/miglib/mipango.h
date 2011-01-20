#ifndef MIPANGO_INCLUDED
#define MIPANGO_H_INCLUDED

#include <glib-object.h>
#include <pango/pango.h>
#include "miauto.h"

#define PANGO_PIXELSD(d) ((d) / double(PANGO_SCALE))



struct PangoContextTraits
{
    typedef PangoContext *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void AddRef(type obj)
    { g_object_ref(obj); }
    static void Delete(type obj)
    { g_object_unref(obj); }
};
typedef miutil::AutoObject<PangoContextTraits> PangoContextPtr;

struct PangoLayoutTraits
{
    typedef PangoLayout *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void AddRef(type obj)
    { g_object_ref(obj); }
    static void Delete(type obj)
    { g_object_unref(obj); }
};
typedef miutil::AutoObject<PangoLayoutTraits> PangoLayoutPtr;

struct PangoLayoutLineTraits
{
    typedef PangoLayoutLine *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void AddRef(type obj)
    { pango_layout_line_ref(obj); }
    static void Delete(type obj)
    { pango_layout_line_unref(obj); }
};
typedef miutil::AutoObject<PangoLayoutLineTraits> PangoLayoutLinePtr;

struct PangoLayoutIterTraits
{
    typedef PangoLayoutIter *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void Delete(type obj)
    { pango_layout_iter_free(obj); }
};
typedef miutil::AutoObject<PangoLayoutIterTraits> PangoLayoutIterPtr;

struct PangoFontDescriptionTraits
{
    typedef PangoFontDescription *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void Delete(type obj)
    { pango_font_description_free(obj); }
};
typedef miutil::AutoObject<PangoFontDescriptionTraits> PangoFontDescriptionPtr;

struct PangoAttrListTraits
{
    typedef PangoAttrList *type;

    static type Null()
    { return NULL; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void AddRef(type obj)
    { pango_attr_list_ref(obj); }
    static void Delete(type obj)
    { pango_attr_list_unref(obj); }
};
typedef miutil::AutoObject<PangoAttrListTraits> PangoAttrListPtr;

struct PangoAttributeTraits
{
    typedef PangoAttribute *type;

    static type Null()
    { return NULL; }
	static type Arrow(type obj)
	{ return obj; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void Delete(type obj)
    { pango_attribute_destroy(obj); }
};
typedef miutil::AutoObject<PangoAttributeTraits> PangoAttributePtr;

struct PangoItemTraits
{
    typedef PangoItem *type;

    static type Null()
    { return NULL; }
	static type Arrow(type obj)
	{ return obj; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void Delete(type obj)
    { pango_item_free(obj); }
};
typedef miutil::AutoObject<PangoItemTraits> PangoItemPtr;

struct PangoGlyphStringTraits
{
    typedef PangoGlyphString *type;

    static type Null()
    { return NULL; }
	static type Arrow(type obj)
	{ return obj; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void Delete(type obj)
    { pango_glyph_string_free(obj); }
};
typedef miutil::AutoObject<PangoGlyphStringTraits> PangoGlyphStringPtr;

struct PangoFontMetricsTraits
{
    typedef PangoFontMetrics *type;

    static type Null()
    { return NULL; }
	static type Arrow(type obj)
	{ return obj; }
    static bool IsValid(type obj)
    { return obj!=NULL; }
    static void AddRef(type obj)
    { pango_font_metrics_ref(obj); }
    static void Delete(type obj)
    { pango_font_metrics_unref(obj); }
};
typedef miutil::AutoObject<PangoFontMetricsTraits> PangoFontMetricsPtr;

#endif //MIPANGO_H_INCLUDED
