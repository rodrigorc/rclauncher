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
//This GtkPtrCast traits is used to manage the "destroy" signal
//and the floating reference

template <typename T> class GtkPtrCast : public GPtrCast<T>
{
private:
    guint id;
    static void OnDestroy(gpointer obj, gpointer data)
    {
        //assert(this->m_ptr == obj);
        GtkPtrCast *that = static_cast<GtkPtrCast*>(data);
        that->Unfollow();
        that->m_ptr = NULL;
        g_object_unref(obj);
    }
protected:
    GtkPtrCast()
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
    ~GtkPtrCast()
    {
        Unfollow();
    }
};

} //namespace

/////////////////////////////////////
//Class hierarchy

#define MIGTK_DECL_EX(x, nm) namespace miglib { typedef GPtr< x, GObjectPtrTraits, GtkPtrCast<x> > nm##Ptr; \
    typedef GPtr< x, GNoMagicPtrTraits > nm##PtrNM; }
#define MIGTK_DECL(x) MIGTK_DECL_EX(x,x)

#define MIGTK_SUBCLASS(sub,cls,type) MIGLIB_SUBCLASS_EX(sub,cls,type) \
    MIGTK_DECL(sub)

//Base classes
#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkObject, GInitiallyUnowned, GTK_TYPE_OBJECT)
MIGTK_SUBCLASS(GtkWidget, GtkObject, GTK_TYPE_WIDGET)
#else
MIGTK_SUBCLASS(GtkWidget, GInitiallyUnowned, GTK_TYPE_WIDGET)
#endif

//Of GtkWidget
MIGTK_SUBCLASS(GtkContainer, GtkWidget, GTK_TYPE_CONTAINER)
MIGTK_SUBCLASS(GtkMisc, GtkWidget, GTK_TYPE_MISC)
MIGTK_SUBCLASS(GtkCalendar, GtkWidget, GTK_TYPE_CALENDAR)
MIGTK_SUBCLASS(GtkCellView, GtkWidget, GTK_TYPE_CELL_VIEW)
MIGTK_SUBCLASS(GtkDrawingArea, GtkWidget, GTK_TYPE_DRAWING_AREA)
MIGTK_SUBCLASS(GtkEntry, GtkWidget, GTK_TYPE_ENTRY)
MIGTK_SUBCLASS(GtkRange, GtkWidget, GTK_TYPE_RANGE)
MIGTK_SUBCLASS(GtkSeparator, GtkWidget, GTK_TYPE_SEPARATOR)
MIGTK_SUBCLASS(GtkHSV, GtkWidget, GTK_TYPE_HSV)
MIGTK_SUBCLASS(GtkInvisible, GtkWidget, GTK_TYPE_INVISIBLE)
//In GTK+2 GtkProgressBar is really a subclass of GtkProgress, but the latter
//is deprecated, so it is just ommitted
MIGTK_SUBCLASS(GtkProgressBar, GtkWidget, GTK_TYPE_PROGRESS_BAR)

#if  GTK_MAJOR_VERSION >= 3
MIGTK_SUBCLASS(GtkSpinner, GtkWidget, GTK_TYPE_SPINNER)
MIGTK_SUBCLASS(GtkSwitch, GtkWidget, GTK_TYPE_SWITCH)
#endif

//Of GtkContainer
MIGTK_SUBCLASS(GtkBin, GtkContainer, GTK_TYPE_BIN)
MIGTK_SUBCLASS(GtkBox, GtkContainer, GTK_TYPE_BOX)
MIGTK_SUBCLASS(GtkFixed, GtkContainer, GTK_TYPE_FIXED)
MIGTK_SUBCLASS(GtkPaned, GtkContainer, GTK_TYPE_PANED)
MIGTK_SUBCLASS(GtkIconView, GtkContainer, GTK_TYPE_ICON_VIEW)
MIGTK_SUBCLASS(GtkLayout, GtkContainer, GTK_TYPE_LAYOUT)
MIGTK_SUBCLASS(GtkMenuShell, GtkContainer, GTK_TYPE_MENU_SHELL)
MIGTK_SUBCLASS(GtkNotebook, GtkContainer, GTK_TYPE_NOTEBOOK)
//MIGTK_SUBCLASS(GtkSocket, GtkContainer, GTK_TYPE_SOCKET)
MIGTK_SUBCLASS(GtkTable, GtkContainer, GTK_TYPE_TABLE)
MIGTK_SUBCLASS(GtkTextView, GtkContainer, GTK_TYPE_TEXT_VIEW)
MIGTK_SUBCLASS(GtkToolbar, GtkContainer, GTK_TYPE_TOOLBAR)
MIGTK_SUBCLASS(GtkToolItemGroup, GtkContainer, GTK_TYPE_TOOL_ITEM_GROUP)
MIGTK_SUBCLASS(GtkToolPalette, GtkContainer, GTK_TYPE_TOOL_PALETTE)
MIGTK_SUBCLASS(GtkTreeView, GtkContainer, GTK_TYPE_TREE_VIEW)
#if  GTK_MAJOR_VERSION >= 3
MIGTK_SUBCLASS(GtkGrid, GtkContainer, GTK_TYPE_GRID)
#endif

