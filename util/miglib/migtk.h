#ifndef MIGTK_H_INCLUDED
#define MIGTK_H_INCLUDED

#include "miglib.h"
#include <gtk/gtk.h>

namespace miglib
{

#define MIGTK_ACTION(cls, func) MIGLIB_SIGNAL_FUNC_1(void, GtkAction*,cls,func)

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

struct GtkPtrFollow
{
    void **pp;
    guint id;

    GtkPtrFollow()
        :pp(0), id(0)
    {}
    static void OnDestroy(void *obj, gpointer data)
    {
        GtkPtrFollow *that = static_cast<GtkPtrFollow *>(data);
        void **pp2 = that->pp;
        that->Unfollow(obj);
        g_object_unref(obj);
        *pp2 = 0;
    }
    void FollowInit(void *&ptr)
    {
        g_object_ref_sink(G_OBJECT(ptr));
        Follow(ptr);
    }
    void Follow(void *&ptr)
    {
        pp = &ptr;
        id = g_signal_connect(ptr, "destroy", GCallback(OnDestroy), this);
    }
    void Unfollow(void *&ptr)
    {
        g_signal_handler_disconnect(ptr, id);
        pp = 0;
        id = 0;
    }
    ~GtkPtrFollow()
    {
        if (pp)
            Unfollow(*pp);
    }
};

} //namespace

/////////////////////////////////////
//Class hierarchy

#define MIGTK_DECL_EX(x, nm) namespace miglib { typedef GPtr< x, GPtrTraits< x >, GtkPtrFollow > nm##Ptr; }
#define MIGTK_DECL(x) MIGTK_DECL_EX(x,x)

#define MIGTK_SUBCLASS(sub,cls,chk) MIGLIB_SUBCLASS_EX(sub,cls,chk) \
    MIGTK_DECL(sub)

MIGTK_SUBCLASS(GtkObject, GObject, GTK_OBJECT)
MIGTK_SUBCLASS(GtkWidget, GtkObject, GTK_WIDGET)
MIGTK_SUBCLASS(GtkContainer, GtkWidget, GTK_CONTAINER)
MIGTK_SUBCLASS(GtkBin, GtkContainer, GTK_BIN)
MIGTK_SUBCLASS(GtkWindow, GtkBin, GTK_WINDOW)
MIGTK_SUBCLASS(GtkScrolledWindow, GtkBin, GTK_SCROLLED_WINDOW)
MIGTK_SUBCLASS(GtkDialog, GtkWindow, GTK_DIALOG)
MIGTK_SUBCLASS(GtkMessageDialog, GtkDialog, GTK_MESSAGE_DIALOG)
MIGTK_SUBCLASS(GtkTreeView, GtkContainer, GTK_TREE_VIEW)
MIGTK_SUBCLASS(GtkTreeViewColumn, GtkObject, GTK_TREE_VIEW_COLUMN)
MIGTK_SUBCLASS(GtkBox, GtkContainer, GTK_BOX)
MIGTK_SUBCLASS(GtkVBox, GtkBox, GTK_VBOX)
MIGTK_SUBCLASS(GtkHBox, GtkBox, GTK_HBOX)
MIGTK_SUBCLASS(GtkButtonBox, GtkBox, GTK_BUTTON_BOX)
MIGTK_SUBCLASS(GtkHButtonBox, GtkButtonBox, GTK_HBUTTON_BOX)
MIGTK_SUBCLASS(GtkVButtonBox, GtkButtonBox, GTK_VBUTTON_BOX)
MIGTK_SUBCLASS(GtkPaned, GtkContainer, GTK_PANED)
MIGTK_SUBCLASS(GtkHPaned, GtkPaned, GTK_HPANED)
MIGTK_SUBCLASS(GtkVPaned, GtkPaned, GTK_VPANED)
MIGTK_SUBCLASS(GtkDrawingArea, GtkWidget, GTK_DRAWING_AREA)
MIGTK_SUBCLASS(GtkCellRenderer, GtkObject, GTK_CELL_RENDERER)
MIGTK_SUBCLASS(GtkCellRendererText, GtkCellRenderer, GTK_CELL_RENDERER_TEXT)
MIGTK_SUBCLASS(GtkCellRendererPixbuf, GtkCellRenderer, GTK_CELL_RENDERER_PIXBUF)
MIGTK_SUBCLASS(GtkCellRendererProgress, GtkCellRenderer, GTK_CELL_RENDERER_PROGRESS)
MIGTK_SUBCLASS(GtkCellRendererToggle, GtkCellRenderer, GTK_CELL_RENDERER_TOGGLE)
MIGTK_SUBCLASS(GtkCellRendererAccel, GtkCellRendererText, GTK_CELL_RENDERER_ACCEL)
MIGTK_SUBCLASS(GtkCellRendererCombo, GtkCellRendererText, GTK_CELL_RENDERER_COMBO)
MIGTK_SUBCLASS(GtkCellRendererSpin, GtkCellRendererText, GTK_CELL_RENDERER_SPIN)
MIGTK_SUBCLASS(GtkMenuShell, GtkContainer, GTK_MENU_SHELL)
MIGTK_SUBCLASS(GtkMenu, GtkMenuShell, GTK_MENU)
MIGTK_SUBCLASS(GtkMenuBar, GtkMenuShell, GTK_MENU_BAR)
MIGTK_SUBCLASS(GtkToolbar, GtkContainer, GTK_TOOLBAR)
MIGTK_SUBCLASS(GtkItem, GtkBin, GTK_ITEM)
MIGTK_SUBCLASS(GtkMenuItem, GtkItem, GTK_MENU_ITEM)
MIGTK_SUBCLASS(GtkButton, GtkBin, GTK_BUTTON)
MIGTK_SUBCLASS(GtkMisc, GtkWidget, GTK_MISC)
MIGTK_SUBCLASS(GtkLabel, GtkMisc, GTK_LABEL)
MIGTK_SUBCLASS(GtkFrame, GtkBin, GTK_FRAME)
MIGTK_SUBCLASS(GtkAspectFrame, GtkFrame, GTK_ASPECT_FRAME)
MIGTK_SUBCLASS(GtkEntry, GtkWidget, GTK_ENTRY)
MIGTK_SUBCLASS(GtkTextView, GtkContainer, GTK_TEXT_VIEW)
MIGTK_SUBCLASS(GtkTable, GtkContainer, GTK_TABLE)
MIGTK_SUBCLASS(GtkSocket, GtkContainer, GTK_SOCKET)
MIGTK_SUBCLASS(GtkPlug, GtkWindow, GTK_PLUG)
MIGTK_SUBCLASS(GtkLayout, GtkContainer, GTK_LAYOUT)
MIGTK_SUBCLASS(GtkImage, GtkMisc, GTK_IMAGE)
MIGTK_SUBCLASS(GtkEventBox, GtkBin, GTK_EVENT_BOX)

