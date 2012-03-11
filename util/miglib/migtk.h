#ifndef MIGTK_H_INCLUDED
#define MIGTK_H_INCLUDED

#include "miglib.h"
#include <gtk/gtk.h>

namespace miglib
{

//Actions
#define MIGTK_ACTION(cls, func) MIGLIB_SIGNAL_FUNC_1(void, GtkAction*, cls, func)
#define MIGTK_SACTION(func) MIGLIB_SIGNAL_SFUNC_1(void, GtkAction*, func)

//Events
#define MIGLIB_CONNECT_EVENT_EX(src, evt, tevt, cls, func, ptr) \
    MIGLIB_CONNECT_2(src, evt, gboolean, GtkWidget*, tevt*, cls, func, ptr)
#define MIGLIB_SCONNECT_EVENT_EX(src, evt, tevt, func, ptr) \
    MIGLIB_SCONNECT_2(src, evt, gboolean, GtkWidget*, tevt*, func, ptr)

#define MIGLIB_CONNECT_EVENT(src, evt, cls, func, ptr) \
    MIGLIB_CONNECT_EVENT_EX(src, evt, GdkEvent, cls, func, ptr)
#define MIGLIB_SCONNECT_EVENT(src, evt, func, ptr) \
    MIGLIB_SCONNECT_EVENT_EX(src, evt, GdkEvent, func, ptr)

struct MiColor
{
    GdkColor color;

    MiColor(guint16 r, guint16 g, guint16 b)
    {
        color.pixel = 0;
        color.red = r;
        color.green = g;
        color.blue = b;
    }
    MiColor(const char *name)
    {
        color.pixel = 0;
        if (!gdk_color_parse(name, &color))
            color.red = color.green = color.blue = 0;
    }
    operator GdkColor *()
    { return &color; }
    operator const GdkColor *() const
    { return &color; }
};

//////////////////////////////////////
//Locks

struct GdkThreadLock
{
    GdkThreadLock()
    {
        gdk_threads_enter();
    }
    ~GdkThreadLock()
    {
        gdk_threads_leave();
    }
};

struct GdkThreadUnlock
{
    GdkThreadUnlock()
    {
        gdk_threads_leave();
    }
    ~GdkThreadUnlock()
    {
        gdk_threads_enter();
    }
};


/////////////////////////////////////
//Pointers
//This GtkFollowCast traits is used to manage the "destroy" signal
//and the floating reference

template <typename T> class GtkFollowCast : public GPtrCast<T>
{
private:
    guint id;
    static void OnDestroy(gpointer obj, gpointer data)
    {
        //assert(this->m_ptr == obj);
        GtkFollowCast *that = static_cast<GtkFollowCast*>(data);
        that->Unfollow();
        that->m_ptr = NULL;
        g_object_unref(obj);
    }
protected:
    GtkFollowCast()
        :id(0)
    {}
    void Init()
    {
        g_object_ref_sink(this->m_ptr);
    }
    void Follow()
    {
        id = g_signal_connect(this->m_ptr, "destroy", GCallback(OnDestroy), this);
    }
    void Unfollow()
    {
        if (id != 0)
        {
            g_signal_handler_disconnect(this->m_ptr, id);
            id = 0;
        }
    }
    ~GtkFollowCast()
    {
        Unfollow();
    }
};

} //namespace

/////////////////////////////////////
//Class hierarchy

#define MIGTK_DECL_EX(x, nm) \
    namespace miglib { \
    typedef GPtr< x, GObjectPtrTraits, GtkFollowCast<x> > nm##PtrFollow; \
    typedef GPtr< x, GObjectPtrTraits, GIUPtrCast<x> > nm##Ptr; \
    typedef GPtr< x, GNoMagicPtrTraits > nm##PtrNM; \
    }

#define MIGTK_DECL(x) MIGTK_DECL_EX(x,x)

#define MIGTK_SUBCLASS(sub,cls,type) MIGLIB_SUBCLASS_EX(sub,cls,type) \
    MIGTK_DECL(sub)

#define MIGTK_SUBCLASS_ITF_1(sub, cls, itf1, type)  \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, type)         \
    MIGLIB_TYPE_CAST_MEMBER(itf1)                   \
    MIGLIB_SUBCLASS_EX_END()                        \
    MIGTK_DECL(sub)

#define MIGTK_SUBCLASS_ITF_2(sub, cls, itf1, itf2, type)  \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, type)         \
    MIGLIB_TYPE_CAST_MEMBER(itf1)                   \
    MIGLIB_TYPE_CAST_MEMBER(itf2)                   \
    MIGLIB_SUBCLASS_EX_END()                        \
    MIGTK_DECL(sub)

#define MIGTK_SUBCLASS_ITF_3(sub, cls, itf1, itf2, itf3, type)  \
    MIGLIB_SUBCLASS_EX_BEGIN(sub, cls, type)         \
    MIGLIB_TYPE_CAST_MEMBER(itf1)                   \
    MIGLIB_TYPE_CAST_MEMBER(itf2)                   \
    MIGLIB_TYPE_CAST_MEMBER(itf3)                   \
    MIGLIB_SUBCLASS_EX_END()                        \
    MIGTK_DECL(sub)

#include "gtk.inc"

namespace miglib
{

class MiWaitCursor
{
private:
    GtkWidgetPtr m_widget;
    GdkCursorPtr m_cursor;
public:
    MiWaitCursor(GtkWidget *w, GdkCursorType type=GDK_WATCH)
        :m_widget(w), m_cursor(gdk_cursor_new(type))
    {
        gdk_window_set_cursor(gtk_widget_get_window(m_widget), m_cursor);
        gdk_flush();
    }
    MiWaitCursor(GtkWidget *w, GdkCursor *c)
        :m_widget(w), m_cursor(c)
    {
        gdk_window_set_cursor(gtk_widget_get_window(m_widget), m_cursor);
        gdk_flush();
    }
    ~MiWaitCursor()
    {
        gdk_window_set_cursor(gtk_widget_get_window(m_widget), NULL);
    }
};

} //namespace miglib


#endif //MIGTK_H_INCLUDED