//Of GtkBin
MIGTK_SUBCLASS(GtkWindow, GtkBin, GTK_TYPE_WINDOW)
MIGTK_SUBCLASS(GtkAlignment, GtkBin, GTK_TYPE_ALIGNMENT)
MIGTK_SUBCLASS(GtkFrame, GtkBin, GTK_TYPE_FRAME)
MIGTK_SUBCLASS(GtkButton, GtkBin, GTK_TYPE_BUTTON)
MIGTK_SUBCLASS(GtkComboBox, GtkBin, GTK_TYPE_COMBO_BOX)
MIGTK_SUBCLASS(GtkEventBox, GtkBin, GTK_TYPE_EVENT_BOX)
MIGTK_SUBCLASS(GtkExpander, GtkBin, GTK_TYPE_EXPANDER)
MIGTK_SUBCLASS(GtkHandleBox, GtkBin, GTK_TYPE_HANDLE_BOX)
MIGTK_SUBCLASS(GtkToolItem, GtkBin, GTK_TYPE_TOOL_ITEM)
MIGTK_SUBCLASS(GtkScrolledWindow, GtkBin, GTK_TYPE_SCROLLED_WINDOW)
MIGTK_SUBCLASS(GtkViewport, GtkBin, GTK_TYPE_VIEWPORT)
#if  GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkItem, GtkBin, GTK_TYPE_ITEM)
MIGTK_SUBCLASS(GtkMenuItem, GtkItem, GTK_TYPE_MENU_ITEM)
#else
MIGTK_SUBCLASS(GtkMenuItem, GtkBin, GTK_TYPE_MENU_ITEM)
#endif

//Of GtkWindow
MIGTK_SUBCLASS(GtkDialog, GtkWindow, GTK_TYPE_DIALOG)
MIGTK_SUBCLASS(GtkAssistant, GtkWindow, GTK_TYPE_ASSISTANT)
MIGTK_SUBCLASS(GtkOffscreenWindow, GtkWindow, GTK_TYPE_OFFSCREEN_WINDOW)
//MIGTK_SUBCLASS(GtkPlug, GtkWindow, GTK_TYPE_PLUG)

//Of GtkDialog
MIGTK_SUBCLASS(GtkAboutDialog, GtkDialog, GTK_TYPE_ABOUT_DIALOG)
MIGTK_SUBCLASS(GtkColorSelectionDialog, GtkDialog, GTK_TYPE_COLOR_SELECTION_DIALOG)
MIGTK_SUBCLASS(GtkFileChooserDialog, GtkDialog, GTK_TYPE_FILE_CHOOSER_DIALOG)
#ifdef GTK_TYPE_FONT_SELECTION_DIALOG
MIGTK_SUBCLASS(GtkFontSelectionDialog, GtkDialog, GTK_TYPE_FONT_SELECTION_DIALOG)
#endif
MIGTK_SUBCLASS(GtkMessageDialog, GtkDialog, GTK_TYPE_MESSAGE_DIALOG)
//MIGTK_SUBCLASS(GtkPageSetupUnixDialog, GtkDialog, GTK_TYPE_PAGE_SETUP_UNIX_DIALOG)
//MIGTK_SUBCLASS(GtkPrintUnixDialog, GtkDialog, GTK_TYPE_PRINT_UNIX_DIALOG)
MIGTK_SUBCLASS(GtkRecentChooserDialog, GtkDialog, GTK_TYPE_RECENT_CHOOSER_DIALOG)
#if GTK_MAJOR_VERSION >= 3
MIGTK_SUBCLASS(GtkAppChooserDialog, GtkDialog, GTK_TYPE_APP_CHOOSER_DIALOG)
MIGTK_SUBCLASS(GtkFontChooserDialog, GtkDialog, GTK_TYPE_FONT_CHOOSER_DIALOG)
#endif