///////////////////////////////////////
// Subclasses of GObject, not GtkObject

//Gtk
MIGLIB_SUBCLASS(GtkTextBuffer, GObject, GTK_TEXT_BUFFER)
MIGLIB_SUBCLASS_ITF_2(GtkListStore, GObject, GtkTreeModel, GtkTreeSortable, GTK_LIST_STORE)
MIGLIB_SUBCLASS_ITF_2(GtkTreeStore, GObject, GtkTreeModel, GtkTreeSortable, GTK_TREE_STORE)
MIGLIB_SUBCLASS(GtkTreeSelection, GObject, GTK_TREE_SELECTION)
MIGLIB_SUBCLASS(GtkStatusIcon, GObject, GTK_STATUS_ICON)
MIGLIB_SUBCLASS(GtkUIManager, GObject, GTK_UI_MANAGER)
MIGLIB_SUBCLASS(GtkActionGroup, GObject, GTK_ACTION_GROUP)
MIGLIB_SUBCLASS(GtkIconFactory, GObject, GTK_ICON_FACTORY)
MIGLIB_SUBCLASS(GtkSizeGroup, GObject, GTK_SIZE_GROUP)
MIGLIB_SUBCLASS(GtkAction, GObject, GTK_ACTION)
MIGLIB_SUBCLASS(GtkToggleAction, GtkAction, GTK_TOGGLE_ACTION)
MIGLIB_SUBCLASS(GtkRadioAction, GtkToggleAction, GTK_RADIO_ACTION)
MIGLIB_SUBCLASS(GtkBuilder, GtkObject, GTK_BUILDER)

//Gdk
MIGLIB_SUBCLASS(GdkPixbuf, GObject, GDK_PIXBUF)
MIGLIB_SUBCLASS(GdkGC, GObject, GDK_GC)

///////////////////////////////////////
// Other pointers without base class

namespace miglib
{

template<> struct GPtrTraits<GdkCursor>
{
    static void Ref(GdkCursor *ptr)
    { gdk_cursor_ref(ptr); }
    static void Unref(GdkCursor *ptr)
    { gdk_cursor_unref(ptr); }
};
typedef GPtr<GdkCursor> GdkCursorPtr;

class MiWaitCursor
{
private:
    GtkWidgetPtr m_widget;
    GdkCursorPtr m_cursor;
public:
    MiWaitCursor(GtkWidget *w, GdkCursorType type=GDK_WATCH)
        :m_widget(w), m_cursor(gdk_cursor_new(type))
    {
        gdk_window_set_cursor(m_widget->window, m_cursor);
        gdk_flush();
    }
    MiWaitCursor(GtkWidget *w, GdkCursor *c)
        :m_widget(w), m_cursor(c)
    {
        gdk_window_set_cursor(m_widget->window, m_cursor);
        gdk_flush();
    }
    ~MiWaitCursor()
    {
        gdk_window_set_cursor(m_widget->window, NULL);
    }
};

template<> struct GPtrTraits<GtkIconSet>
{
    static void Ref(GtkIconSet *ptr)
    { gtk_icon_set_ref(ptr); }
    static void Unref(GtkIconSet *ptr)
    { gtk_icon_set_unref(ptr); }
};
typedef GPtr<GtkIconSet> GtkIconSetPtr;

template<> struct GPtrTraits<GtkTreePath>
{
    static void Unref(GtkTreePath *ptr)
    { gtk_tree_path_free(ptr); }
};
typedef GPtr<GtkTreePath> GtkTreePathPtr;

template<> struct GPtrTraits<GtkTreeRowReference>
{
    static void Unref(GtkTreeRowReference *ptr)
    { gtk_tree_row_reference_free(ptr); }
};
typedef GPtr<GtkTreeRowReference> GtkTreeRowReferencePtr;

////////////////////////////////////
//Events
#define MIGLIB_CONNECT_EVENT_EX(src, evt, tevt, cls, func, ptr) \
    MIGLIB_CONNECT_2(src, evt, gboolean, GtkWidget*, tevt*, cls, func, ptr)

#define MIGLIB_CONNECT_EVENT(src, evt, cls, func, ptr) \
    MIGLIB_CONNECT_EVENT_EX(src, evt, GdkEvent, cls, func, ptr)

////////////////////////////////////
//Miscellaneous
#define MIGTK_CELL_DATA_FUNC(cls, fun) \
    MIGLIB_FUNC_4(void, GtkTreeViewColumn*,GtkCellRenderer*,GtkTreeModel*,GtkTreeIter*, cls, fun)


} //namespace miglib

#endif //MIGTK_H_INCLUDED