//Of GtkBox
#ifdef GTK_TYPE_VBOX
MIGTK_SUBCLASS(GtkVBox, GtkBox, GTK_TYPE_VBOX)
#endif
#ifdef GTK_TYPE_HBOX
MIGTK_SUBCLASS(GtkHBox, GtkBox, GTK_TYPE_HBOX)
#endif
MIGTK_SUBCLASS(GtkButtonBox, GtkBox, GTK_TYPE_BUTTON_BOX)
#ifdef GTK_TYPE_VBUTTON_BOX
MIGTK_SUBCLASS(GtkVButtonBox, GtkButtonBox, GTK_TYPE_VBUTTON_BOX)
#endif
#ifdef GTK_TYPE_HBUTTON_BOX
MIGTK_SUBCLASS(GtkHButtonBox, GtkButtonBox, GTK_TYPE_HBUTTON_BOX)
#endif

//Of GtkButton
MIGTK_SUBCLASS(GtkToggleButton, GtkButton, GTK_TYPE_TOGGLE_BUTTON)
MIGTK_SUBCLASS(GtkCheckButton, GtkToggleButton, GTK_TYPE_CHECK_BUTTON)
MIGTK_SUBCLASS(GtkRadioButton, GtkCheckButton, GTK_TYPE_RADIO_BUTTON)
MIGTK_SUBCLASS(GtkColorButton, GtkButton, GTK_TYPE_COLOR_BUTTON)
MIGTK_SUBCLASS(GtkFontButton, GtkButton, GTK_TYPE_FONT_BUTTON)
MIGTK_SUBCLASS(GtkLinkButton, GtkButton, GTK_TYPE_LINK_BUTTON)
MIGTK_SUBCLASS(GtkScaleButton, GtkButton, GTK_TYPE_SCALE_BUTTON)
MIGTK_SUBCLASS(GtkVolumeButton, GtkScaleButton, GTK_TYPE_VOLUME_BUTTON)

//Of GtkMisc
MIGTK_SUBCLASS(GtkLabel, GtkMisc, GTK_TYPE_LABEL)
MIGTK_SUBCLASS(GtkArrow, GtkMisc, GTK_TYPE_ARROW)
MIGTK_SUBCLASS(GtkImage, GtkMisc, GTK_TYPE_IMAGE)
MIGTK_SUBCLASS(GtkAccelLabel, GtkLabel, GTK_TYPE_IMAGE)

//Other
MIGTK_SUBCLASS(GtkAspectFrame, GtkFrame, GTK_TYPE_ASPECT_FRAME)
#ifdef GTK_TYPE_VPANED
MIGTK_SUBCLASS(GtkVPaned, GtkPaned, GTK_TYPE_VPANED)
#endif
#ifdef GTK_TYPE_HPANED
MIGTK_SUBCLASS(GtkHPaned, GtkPaned, GTK_TYPE_HPANED)
#endif
MIGTK_SUBCLASS(GtkMenu, GtkMenuShell, GTK_TYPE_MENU)
MIGTK_SUBCLASS(GtkMenuBar, GtkMenuShell, GTK_TYPE_MENU_BAR)

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkTreeViewColumn, GtkObject, GTK_TYPE_TREE_VIEW_COLUMN)
MIGTK_SUBCLASS(GtkCellRenderer, GtkObject, GTK_TYPE_CELL_RENDERER)
MIGTK_SUBCLASS(GtkCellRendererText, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_TEXT)
MIGTK_SUBCLASS(GtkCellRendererPixbuf, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_PIXBUF)
MIGTK_SUBCLASS(GtkCellRendererProgress, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_PROGRESS)
MIGTK_SUBCLASS(GtkCellRendererToggle, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_TOGGLE)
MIGTK_SUBCLASS(GtkCellRendererAccel, GtkCellRendererText, GTK_TYPE_CELL_RENDERER_ACCEL)
MIGTK_SUBCLASS(GtkCellRendererCombo, GtkCellRendererText, GTK_TYPE_CELL_RENDERER_COMBO)
MIGTK_SUBCLASS(GtkCellRendererSpin, GtkCellRendererText, GTK_TYPE_CELL_RENDERER_SPIN)
#else
MIGIU_SUBCLASS(GtkTreeViewColumn, GInitiallyUnowned, GTK_TYPE_TREE_VIEW_COLUMN)
MIGIU_SUBCLASS(GtkCellRenderer, GInitiallyUnowned, GTK_TYPE_CELL_RENDERER)
MIGIU_SUBCLASS(GtkCellRendererText, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_TEXT)
MIGIU_SUBCLASS(GtkCellRendererPixbuf, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_PIXBUF)
MIGIU_SUBCLASS(GtkCellRendererProgress, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_PROGRESS)
MIGIU_SUBCLASS(GtkCellRendererToggle, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_TOGGLE)
MIGIU_SUBCLASS(GtkCellRendererAccel, GtkCellRendererText, GTK_TYPE_CELL_RENDERER_ACCEL)
MIGIU_SUBCLASS(GtkCellRendererCombo, GtkCellRendererText, GTK_TYPE_CELL_RENDERER_COMBO)
MIGIU_SUBCLASS(GtkCellRendererSpin, GtkCellRendererText, GTK_TYPE_CELL_RENDERER_SPIN)
#endif

///////////////////////////////////////
// Subclasses of GObject, not GtkObject

//Gtk
MIGLIB_SUBCLASS(GtkTextBuffer, GObject, GTK_TYPE_TEXT_BUFFER)
MIGLIB_SUBCLASS_ITF_2(GtkListStore, GObject, GtkTreeModel, GtkTreeSortable, GTK_TYPE_LIST_STORE)
MIGLIB_SUBCLASS_ITF_2(GtkTreeStore, GObject, GtkTreeModel, GtkTreeSortable, GTK_TYPE_TREE_STORE)
MIGLIB_SUBCLASS(GtkTreeSelection, GObject, GTK_TYPE_TREE_SELECTION)
MIGLIB_SUBCLASS(GtkStatusIcon, GObject, GTK_TYPE_STATUS_ICON)
MIGLIB_SUBCLASS(GtkUIManager, GObject, GTK_TYPE_UI_MANAGER)
MIGLIB_SUBCLASS(GtkActionGroup, GObject, GTK_TYPE_ACTION_GROUP)
MIGLIB_SUBCLASS(GtkIconFactory, GObject, GTK_TYPE_ICON_FACTORY)
MIGLIB_SUBCLASS(GtkSizeGroup, GObject, GTK_TYPE_SIZE_GROUP)
MIGLIB_SUBCLASS(GtkAction, GObject, GTK_TYPE_ACTION)
MIGLIB_SUBCLASS(GtkToggleAction, GtkAction, GTK_TYPE_TOGGLE_ACTION)
MIGLIB_SUBCLASS(GtkRadioAction, GtkToggleAction, GTK_TYPE_RADIO_ACTION)
MIGLIB_SUBCLASS(GtkBuilder, GObject, GTK_TYPE_BUILDER)

//Gdk
MIGLIB_SUBCLASS(GdkPixbuf, GObject, GDK_TYPE_PIXBUF)
#if GTK_MAJOR_VERSION < 3
MIGLIB_SUBCLASS(GdkGC, GObject, GDK_TYPE_GC)
#endif

///////////////////////////////////////
// Other pointers without base class

namespace miglib
{

struct GdkCursorPtrTraits
{
    static void Ref(GdkCursor *ptr)
    { gdk_cursor_ref(ptr); }
    static void Unref(GdkCursor *ptr)
    { gdk_cursor_unref(ptr); }
};
typedef GPtr<GdkCursor, GdkCursorPtrTraits> GdkCursorPtr;

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

struct GtkIconSetPtrTraits
{
    static void Ref(GtkIconSet *ptr)
    { gtk_icon_set_ref(ptr); }
    static void Unref(GtkIconSet *ptr)
    { gtk_icon_set_unref(ptr); }
};
typedef GPtr<GtkIconSet, GtkIconSetPtrTraits> GtkIconSetPtr;

struct GtkTreePathPtrTraits
{
    static void Unref(GtkTreePath *ptr)
    { gtk_tree_path_free(ptr); }
};
typedef GPtr<GtkTreePath, GtkTreePathPtrTraits> GtkTreePathPtr;

struct GtkTreeRowReferencePtrTraits
{
    static void Unref(GtkTreeRowReference *ptr)
    { gtk_tree_row_reference_free(ptr); }
};
typedef GPtr<GtkTreeRowReference, GtkTreeRowReferencePtrTraits> GtkTreeRowReferencePtr;

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
