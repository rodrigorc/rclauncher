// FILE GENERATED AUTOMATICALLY. DO NOT MODIFY! ./gendefs.py
// This file is generated automatically by "gendefs.py".
// If you feel that something is wrong or missing, change instead 
// "definitions.xml" or "gendefs.py" and regenerate it.


#ifdef GTK_TYPE_OBJECT
MIGTK_SUBCLASS(GtkObject, GInitiallyUnowned, GTK_TYPE_OBJECT)
// void destroy(GtkObject*)
#define MIGTK_OBJECT_destroy(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkObject*>(obj), "destroy", void, GtkObject*, cls, fun, ptr)
#define MIGTK_S_OBJECT_destroy(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkObject*>(obj), "destroy", reinterpret_cast<GCallback>(static_cast<void(*)(GtkObject*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_OBJECT


#ifdef GTK_TYPE_BUILDABLE
MIGTK_SUBCLASS(GtkBuildable, GObject, GTK_TYPE_BUILDABLE)
#endif //ifdef GTK_TYPE_BUILDABLE


#ifdef GTK_TYPE_WIDGET

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS_ITF_1(GtkWidget, GtkObject, GtkBuildable, GTK_TYPE_WIDGET)

#else //if GTK_MAJOR_VERSION >= 3

MIGTK_SUBCLASS_ITF_1(GtkWidget, GInitiallyUnowned, GtkBuildable, GTK_TYPE_WIDGET)
#endif //if GTK_MAJOR_VERSION >= 3

// void destroy(GtkWidget*)
#define MIGTK_WIDGET_destroy(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWidget*>(obj), "destroy", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_destroy(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "destroy", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, gpointer)>(fun)), ptr)
// gboolean button_press_event(GtkWidget*, GdkEventButton*)
#define MIGTK_WIDGET_button_press_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "button-press-event", gboolean, GtkWidget*, GdkEventButton*, cls, fun, ptr)
#define MIGTK_S_WIDGET_button_press_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "button-press-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventButton*, gpointer)>(fun)), ptr)
// gboolean button_release_event(GtkWidget*, GdkEventButton*)
#define MIGTK_WIDGET_button_release_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "button-release-event", gboolean, GtkWidget*, GdkEventButton*, cls, fun, ptr)
#define MIGTK_S_WIDGET_button_release_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "button-release-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventButton*, gpointer)>(fun)), ptr)
// gboolean can_activate_accel(GtkWidget*, guint)
#define MIGTK_WIDGET_can_activate_accel(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "can-activate-accel", gboolean, GtkWidget*, guint, cls, fun, ptr)
#define MIGTK_S_WIDGET_can_activate_accel(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "can-activate-accel", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, guint, gpointer)>(fun)), ptr)
// void child_notify(GtkWidget*, GParamSpec*)
#define MIGTK_WIDGET_child_notify(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "child-notify", void, GtkWidget*, GParamSpec*, cls, fun, ptr)
#define MIGTK_S_WIDGET_child_notify(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "child-notify", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GParamSpec*, gpointer)>(fun)), ptr)
// gboolean client_event(GtkWidget*, GdkEventClient*)
#define MIGTK_WIDGET_client_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "client-event", gboolean, GtkWidget*, GdkEventClient*, cls, fun, ptr)
#define MIGTK_S_WIDGET_client_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "client-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventClient*, gpointer)>(fun)), ptr)
// void composited_changed(GtkWidget*)
#define MIGTK_WIDGET_composited_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWidget*>(obj), "composited-changed", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_composited_changed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "composited-changed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, gpointer)>(fun)), ptr)
// gboolean configure_event(GtkWidget*, GdkEventConfigure*)
#define MIGTK_WIDGET_configure_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "configure-event", gboolean, GtkWidget*, GdkEventConfigure*, cls, fun, ptr)
#define MIGTK_S_WIDGET_configure_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "configure-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventConfigure*, gpointer)>(fun)), ptr)
// gboolean delete_event(GtkWidget*, GdkEventAny*)
#define MIGTK_WIDGET_delete_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "delete-event", gboolean, GtkWidget*, GdkEventAny*, cls, fun, ptr)
#define MIGTK_S_WIDGET_delete_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "delete-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventAny*, gpointer)>(fun)), ptr)
// gboolean destroy_event(GtkWidget*, GdkEventAny*)
#define MIGTK_WIDGET_destroy_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "destroy-event", gboolean, GtkWidget*, GdkEventAny*, cls, fun, ptr)
#define MIGTK_S_WIDGET_destroy_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "destroy-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventAny*, gpointer)>(fun)), ptr)
// void direction_changed(GtkWidget*, GtkTextDirection)
#define MIGTK_WIDGET_direction_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "direction-changed", void, GtkWidget*, GtkTextDirection, cls, fun, ptr)
#define MIGTK_S_WIDGET_direction_changed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "direction-changed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GtkTextDirection, gpointer)>(fun)), ptr)
// void drag_begin(GtkWidget*, GdkDragContext*)
#define MIGTK_WIDGET_drag_begin(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "drag-begin", void, GtkWidget*, GdkDragContext*, cls, fun, ptr)
#define MIGTK_S_WIDGET_drag_begin(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "drag-begin", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GdkDragContext*, gpointer)>(fun)), ptr)
// void drag_data_delete(GtkWidget*, GdkDragContext*)
#define MIGTK_WIDGET_drag_data_delete(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "drag-data-delete", void, GtkWidget*, GdkDragContext*, cls, fun, ptr)
#define MIGTK_S_WIDGET_drag_data_delete(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "drag-data-delete", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GdkDragContext*, gpointer)>(fun)), ptr)
// void drag_data_get(GtkWidget*, GdkDragContext*, GtkSelectionData*, guint, guint)
#define MIGTK_WIDGET_drag_data_get(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_5(static_cast<GtkWidget*>(obj), "drag-data-get", void, GtkWidget*, GdkDragContext*, GtkSelectionData*, guint, guint, cls, fun, ptr)
#define MIGTK_S_WIDGET_drag_data_get(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "drag-data-get", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GdkDragContext*, GtkSelectionData*, guint, guint, gpointer)>(fun)), ptr)
// void drag_data_received(GtkWidget*, GdkDragContext*, gint, gint, GtkSelectionData*, guint, guint)
#define MIGTK_WIDGET_drag_data_received(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_7(static_cast<GtkWidget*>(obj), "drag-data-received", void, GtkWidget*, GdkDragContext*, gint, gint, GtkSelectionData*, guint, guint, cls, fun, ptr)
#define MIGTK_S_WIDGET_drag_data_received(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "drag-data-received", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GdkDragContext*, gint, gint, GtkSelectionData*, guint, guint, gpointer)>(fun)), ptr)
// gboolean drag_drop(GtkWidget*, GdkDragContext*, gint, gint, guint)
#define MIGTK_WIDGET_drag_drop(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_5(static_cast<GtkWidget*>(obj), "drag-drop", gboolean, GtkWidget*, GdkDragContext*, gint, gint, guint, cls, fun, ptr)
#define MIGTK_S_WIDGET_drag_drop(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "drag-drop", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkDragContext*, gint, gint, guint, gpointer)>(fun)), ptr)
// void drag_end(GtkWidget*, GdkDragContext*)
#define MIGTK_WIDGET_drag_end(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "drag-end", void, GtkWidget*, GdkDragContext*, cls, fun, ptr)
#define MIGTK_S_WIDGET_drag_end(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "drag-end", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GdkDragContext*, gpointer)>(fun)), ptr)
// void drag_leave(GtkWidget*, GdkDragContext*, guint)
#define MIGTK_WIDGET_drag_leave(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkWidget*>(obj), "drag-leave", void, GtkWidget*, GdkDragContext*, guint, cls, fun, ptr)
#define MIGTK_S_WIDGET_drag_leave(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "drag-leave", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GdkDragContext*, guint, gpointer)>(fun)), ptr)
// gboolean drag_motion(GtkWidget*, GdkDragContext*, gint, gint, guint)
#define MIGTK_WIDGET_drag_motion(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_5(static_cast<GtkWidget*>(obj), "drag-motion", gboolean, GtkWidget*, GdkDragContext*, gint, gint, guint, cls, fun, ptr)
#define MIGTK_S_WIDGET_drag_motion(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "drag-motion", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkDragContext*, gint, gint, guint, gpointer)>(fun)), ptr)
// gboolean enter_notify_event(GtkWidget*, GdkEventCrossing*)
#define MIGTK_WIDGET_enter_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "enter-notify-event", gboolean, GtkWidget*, GdkEventCrossing*, cls, fun, ptr)
#define MIGTK_S_WIDGET_enter_notify_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "enter-notify-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventCrossing*, gpointer)>(fun)), ptr)
// gboolean event(GtkWidget*, GdkEvent*)
#define MIGTK_WIDGET_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "event", gboolean, GtkWidget*, GdkEvent*, cls, fun, ptr)
#define MIGTK_S_WIDGET_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEvent*, gpointer)>(fun)), ptr)
// gboolean focus(GtkWidget*, GtkDirectionType)
#define MIGTK_WIDGET_focus(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "focus", gboolean, GtkWidget*, GtkDirectionType, cls, fun, ptr)
#define MIGTK_S_WIDGET_focus(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "focus", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GtkDirectionType, gpointer)>(fun)), ptr)
// gboolean focus_in_event(GtkWidget*, GdkEventFocus*)
#define MIGTK_WIDGET_focus_in_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "focus-in-event", gboolean, GtkWidget*, GdkEventFocus*, cls, fun, ptr)
#define MIGTK_S_WIDGET_focus_in_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "focus-in-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventFocus*, gpointer)>(fun)), ptr)
// gboolean focus_out_event(GtkWidget*, GdkEventFocus*)
#define MIGTK_WIDGET_focus_out_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "focus-out-event", gboolean, GtkWidget*, GdkEventFocus*, cls, fun, ptr)
#define MIGTK_S_WIDGET_focus_out_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "focus-out-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventFocus*, gpointer)>(fun)), ptr)
// gboolean grab_broken_event(GtkWidget*, GdkEventGrabBroken*)
#define MIGTK_WIDGET_grab_broken_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "grab-broken-event", gboolean, GtkWidget*, GdkEventGrabBroken*, cls, fun, ptr)
#define MIGTK_S_WIDGET_grab_broken_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "grab-broken-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventGrabBroken*, gpointer)>(fun)), ptr)
// void grab_focus(GtkWidget*)
#define MIGTK_WIDGET_grab_focus(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWidget*>(obj), "grab-focus", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_grab_focus(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "grab-focus", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, gpointer)>(fun)), ptr)
// void grab_notify(GtkWidget*, gboolean)
#define MIGTK_WIDGET_grab_notify(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "grab-notify", void, GtkWidget*, gboolean, cls, fun, ptr)
#define MIGTK_S_WIDGET_grab_notify(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "grab-notify", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, gboolean, gpointer)>(fun)), ptr)
// void hide(GtkWidget*)
#define MIGTK_WIDGET_hide(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWidget*>(obj), "hide", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_hide(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "hide", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, gpointer)>(fun)), ptr)
// void hierarchy_changed(GtkWidget*, GtkWidget*)
#define MIGTK_WIDGET_hierarchy_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "hierarchy-changed", void, GtkWidget*, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_hierarchy_changed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "hierarchy-changed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GtkWidget*, gpointer)>(fun)), ptr)
// gboolean key_press_event(GtkWidget*, GdkEventKey*)
#define MIGTK_WIDGET_key_press_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "key-press-event", gboolean, GtkWidget*, GdkEventKey*, cls, fun, ptr)
#define MIGTK_S_WIDGET_key_press_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "key-press-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventKey*, gpointer)>(fun)), ptr)
// gboolean key_release_event(GtkWidget*, GdkEventKey*)
#define MIGTK_WIDGET_key_release_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "key-release-event", gboolean, GtkWidget*, GdkEventKey*, cls, fun, ptr)
#define MIGTK_S_WIDGET_key_release_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "key-release-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventKey*, gpointer)>(fun)), ptr)
// gboolean leave_notify_event(GtkWidget*, GdkEventCrossing*)
#define MIGTK_WIDGET_leave_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "leave-notify-event", gboolean, GtkWidget*, GdkEventCrossing*, cls, fun, ptr)
#define MIGTK_S_WIDGET_leave_notify_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "leave-notify-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventCrossing*, gpointer)>(fun)), ptr)
// void map(GtkWidget*)
#define MIGTK_WIDGET_map(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWidget*>(obj), "map", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_map(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "map", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, gpointer)>(fun)), ptr)
// gboolean map_event(GtkWidget*, GdkEventAny*)
#define MIGTK_WIDGET_map_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "map-event", gboolean, GtkWidget*, GdkEventAny*, cls, fun, ptr)
#define MIGTK_S_WIDGET_map_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "map-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventAny*, gpointer)>(fun)), ptr)
// gboolean mnemonic_activate(GtkWidget*, gboolean)
#define MIGTK_WIDGET_mnemonic_activate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "mnemonic-activate", gboolean, GtkWidget*, gboolean, cls, fun, ptr)
#define MIGTK_S_WIDGET_mnemonic_activate(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "mnemonic-activate", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, gboolean, gpointer)>(fun)), ptr)
// gboolean motion_notify_event(GtkWidget*, GdkEventMotion*)
#define MIGTK_WIDGET_motion_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "motion-notify-event", gboolean, GtkWidget*, GdkEventMotion*, cls, fun, ptr)
#define MIGTK_S_WIDGET_motion_notify_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "motion-notify-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventMotion*, gpointer)>(fun)), ptr)
// void parent_set(GtkWidget*, GtkWidget*)
#define MIGTK_WIDGET_parent_set(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "parent-set", void, GtkWidget*, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_parent_set(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "parent-set", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GtkWidget*, gpointer)>(fun)), ptr)
// gboolean popup_menu(GtkWidget*)
#define MIGTK_WIDGET_popup_menu(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWidget*>(obj), "popup-menu", gboolean, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_popup_menu(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "popup-menu", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, gpointer)>(fun)), ptr)
// gboolean property_notify_event(GtkWidget*, GdkEventProperty*)
#define MIGTK_WIDGET_property_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "property-notify-event", gboolean, GtkWidget*, GdkEventProperty*, cls, fun, ptr)
#define MIGTK_S_WIDGET_property_notify_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "property-notify-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventProperty*, gpointer)>(fun)), ptr)
// gboolean proximity_in_event(GtkWidget*, GdkEventProximity*)
#define MIGTK_WIDGET_proximity_in_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "proximity-in-event", gboolean, GtkWidget*, GdkEventProximity*, cls, fun, ptr)
#define MIGTK_S_WIDGET_proximity_in_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "proximity-in-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventProximity*, gpointer)>(fun)), ptr)
// gboolean proximity_out_event(GtkWidget*, GdkEventProximity*)
#define MIGTK_WIDGET_proximity_out_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "proximity-out-event", gboolean, GtkWidget*, GdkEventProximity*, cls, fun, ptr)
#define MIGTK_S_WIDGET_proximity_out_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "proximity-out-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventProximity*, gpointer)>(fun)), ptr)
// void realize(GtkWidget*)
#define MIGTK_WIDGET_realize(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWidget*>(obj), "realize", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_realize(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "realize", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, gpointer)>(fun)), ptr)
// void screen_changed(GtkWidget*, GdkScreen*)
#define MIGTK_WIDGET_screen_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "screen-changed", void, GtkWidget*, GdkScreen*, cls, fun, ptr)
#define MIGTK_S_WIDGET_screen_changed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "screen-changed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GdkScreen*, gpointer)>(fun)), ptr)
// gboolean scroll_event(GtkWidget*, GdkEventScroll*)
#define MIGTK_WIDGET_scroll_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "scroll-event", gboolean, GtkWidget*, GdkEventScroll*, cls, fun, ptr)
#define MIGTK_S_WIDGET_scroll_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "scroll-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventScroll*, gpointer)>(fun)), ptr)
// gboolean selection_clear_event(GtkWidget*, GdkEventSelection*)
#define MIGTK_WIDGET_selection_clear_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "selection-clear-event", gboolean, GtkWidget*, GdkEventSelection*, cls, fun, ptr)
#define MIGTK_S_WIDGET_selection_clear_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "selection-clear-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventSelection*, gpointer)>(fun)), ptr)
// void selection_get(GtkWidget*, GtkSelectionData*, guint, guint)
#define MIGTK_WIDGET_selection_get(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_4(static_cast<GtkWidget*>(obj), "selection-get", void, GtkWidget*, GtkSelectionData*, guint, guint, cls, fun, ptr)
#define MIGTK_S_WIDGET_selection_get(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "selection-get", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GtkSelectionData*, guint, guint, gpointer)>(fun)), ptr)
// gboolean selection_notify_event(GtkWidget*, GdkEventSelection*)
#define MIGTK_WIDGET_selection_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "selection-notify-event", gboolean, GtkWidget*, GdkEventSelection*, cls, fun, ptr)
#define MIGTK_S_WIDGET_selection_notify_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "selection-notify-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventSelection*, gpointer)>(fun)), ptr)
// void selection_received(GtkWidget*, GtkSelectionData*, guint)
#define MIGTK_WIDGET_selection_received(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkWidget*>(obj), "selection-received", void, GtkWidget*, GtkSelectionData*, guint, cls, fun, ptr)
#define MIGTK_S_WIDGET_selection_received(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "selection-received", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GtkSelectionData*, guint, gpointer)>(fun)), ptr)
// gboolean selection_request_event(GtkWidget*, GdkEventSelection*)
#define MIGTK_WIDGET_selection_request_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "selection-request-event", gboolean, GtkWidget*, GdkEventSelection*, cls, fun, ptr)
#define MIGTK_S_WIDGET_selection_request_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "selection-request-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventSelection*, gpointer)>(fun)), ptr)
// void show(GtkWidget*)
#define MIGTK_WIDGET_show(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWidget*>(obj), "show", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_show(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "show", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, gpointer)>(fun)), ptr)
// gboolean show_help(GtkWidget*, GtkWidgetHelpType)
#define MIGTK_WIDGET_show_help(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "show-help", gboolean, GtkWidget*, GtkWidgetHelpType, cls, fun, ptr)
#define MIGTK_S_WIDGET_show_help(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "show-help", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GtkWidgetHelpType, gpointer)>(fun)), ptr)
// void size_allocate(GtkWidget*, GtkAllocation*)
#define MIGTK_WIDGET_size_allocate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "size-allocate", void, GtkWidget*, GtkAllocation*, cls, fun, ptr)
#define MIGTK_S_WIDGET_size_allocate(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "size-allocate", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GtkAllocation*, gpointer)>(fun)), ptr)
// void size_request(GtkWidget*, GtkRequisition*)
#define MIGTK_WIDGET_size_request(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "size-request", void, GtkWidget*, GtkRequisition*, cls, fun, ptr)
#define MIGTK_S_WIDGET_size_request(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "size-request", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GtkRequisition*, gpointer)>(fun)), ptr)
// void state_changed(GtkWidget*, GtkStateType)
#define MIGTK_WIDGET_state_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "state-changed", void, GtkWidget*, GtkStateType, cls, fun, ptr)
#define MIGTK_S_WIDGET_state_changed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "state-changed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GtkStateType, gpointer)>(fun)), ptr)
// void style_set(GtkWidget*, GtkStyle*)
#define MIGTK_WIDGET_style_set(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "style-set", void, GtkWidget*, GtkStyle*, cls, fun, ptr)
#define MIGTK_S_WIDGET_style_set(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "style-set", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, GtkStyle*, gpointer)>(fun)), ptr)
// void unmap(GtkWidget*)
#define MIGTK_WIDGET_unmap(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWidget*>(obj), "unmap", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_unmap(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "unmap", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, gpointer)>(fun)), ptr)
// gboolean unmap_event(GtkWidget*, GdkEventAny*)
#define MIGTK_WIDGET_unmap_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "unmap-event", gboolean, GtkWidget*, GdkEventAny*, cls, fun, ptr)
#define MIGTK_S_WIDGET_unmap_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "unmap-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventAny*, gpointer)>(fun)), ptr)
// void unrealize(GtkWidget*)
#define MIGTK_WIDGET_unrealize(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWidget*>(obj), "unrealize", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WIDGET_unrealize(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "unrealize", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWidget*, gpointer)>(fun)), ptr)
// gboolean visibility_notify_event(GtkWidget*, GdkEventVisibility*)
#define MIGTK_WIDGET_visibility_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "visibility-notify-event", gboolean, GtkWidget*, GdkEventVisibility*, cls, fun, ptr)
#define MIGTK_S_WIDGET_visibility_notify_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "visibility-notify-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventVisibility*, gpointer)>(fun)), ptr)
// gboolean window_state_event(GtkWidget*, GdkEventWindowState*)
#define MIGTK_WIDGET_window_state_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "window-state-event", gboolean, GtkWidget*, GdkEventWindowState*, cls, fun, ptr)
#define MIGTK_S_WIDGET_window_state_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "window-state-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventWindowState*, gpointer)>(fun)), ptr)

#if GTK_MAJOR_VERSION < 3
// gboolean expose_event(GtkWidget*, GdkEventExpose*)
#define MIGTK_WIDGET_expose_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "expose-event", gboolean, GtkWidget*, GdkEventExpose*, cls, fun, ptr)
#define MIGTK_S_WIDGET_expose_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "expose-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventExpose*, gpointer)>(fun)), ptr)
// gboolean no_expose_event(GtkWidget*, GdkEventAny*)
#define MIGTK_WIDGET_no_expose_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "no-expose-event", gboolean, GtkWidget*, GdkEventAny*, cls, fun, ptr)
#define MIGTK_S_WIDGET_no_expose_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "no-expose-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, GdkEventAny*, gpointer)>(fun)), ptr)

#else //if GTK_MAJOR_VERSION >= 3

// gboolean draw(GtkWidget*, cairo_t*)
#define MIGTK_WIDGET_draw(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWidget*>(obj), "draw", gboolean, GtkWidget*, cairo_t*, cls, fun, ptr)
#define MIGTK_S_WIDGET_draw(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWidget*>(obj), "draw", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWidget*, cairo_t*, gpointer)>(fun)), ptr)
#endif //if GTK_MAJOR_VERSION >= 3

#endif //ifdef GTK_TYPE_WIDGET


#ifdef GTK_TYPE_CONTAINER
MIGTK_SUBCLASS(GtkContainer, GtkWidget, GTK_TYPE_CONTAINER)
// void add(GtkContainer*, GtkWidget*)
#define MIGTK_CONTAINER_add(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkContainer*>(obj), "add", void, GtkContainer*, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_CONTAINER_add(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkContainer*>(obj), "add", reinterpret_cast<GCallback>(static_cast<void(*)(GtkContainer*, GtkWidget*, gpointer)>(fun)), ptr)
// void remove(GtkContainer*, GtkWidget*)
#define MIGTK_CONTAINER_remove(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkContainer*>(obj), "remove", void, GtkContainer*, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_CONTAINER_remove(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkContainer*>(obj), "remove", reinterpret_cast<GCallback>(static_cast<void(*)(GtkContainer*, GtkWidget*, gpointer)>(fun)), ptr)
// void check_resize(GtkContainer*)
#define MIGTK_CONTAINER_check_resize(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkContainer*>(obj), "check-resize", void, GtkContainer*, cls, fun, ptr)
#define MIGTK_S_CONTAINER_check_resize(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkContainer*>(obj), "check-resize", reinterpret_cast<GCallback>(static_cast<void(*)(GtkContainer*, gpointer)>(fun)), ptr)
// void set_focus_child(GtkContainer*, GtkWidget*)
#define MIGTK_CONTAINER_set_focus_child(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkContainer*>(obj), "set-focus-child", void, GtkContainer*, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_CONTAINER_set_focus_child(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkContainer*>(obj), "set-focus-child", reinterpret_cast<GCallback>(static_cast<void(*)(GtkContainer*, GtkWidget*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_CONTAINER


#ifdef GTK_TYPE_MISC
MIGTK_SUBCLASS(GtkMisc, GtkWidget, GTK_TYPE_MISC)
#endif //ifdef GTK_TYPE_MISC


#ifdef GTK_TYPE_CALENDAR
MIGTK_SUBCLASS(GtkCalendar, GtkWidget, GTK_TYPE_CALENDAR)
#endif //ifdef GTK_TYPE_CALENDAR


#ifdef GTK_TYPE_CELL_VIEW
MIGTK_SUBCLASS(GtkCellView, GtkWidget, GTK_TYPE_CELL_VIEW)
#endif //ifdef GTK_TYPE_CELL_VIEW


#ifdef GTK_TYPE_DRAWING_AREA
MIGTK_SUBCLASS(GtkDrawingArea, GtkWidget, GTK_TYPE_DRAWING_AREA)
#endif //ifdef GTK_TYPE_DRAWING_AREA


#ifdef GTK_TYPE_ENTRY
MIGTK_SUBCLASS(GtkEntry, GtkWidget, GTK_TYPE_ENTRY)
// void move_cursor(GtkEntry*, GtkMovementStep, gint , gboolean)
#define MIGTK_ENTRY_move_cursor(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_4(static_cast<GtkEntry*>(obj), "move_cursor", void, GtkEntry*, GtkMovementStep, gint , gboolean, cls, fun, ptr)
#define MIGTK_S_ENTRY_move_cursor(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkEntry*>(obj), "move_cursor", reinterpret_cast<GCallback>(static_cast<void(*)(GtkEntry*, GtkMovementStep, gint , gboolean, gpointer)>(fun)), ptr)
// void insert_at_cursor(GtkEntry*, const gchar *)
#define MIGTK_ENTRY_insert_at_cursor(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkEntry*>(obj), "insert_at_cursor", void, GtkEntry*, const gchar *, cls, fun, ptr)
#define MIGTK_S_ENTRY_insert_at_cursor(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkEntry*>(obj), "insert_at_cursor", reinterpret_cast<GCallback>(static_cast<void(*)(GtkEntry*, const gchar *, gpointer)>(fun)), ptr)
// void delete_from_cursor(GtkEntry*, GtkDeleteType, gint)
#define MIGTK_ENTRY_delete_from_cursor(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkEntry*>(obj), "delete_from_cursor", void, GtkEntry*, GtkDeleteType, gint, cls, fun, ptr)
#define MIGTK_S_ENTRY_delete_from_cursor(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkEntry*>(obj), "delete_from_cursor", reinterpret_cast<GCallback>(static_cast<void(*)(GtkEntry*, GtkDeleteType, gint, gpointer)>(fun)), ptr)
// void backspace(GtkEntry*)
#define MIGTK_ENTRY_backspace(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkEntry*>(obj), "backspace", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_S_ENTRY_backspace(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkEntry*>(obj), "backspace", reinterpret_cast<GCallback>(static_cast<void(*)(GtkEntry*, gpointer)>(fun)), ptr)
// void cut_clipboard(GtkEntry*)
#define MIGTK_ENTRY_cut_clipboard(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkEntry*>(obj), "cut_clipboard", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_S_ENTRY_cut_clipboard(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkEntry*>(obj), "cut_clipboard", reinterpret_cast<GCallback>(static_cast<void(*)(GtkEntry*, gpointer)>(fun)), ptr)
// void copy_clipboard(GtkEntry*)
#define MIGTK_ENTRY_copy_clipboard(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkEntry*>(obj), "copy_clipboard", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_S_ENTRY_copy_clipboard(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkEntry*>(obj), "copy_clipboard", reinterpret_cast<GCallback>(static_cast<void(*)(GtkEntry*, gpointer)>(fun)), ptr)
// void paste_clipboard(GtkEntry*)
#define MIGTK_ENTRY_paste_clipboard(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkEntry*>(obj), "paste_clipboard", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_S_ENTRY_paste_clipboard(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkEntry*>(obj), "paste_clipboard", reinterpret_cast<GCallback>(static_cast<void(*)(GtkEntry*, gpointer)>(fun)), ptr)
// void toggle_overwrite(GtkEntry*)
#define MIGTK_ENTRY_toggle_overwrite(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkEntry*>(obj), "toggle_overwrite", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_S_ENTRY_toggle_overwrite(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkEntry*>(obj), "toggle_overwrite", reinterpret_cast<GCallback>(static_cast<void(*)(GtkEntry*, gpointer)>(fun)), ptr)
// void get_text_area_size(GtkEntry*, gint*, gint*, gint*, gint*)
#define MIGTK_ENTRY_get_text_area_size(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_5(static_cast<GtkEntry*>(obj), "get_text_area_size", void, GtkEntry*, gint*, gint*, gint*, gint*, cls, fun, ptr)
#define MIGTK_S_ENTRY_get_text_area_size(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkEntry*>(obj), "get_text_area_size", reinterpret_cast<GCallback>(static_cast<void(*)(GtkEntry*, gint*, gint*, gint*, gint*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_ENTRY


#ifdef GTK_TYPE_RANGE
MIGTK_SUBCLASS(GtkRange, GtkWidget, GTK_TYPE_RANGE)
#endif //ifdef GTK_TYPE_RANGE


#ifdef GTK_TYPE_SEPARATOR
MIGTK_SUBCLASS(GtkSeparator, GtkWidget, GTK_TYPE_SEPARATOR)
#endif //ifdef GTK_TYPE_SEPARATOR


#ifdef GTK_TYPE_HSV
MIGTK_SUBCLASS(GtkHSV, GtkWidget, GTK_TYPE_HSV)
#endif //ifdef GTK_TYPE_HSV


#ifdef GTK_TYPE_INVISIBLE
MIGTK_SUBCLASS(GtkInvisible, GtkWidget, GTK_TYPE_INVISIBLE)
#endif //ifdef GTK_TYPE_INVISIBLE


#ifdef GTK_TYPE_PROGRESS_BAR
MIGTK_SUBCLASS(GtkProgressBar, GtkWidget, GTK_TYPE_PROGRESS_BAR)
#endif //ifdef GTK_TYPE_PROGRESS_BAR


#ifdef GTK_TYPE_SPINNER
MIGTK_SUBCLASS(GtkSpinner, GtkWidget, GTK_TYPE_SPINNER)
#endif //ifdef GTK_TYPE_SPINNER


#ifdef GTK_TYPE_SWITCH
MIGTK_SUBCLASS(GtkSwitch, GtkWidget, GTK_TYPE_SWITCH)
#endif //ifdef GTK_TYPE_SWITCH


#ifdef GTK_TYPE_BIN
MIGTK_SUBCLASS(GtkBin, GtkContainer, GTK_TYPE_BIN)
#endif //ifdef GTK_TYPE_BIN


#ifdef GTK_TYPE_BOX
MIGTK_SUBCLASS(GtkBox, GtkContainer, GTK_TYPE_BOX)
#endif //ifdef GTK_TYPE_BOX


#ifdef GTK_TYPE_FIXED
MIGTK_SUBCLASS(GtkFixed, GtkContainer, GTK_TYPE_FIXED)
#endif //ifdef GTK_TYPE_FIXED


#ifdef GTK_TYPE_PANED
MIGTK_SUBCLASS(GtkPaned, GtkContainer, GTK_TYPE_PANED)
#endif //ifdef GTK_TYPE_PANED


#ifdef GTK_TYPE_ICON_VIEW
MIGTK_SUBCLASS(GtkIconView, GtkContainer, GTK_TYPE_ICON_VIEW)
#endif //ifdef GTK_TYPE_ICON_VIEW


#ifdef GTK_TYPE_LAYOUT
MIGTK_SUBCLASS(GtkLayout, GtkContainer, GTK_TYPE_LAYOUT)
#endif //ifdef GTK_TYPE_LAYOUT


#ifdef GTK_TYPE_MENU_SHELL
MIGTK_SUBCLASS(GtkMenuShell, GtkContainer, GTK_TYPE_MENU_SHELL)
#endif //ifdef GTK_TYPE_MENU_SHELL


#ifdef GTK_TYPE_NOTEBOOK
MIGTK_SUBCLASS(GtkNotebook, GtkContainer, GTK_TYPE_NOTEBOOK)
#endif //ifdef GTK_TYPE_NOTEBOOK


#ifdef GTK_TYPE_TABLE
MIGTK_SUBCLASS(GtkTable, GtkContainer, GTK_TYPE_TABLE)
#endif //ifdef GTK_TYPE_TABLE


#ifdef GTK_TYPE_TEXT_VIEW
MIGTK_SUBCLASS(GtkTextView, GtkContainer, GTK_TYPE_TEXT_VIEW)
#endif //ifdef GTK_TYPE_TEXT_VIEW


#ifdef GTK_TYPE_TOOLBAR
MIGTK_SUBCLASS(GtkToolbar, GtkContainer, GTK_TYPE_TOOLBAR)
#endif //ifdef GTK_TYPE_TOOLBAR


#ifdef GTK_TYPE_TOOL_ITEM_GROUP
MIGTK_SUBCLASS(GtkToolItemGroup, GtkContainer, GTK_TYPE_TOOL_ITEM_GROUP)
#endif //ifdef GTK_TYPE_TOOL_ITEM_GROUP


#ifdef GTK_TYPE_TOOL_PALETTE
MIGTK_SUBCLASS(GtkToolPalette, GtkContainer, GTK_TYPE_TOOL_PALETTE)
#endif //ifdef GTK_TYPE_TOOL_PALETTE


#ifdef GTK_TYPE_TREE_VIEW
MIGTK_SUBCLASS(GtkTreeView, GtkContainer, GTK_TYPE_TREE_VIEW)
// void columns_changed(GtkTreeView*)
#define MIGTK_TREE_VIEW_columns_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkTreeView*>(obj), "columns-changed", void, GtkTreeView*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_columns_changed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "columns-changed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkTreeView*, gpointer)>(fun)), ptr)
// void cursor_changed(GtkTreeView*)
#define MIGTK_TREE_VIEW_cursor_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkTreeView*>(obj), "cursor-changed", void, GtkTreeView*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_cursor_changed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "cursor-changed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkTreeView*, gpointer)>(fun)), ptr)
// gboolean expand_collapse_cursor_row(GtkTreeView*, gboolean, gboolean, gboolean)
#define MIGTK_TREE_VIEW_expand_collapse_cursor_row(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_4(static_cast<GtkTreeView*>(obj), "expand-collapse-cursor-row", gboolean, GtkTreeView*, gboolean, gboolean, gboolean, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_expand_collapse_cursor_row(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "expand-collapse-cursor-row", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkTreeView*, gboolean, gboolean, gboolean, gpointer)>(fun)), ptr)
// gboolean move_cursor(GtkTreeView*, GtkMovementStep, gint)
#define MIGTK_TREE_VIEW_move_cursor(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkTreeView*>(obj), "move-cursor", gboolean, GtkTreeView*, GtkMovementStep, gint, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_move_cursor(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "move-cursor", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkTreeView*, GtkMovementStep, gint, gpointer)>(fun)), ptr)
// void row_activated(GtkTreeView*, GtkTreePath*, GtkTreeViewColumn*)
#define MIGTK_TREE_VIEW_row_activated(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkTreeView*>(obj), "row-activated", void, GtkTreeView*, GtkTreePath*, GtkTreeViewColumn*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_row_activated(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "row-activated", reinterpret_cast<GCallback>(static_cast<void(*)(GtkTreeView*, GtkTreePath*, GtkTreeViewColumn*, gpointer)>(fun)), ptr)
// void row_collapsed(GtkTreeView*, GtkTreeIter*, GtkTreePath*)
#define MIGTK_TREE_VIEW_row_collapsed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkTreeView*>(obj), "row-collapsed", void, GtkTreeView*, GtkTreeIter*, GtkTreePath*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_row_collapsed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "row-collapsed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkTreeView*, GtkTreeIter*, GtkTreePath*, gpointer)>(fun)), ptr)
// void row_expanded(GtkTreeView*, GtkTreeIter*, GtkTreePath*)
#define MIGTK_TREE_VIEW_row_expanded(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkTreeView*>(obj), "row-expanded", void, GtkTreeView*, GtkTreeIter*, GtkTreePath*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_row_expanded(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "row-expanded", reinterpret_cast<GCallback>(static_cast<void(*)(GtkTreeView*, GtkTreeIter*, GtkTreePath*, gpointer)>(fun)), ptr)
// gboolean select_all(GtkTreeView*)
#define MIGTK_TREE_VIEW_select_all(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkTreeView*>(obj), "select-all", gboolean, GtkTreeView*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_select_all(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "select-all", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkTreeView*, gpointer)>(fun)), ptr)
// gboolean select_cursor_parent(GtkTreeView*)
#define MIGTK_TREE_VIEW_select_cursor_parent(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkTreeView*>(obj), "select-cursor-parent", gboolean, GtkTreeView*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_select_cursor_parent(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "select-cursor-parent", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkTreeView*, gpointer)>(fun)), ptr)
// gboolean select_cursor_row(GtkTreeView*, gboolean)
#define MIGTK_TREE_VIEW_select_cursor_row(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkTreeView*>(obj), "select-cursor-row", gboolean, GtkTreeView*, gboolean, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_select_cursor_row(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "select-cursor-row", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkTreeView*, gboolean, gpointer)>(fun)), ptr)
// void set_scroll_adjustments(GtkTreeView*, GtkAdjustment*, GtkAdjustment*)
#define MIGTK_TREE_VIEW_set_scroll_adjustments(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkTreeView*>(obj), "set-scroll-adjustments", void, GtkTreeView*, GtkAdjustment*, GtkAdjustment*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_set_scroll_adjustments(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "set-scroll-adjustments", reinterpret_cast<GCallback>(static_cast<void(*)(GtkTreeView*, GtkAdjustment*, GtkAdjustment*, gpointer)>(fun)), ptr)
// gboolean start_interactive_search(GtkTreeView*)
#define MIGTK_TREE_VIEW_start_interactive_search(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkTreeView*>(obj), "start-interactive-search", gboolean, GtkTreeView*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_start_interactive_search(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "start-interactive-search", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkTreeView*, gpointer)>(fun)), ptr)
// gboolean test_collapse_row(GtkTreeView*, GtkTreeIter*, GtkTreePath*)
#define MIGTK_TREE_VIEW_test_collapse_row(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkTreeView*>(obj), "test-collapse-row", gboolean, GtkTreeView*, GtkTreeIter*, GtkTreePath*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_test_collapse_row(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "test-collapse-row", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkTreeView*, GtkTreeIter*, GtkTreePath*, gpointer)>(fun)), ptr)
// gboolean test_expand_row(GtkTreeView*, GtkTreeIter*, GtkTreePath*)
#define MIGTK_TREE_VIEW_test_expand_row(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkTreeView*>(obj), "test-expand-row", gboolean, GtkTreeView*, GtkTreeIter*, GtkTreePath*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_test_expand_row(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "test-expand-row", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkTreeView*, GtkTreeIter*, GtkTreePath*, gpointer)>(fun)), ptr)
// gboolean toggle_cursor_row(GtkTreeView*)
#define MIGTK_TREE_VIEW_toggle_cursor_row(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkTreeView*>(obj), "toggle-cursor-row", gboolean, GtkTreeView*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_toggle_cursor_row(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "toggle-cursor-row", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkTreeView*, gpointer)>(fun)), ptr)
// gboolean unselect_all(GtkTreeView*)
#define MIGTK_TREE_VIEW_unselect_all(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkTreeView*>(obj), "unselect_all", gboolean, GtkTreeView*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_unselect_all(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeView*>(obj), "unselect_all", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkTreeView*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_TREE_VIEW


#ifdef GTK_TYPE_GRID
MIGTK_SUBCLASS(GtkGrid, GtkContainer, GTK_TYPE_GRID)
#endif //ifdef GTK_TYPE_GRID


#ifdef GTK_TYPE_WINDOW
MIGTK_SUBCLASS(GtkWindow, GtkBin, GTK_TYPE_WINDOW)
// void set_focus(GtkWindow*, GtkWidget*)
#define MIGTK_WINDOW_set_focus(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWindow*>(obj), "set-focus", void, GtkWindow*, GtkWidget*, cls, fun, ptr)
#define MIGTK_S_WINDOW_set_focus(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWindow*>(obj), "set-focus", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWindow*, GtkWidget*, gpointer)>(fun)), ptr)
// gboolean frame_event(GtkWindow*, GdkEvent*)
#define MIGTK_WINDOW_frame_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWindow*>(obj), "frame-event", gboolean, GtkWindow*, GdkEvent*, cls, fun, ptr)
#define MIGTK_S_WINDOW_frame_event(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWindow*>(obj), "frame-event", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkWindow*, GdkEvent*, gpointer)>(fun)), ptr)
// void activate_focus(GtkWindow*)
#define MIGTK_WINDOW_activate_focus(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWindow*>(obj), "activate-focus", void, GtkWindow*, cls, fun, ptr)
#define MIGTK_S_WINDOW_activate_focus(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWindow*>(obj), "activate-focus", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWindow*, gpointer)>(fun)), ptr)
// void activate_default(GtkWindow*)
#define MIGTK_WINDOW_activate_default(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWindow*>(obj), "activate-default", void, GtkWindow*, cls, fun, ptr)
#define MIGTK_S_WINDOW_activate_default(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWindow*>(obj), "activate-default", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWindow*, gpointer)>(fun)), ptr)
// void move_focus(GtkWindow*, GtkDirectionType)
#define MIGTK_WINDOW_move_focus(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkWindow*>(obj), "move-focus", void, GtkWindow*, GtkDirectionType, cls, fun, ptr)
#define MIGTK_S_WINDOW_move_focus(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWindow*>(obj), "move-focus", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWindow*, GtkDirectionType, gpointer)>(fun)), ptr)
// void keys_changed(GtkWindow*)
#define MIGTK_WINDOW_keys_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkWindow*>(obj), "keys-changed", void, GtkWindow*, cls, fun, ptr)
#define MIGTK_S_WINDOW_keys_changed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkWindow*>(obj), "keys-changed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkWindow*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_WINDOW


#ifdef GTK_TYPE_ALIGNMENT
MIGTK_SUBCLASS(GtkAlignment, GtkBin, GTK_TYPE_ALIGNMENT)
#endif //ifdef GTK_TYPE_ALIGNMENT


#ifdef GTK_TYPE_FRAME
MIGTK_SUBCLASS(GtkFrame, GtkBin, GTK_TYPE_FRAME)
#endif //ifdef GTK_TYPE_FRAME


#ifdef GTK_TYPE_BUTTON
MIGTK_SUBCLASS(GtkButton, GtkBin, GTK_TYPE_BUTTON)
// void clicked(GtkButton*)
#define MIGTK_BUTTON_clicked(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkButton*>(obj), "clicked", void, GtkButton*, cls, fun, ptr)
#define MIGTK_S_BUTTON_clicked(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkButton*>(obj), "clicked", reinterpret_cast<GCallback>(static_cast<void(*)(GtkButton*, gpointer)>(fun)), ptr)
// void enter(GtkButton*)
#define MIGTK_BUTTON_enter(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkButton*>(obj), "enter", void, GtkButton*, cls, fun, ptr)
#define MIGTK_S_BUTTON_enter(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkButton*>(obj), "enter", reinterpret_cast<GCallback>(static_cast<void(*)(GtkButton*, gpointer)>(fun)), ptr)
// void leave(GtkButton*)
#define MIGTK_BUTTON_leave(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkButton*>(obj), "leave", void, GtkButton*, cls, fun, ptr)
#define MIGTK_S_BUTTON_leave(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkButton*>(obj), "leave", reinterpret_cast<GCallback>(static_cast<void(*)(GtkButton*, gpointer)>(fun)), ptr)
// void pressed(GtkButton*)
#define MIGTK_BUTTON_pressed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkButton*>(obj), "pressed", void, GtkButton*, cls, fun, ptr)
#define MIGTK_S_BUTTON_pressed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkButton*>(obj), "pressed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkButton*, gpointer)>(fun)), ptr)
// void released(GtkButton*)
#define MIGTK_BUTTON_released(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkButton*>(obj), "released", void, GtkButton*, cls, fun, ptr)
#define MIGTK_S_BUTTON_released(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkButton*>(obj), "released", reinterpret_cast<GCallback>(static_cast<void(*)(GtkButton*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_BUTTON


#ifdef GTK_TYPE_COMBO_BOX
MIGTK_SUBCLASS(GtkComboBox, GtkBin, GTK_TYPE_COMBO_BOX)
#endif //ifdef GTK_TYPE_COMBO_BOX


#ifdef GTK_TYPE_EVENT_BOX
MIGTK_SUBCLASS(GtkEventBox, GtkBin, GTK_TYPE_EVENT_BOX)
#endif //ifdef GTK_TYPE_EVENT_BOX


#ifdef GTK_TYPE_EXPANDER
MIGTK_SUBCLASS(GtkExpander, GtkBin, GTK_TYPE_EXPANDER)
#endif //ifdef GTK_TYPE_EXPANDER


#ifdef GTK_TYPE_HANDLE_BOX
MIGTK_SUBCLASS(GtkHandleBox, GtkBin, GTK_TYPE_HANDLE_BOX)
#endif //ifdef GTK_TYPE_HANDLE_BOX


#ifdef GTK_TYPE_TOOL_ITEM
MIGTK_SUBCLASS(GtkToolItem, GtkBin, GTK_TYPE_TOOL_ITEM)
#endif //ifdef GTK_TYPE_TOOL_ITEM


#ifdef GTK_TYPE_SCROLLED_WINDOW
MIGTK_SUBCLASS(GtkScrolledWindow, GtkBin, GTK_TYPE_SCROLLED_WINDOW)
#endif //ifdef GTK_TYPE_SCROLLED_WINDOW


#ifdef GTK_TYPE_VIEWPORT
MIGTK_SUBCLASS(GtkViewport, GtkBin, GTK_TYPE_VIEWPORT)
#endif //ifdef GTK_TYPE_VIEWPORT


#ifdef GTK_TYPE_ITEM
MIGTK_SUBCLASS(GtkItem, GtkBin, GTK_TYPE_ITEM)
#endif //ifdef GTK_TYPE_ITEM


#ifdef GTK_TYPE_MENU_ITEM

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkMenuItem, GtkItem, GTK_TYPE_MENU_ITEM)

#else //if GTK_MAJOR_VERSION >= 3

MIGTK_SUBCLASS(GtkMenuItem, GtkBin, GTK_TYPE_MENU_ITEM)
#endif //if GTK_MAJOR_VERSION >= 3

// void activate(GtkMenuItem*)
#define MIGTK_MENU_ITEM_activate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkMenuItem*>(obj), "activate", void, GtkMenuItem*, cls, fun, ptr)
#define MIGTK_S_MENU_ITEM_activate(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkMenuItem*>(obj), "activate", reinterpret_cast<GCallback>(static_cast<void(*)(GtkMenuItem*, gpointer)>(fun)), ptr)
// void activate_item(GtkMenuItem*)
#define MIGTK_MENU_ITEM_activate_item(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkMenuItem*>(obj), "activate-item", void, GtkMenuItem*, cls, fun, ptr)
#define MIGTK_S_MENU_ITEM_activate_item(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkMenuItem*>(obj), "activate-item", reinterpret_cast<GCallback>(static_cast<void(*)(GtkMenuItem*, gpointer)>(fun)), ptr)
// void toggle_size_request(GtkMenuItem*, gint)
#define MIGTK_MENU_ITEM_toggle_size_request(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkMenuItem*>(obj), "toggle-size-request", void, GtkMenuItem*, gint, cls, fun, ptr)
#define MIGTK_S_MENU_ITEM_toggle_size_request(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkMenuItem*>(obj), "toggle-size-request", reinterpret_cast<GCallback>(static_cast<void(*)(GtkMenuItem*, gint, gpointer)>(fun)), ptr)
// void toggle_size_allocate(GtkMenuItem*, gint)
#define MIGTK_MENU_ITEM_toggle_size_allocate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkMenuItem*>(obj), "toggle-size-allocate", void, GtkMenuItem*, gint, cls, fun, ptr)
#define MIGTK_S_MENU_ITEM_toggle_size_allocate(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkMenuItem*>(obj), "toggle-size-allocate", reinterpret_cast<GCallback>(static_cast<void(*)(GtkMenuItem*, gint, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_MENU_ITEM


#ifdef GTK_TYPE_DIALOG
MIGTK_SUBCLASS(GtkDialog, GtkWindow, GTK_TYPE_DIALOG)
// void close(GtkDialog*)
#define MIGTK_DIALOG_close(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkDialog*>(obj), "close", void, GtkDialog*, cls, fun, ptr)
#define MIGTK_S_DIALOG_close(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkDialog*>(obj), "close", reinterpret_cast<GCallback>(static_cast<void(*)(GtkDialog*, gpointer)>(fun)), ptr)
// void response(GtkDialog*, gint)
#define MIGTK_DIALOG_response(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkDialog*>(obj), "response", void, GtkDialog*, gint, cls, fun, ptr)
#define MIGTK_S_DIALOG_response(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkDialog*>(obj), "response", reinterpret_cast<GCallback>(static_cast<void(*)(GtkDialog*, gint, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_DIALOG


#ifdef GTK_TYPE_ASSISTANT
MIGTK_SUBCLASS(GtkAssistant, GtkWindow, GTK_TYPE_ASSISTANT)
#endif //ifdef GTK_TYPE_ASSISTANT


#ifdef GTK_TYPE_OFFSCREEN_WINDOW
MIGTK_SUBCLASS(GtkOffscreenWindow, GtkWindow, GTK_TYPE_OFFSCREEN_WINDOW)
#endif //ifdef GTK_TYPE_OFFSCREEN_WINDOW


#ifdef GTK_TYPE_ABOUT_DIALOG
MIGTK_SUBCLASS(GtkAboutDialog, GtkDialog, GTK_TYPE_ABOUT_DIALOG)
#endif //ifdef GTK_TYPE_ABOUT_DIALOG


#ifdef GTK_TYPE_COLOR_SELECTION_DIALOG
MIGTK_SUBCLASS(GtkColorSelectionDialog, GtkDialog, GTK_TYPE_COLOR_SELECTION_DIALOG)
#endif //ifdef GTK_TYPE_COLOR_SELECTION_DIALOG


#ifdef GTK_TYPE_FILE_CHOOSER_DIALOG
MIGTK_SUBCLASS(GtkFileChooserDialog, GtkDialog, GTK_TYPE_FILE_CHOOSER_DIALOG)
#endif //ifdef GTK_TYPE_FILE_CHOOSER_DIALOG


#ifdef GTK_TYPE_FONT_SELECTION_DIALOG
MIGTK_SUBCLASS(GtkFontSelectionDialog, GtkDialog, GTK_TYPE_FONT_SELECTION_DIALOG)
#endif //ifdef GTK_TYPE_FONT_SELECTION_DIALOG


#ifdef GTK_TYPE_MESSAGE_DIALOG
MIGTK_SUBCLASS(GtkMessageDialog, GtkDialog, GTK_TYPE_MESSAGE_DIALOG)
#endif //ifdef GTK_TYPE_MESSAGE_DIALOG


#ifdef GTK_TYPE_RECENT_CHOOSER_DIALOG
MIGTK_SUBCLASS(GtkRecentChooserDialog, GtkDialog, GTK_TYPE_RECENT_CHOOSER_DIALOG)
#endif //ifdef GTK_TYPE_RECENT_CHOOSER_DIALOG


#ifdef GTK_TYPE_APP_CHOOSER_DIALOG
MIGTK_SUBCLASS(GtkAppChooserDialog, GtkDialog, GTK_TYPE_APP_CHOOSER_DIALOG)
#endif //ifdef GTK_TYPE_APP_CHOOSER_DIALOG


#ifdef GTK_TYPE_FONT_CHOOSER_DIALOG
MIGTK_SUBCLASS(GtkFontChooserDialog, GtkDialog, GTK_TYPE_FONT_CHOOSER_DIALOG)
#endif //ifdef GTK_TYPE_FONT_CHOOSER_DIALOG


#ifdef GTK_TYPE_VBOX
MIGTK_SUBCLASS(GtkVBox, GtkBox, GTK_TYPE_VBOX)
#endif //ifdef GTK_TYPE_VBOX


#ifdef GTK_TYPE_HBOX
MIGTK_SUBCLASS(GtkHBox, GtkBox, GTK_TYPE_HBOX)
#endif //ifdef GTK_TYPE_HBOX


#ifdef GTK_TYPE_BUTTON_BOX
MIGTK_SUBCLASS(GtkButtonBox, GtkBox, GTK_TYPE_BUTTON_BOX)
#endif //ifdef GTK_TYPE_BUTTON_BOX


#ifdef GTK_TYPE_VBUTTON_BOX
MIGTK_SUBCLASS(GtkVButtonBox, GtkButtonBox, GTK_TYPE_VBUTTON_BOX)
#endif //ifdef GTK_TYPE_VBUTTON_BOX


#ifdef GTK_TYPE_HBUTTON_BOX
MIGTK_SUBCLASS(GtkHButtonBox, GtkButtonBox, GTK_TYPE_HBUTTON_BOX)
#endif //ifdef GTK_TYPE_HBUTTON_BOX


#ifdef GTK_TYPE_TOGGLE_BUTTON
MIGTK_SUBCLASS(GtkToggleButton, GtkButton, GTK_TYPE_TOGGLE_BUTTON)
#endif //ifdef GTK_TYPE_TOGGLE_BUTTON


#ifdef GTK_TYPE_CHECK_BUTTON
MIGTK_SUBCLASS(GtkCheckButton, GtkToggleButton, GTK_TYPE_CHECK_BUTTON)
#endif //ifdef GTK_TYPE_CHECK_BUTTON


#ifdef GTK_TYPE_RADIO_BUTTON
MIGTK_SUBCLASS(GtkRadioButton, GtkCheckButton, GTK_TYPE_RADIO_BUTTON)
#endif //ifdef GTK_TYPE_RADIO_BUTTON


#ifdef GTK_TYPE_COLOR_BUTTON
MIGTK_SUBCLASS(GtkColorButton, GtkButton, GTK_TYPE_COLOR_BUTTON)
#endif //ifdef GTK_TYPE_COLOR_BUTTON


#ifdef GTK_TYPE_FONT_BUTTON
MIGTK_SUBCLASS(GtkFontButton, GtkButton, GTK_TYPE_FONT_BUTTON)
#endif //ifdef GTK_TYPE_FONT_BUTTON


#ifdef GTK_TYPE_LINK_BUTTON
MIGTK_SUBCLASS(GtkLinkButton, GtkButton, GTK_TYPE_LINK_BUTTON)
#endif //ifdef GTK_TYPE_LINK_BUTTON


#ifdef GTK_TYPE_SCALE_BUTTON
MIGTK_SUBCLASS(GtkScaleButton, GtkButton, GTK_TYPE_SCALE_BUTTON)
#endif //ifdef GTK_TYPE_SCALE_BUTTON


#ifdef GTK_TYPE_VOLUME_BUTTON
MIGTK_SUBCLASS(GtkVolumeButton, GtkScaleButton, GTK_TYPE_VOLUME_BUTTON)
#endif //ifdef GTK_TYPE_VOLUME_BUTTON


#ifdef GTK_TYPE_LABEL
MIGTK_SUBCLASS(GtkLabel, GtkMisc, GTK_TYPE_LABEL)
// void move_cursor(GtkLabel*, GtkMovementStep, gint, gboolean)
#define MIGTK_LABEL_move_cursor(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_4(static_cast<GtkLabel*>(obj), "move_cursor", void, GtkLabel*, GtkMovementStep, gint, gboolean, cls, fun, ptr)
#define MIGTK_S_LABEL_move_cursor(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkLabel*>(obj), "move_cursor", reinterpret_cast<GCallback>(static_cast<void(*)(GtkLabel*, GtkMovementStep, gint, gboolean, gpointer)>(fun)), ptr)
// void copy_clipboard(GtkLabel*)
#define MIGTK_LABEL_copy_clipboard(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkLabel*>(obj), "copy_clipboard", void, GtkLabel*, cls, fun, ptr)
#define MIGTK_S_LABEL_copy_clipboard(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkLabel*>(obj), "copy_clipboard", reinterpret_cast<GCallback>(static_cast<void(*)(GtkLabel*, gpointer)>(fun)), ptr)
// void populate_popup(GtkLabel*, GtkMenu)
#define MIGTK_LABEL_populate_popup(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkLabel*>(obj), "populate_popup", void, GtkLabel*, GtkMenu, cls, fun, ptr)
#define MIGTK_S_LABEL_populate_popup(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkLabel*>(obj), "populate_popup", reinterpret_cast<GCallback>(static_cast<void(*)(GtkLabel*, GtkMenu, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_LABEL


#ifdef GTK_TYPE_ARROW
MIGTK_SUBCLASS(GtkArrow, GtkMisc, GTK_TYPE_ARROW)
#endif //ifdef GTK_TYPE_ARROW


#ifdef GTK_TYPE_IMAGE
MIGTK_SUBCLASS(GtkImage, GtkMisc, GTK_TYPE_IMAGE)
#endif //ifdef GTK_TYPE_IMAGE


#ifdef GTK_TYPE_ACCEL_LABEL
MIGTK_SUBCLASS(GtkAccelLabel, GtkLabel, GTK_TYPE_ACCEL_LABEL)
#endif //ifdef GTK_TYPE_ACCEL_LABEL


#ifdef GTK_TYPE_ASPECT_FRAME
MIGTK_SUBCLASS(GtkAspectFrame, GtkFrame, GTK_TYPE_ASPECT_FRAME)
#endif //ifdef GTK_TYPE_ASPECT_FRAME


#ifdef GTK_TYPE_VPANED
MIGTK_SUBCLASS(GtkVPaned, GtkPaned, GTK_TYPE_VPANED)
#endif //ifdef GTK_TYPE_VPANED


#ifdef GTK_TYPE_HPANED
MIGTK_SUBCLASS(GtkHPaned, GtkPaned, GTK_TYPE_HPANED)
#endif //ifdef GTK_TYPE_HPANED


#ifdef GTK_TYPE_MENU
MIGTK_SUBCLASS(GtkMenu, GtkMenuShell, GTK_TYPE_MENU)
#endif //ifdef GTK_TYPE_MENU


#ifdef GTK_TYPE_MENU_BAR
MIGTK_SUBCLASS(GtkMenuBar, GtkMenuShell, GTK_TYPE_MENU_BAR)
#endif //ifdef GTK_TYPE_MENU_BAR


#ifdef GTK_TYPE_TREE_VIEW_COLUMN

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkTreeViewColumn, GtkObject, GTK_TYPE_TREE_VIEW_COLUMN)

#else //if GTK_MAJOR_VERSION >= 3

MIGIU_SUBCLASS(GtkTreeViewColumn, GInitiallyUnowned, GTK_TYPE_TREE_VIEW_COLUMN)
#endif //if GTK_MAJOR_VERSION >= 3

// void clicked(GtkTreeViewColumn*)
#define MIGTK_TREE_VIEW_COLUMN_clicked(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkTreeViewColumn*>(obj), "clicked", void, GtkTreeViewColumn*, cls, fun, ptr)
#define MIGTK_S_TREE_VIEW_COLUMN_clicked(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeViewColumn*>(obj), "clicked", reinterpret_cast<GCallback>(static_cast<void(*)(GtkTreeViewColumn*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_TREE_VIEW_COLUMN


#ifdef GTK_TYPE_CELL_RENDERER

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkCellRenderer, GtkObject, GTK_TYPE_CELL_RENDERER)

#else //if GTK_MAJOR_VERSION >= 3

MIGIU_SUBCLASS(GtkCellRenderer, GInitiallyUnowned, GTK_TYPE_CELL_RENDERER)
#endif //if GTK_MAJOR_VERSION >= 3

// void editing_canceled(GtkCellRenderer*)
#define MIGTK_CELL_RENDERER_editing_canceled(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkCellRenderer*>(obj), "editing_canceled", void, GtkCellRenderer*, cls, fun, ptr)
#define MIGTK_S_CELL_RENDERER_editing_canceled(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkCellRenderer*>(obj), "editing_canceled", reinterpret_cast<GCallback>(static_cast<void(*)(GtkCellRenderer*, gpointer)>(fun)), ptr)
// void editing_started(GtkCellRenderer*, gchar*)
#define MIGTK_CELL_RENDERER_editing_started(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkCellRenderer*>(obj), "editing_started", void, GtkCellRenderer*, gchar*, cls, fun, ptr)
#define MIGTK_S_CELL_RENDERER_editing_started(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkCellRenderer*>(obj), "editing_started", reinterpret_cast<GCallback>(static_cast<void(*)(GtkCellRenderer*, gchar*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_CELL_RENDERER


#ifdef GTK_TYPE_CELL_RENDERER_TEXT

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkCellRendererText, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_TEXT)

#else //if GTK_MAJOR_VERSION >= 3

MIGIU_SUBCLASS(GtkCellRendererText, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_TEXT)
#endif //if GTK_MAJOR_VERSION >= 3

#endif //ifdef GTK_TYPE_CELL_RENDERER_TEXT


#ifdef GTK_TYPE_CELL_RENDERER_PIXBUF

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkCellRendererPixbuf, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_PIXBUF)

#else //if GTK_MAJOR_VERSION >= 3

MIGIU_SUBCLASS(GtkCellRendererPixbuf, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_PIXBUF)
#endif //if GTK_MAJOR_VERSION >= 3

#endif //ifdef GTK_TYPE_CELL_RENDERER_PIXBUF


#ifdef GTK_TYPE_CELL_RENDERER_PROGRESS

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkCellRendererProgress, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_PROGRESS)

#else //if GTK_MAJOR_VERSION >= 3

MIGIU_SUBCLASS(GtkCellRendererProgress, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_PROGRESS)
#endif //if GTK_MAJOR_VERSION >= 3

#endif //ifdef GTK_TYPE_CELL_RENDERER_PROGRESS


#ifdef GTK_TYPE_CELL_RENDERER_TOGGLE

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkCellRendererToggle, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_TOGGLE)

#else //if GTK_MAJOR_VERSION >= 3

MIGIU_SUBCLASS(GtkCellRendererToggle, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_TOGGLE)
#endif //if GTK_MAJOR_VERSION >= 3

// void toggled(GtkCellRendererToggle*, gchar*)
#define MIGTK_CELL_RENDERER_TOGGLE_toggled(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkCellRendererToggle*>(obj), "toggled", void, GtkCellRendererToggle*, gchar*, cls, fun, ptr)
#define MIGTK_S_CELL_RENDERER_TOGGLE_toggled(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkCellRendererToggle*>(obj), "toggled", reinterpret_cast<GCallback>(static_cast<void(*)(GtkCellRendererToggle*, gchar*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_CELL_RENDERER_TOGGLE


#ifdef GTK_TYPE_CELL_RENDERER_ACCEL

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkCellRendererAccel, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_ACCEL)

#else //if GTK_MAJOR_VERSION >= 3

MIGIU_SUBCLASS(GtkCellRendererAccel, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_ACCEL)
#endif //if GTK_MAJOR_VERSION >= 3

#endif //ifdef GTK_TYPE_CELL_RENDERER_ACCEL


#ifdef GTK_TYPE_CELL_RENDERER_COMBO

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkCellRendererCombo, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_COMBO)

#else //if GTK_MAJOR_VERSION >= 3

MIGIU_SUBCLASS(GtkCellRendererCombo, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_COMBO)
#endif //if GTK_MAJOR_VERSION >= 3

#endif //ifdef GTK_TYPE_CELL_RENDERER_COMBO


#ifdef GTK_TYPE_CELL_RENDERER_SPIN

#if GTK_MAJOR_VERSION < 3
MIGTK_SUBCLASS(GtkCellRendererSpin, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_SPIN)

#else //if GTK_MAJOR_VERSION >= 3

MIGIU_SUBCLASS(GtkCellRendererSpin, GtkCellRenderer, GTK_TYPE_CELL_RENDERER_SPIN)
#endif //if GTK_MAJOR_VERSION >= 3

#endif //ifdef GTK_TYPE_CELL_RENDERER_SPIN


#ifdef GTK_TYPE_TEXT_BUFFER
MIGLIB_SUBCLASS(GtkTextBuffer, GObject, GTK_TYPE_TEXT_BUFFER)
#endif //ifdef GTK_TYPE_TEXT_BUFFER


#ifdef GTK_TYPE_TREE_MODEL
MIGLIB_SUBCLASS(GtkTreeModel, GObject, GTK_TYPE_TREE_MODEL)
#endif //ifdef GTK_TYPE_TREE_MODEL


#ifdef GTK_TYPE_TREE_SORTABLE
MIGLIB_SUBCLASS(GtkTreeSortable, GObject, GTK_TYPE_TREE_SORTABLE)
#endif //ifdef GTK_TYPE_TREE_SORTABLE


#ifdef GTK_TYPE_LIST_STORE
MIGLIB_SUBCLASS_ITF_2(GtkListStore, GObject, GtkTreeModel, GtkTreeSortable, GTK_TYPE_LIST_STORE)
#endif //ifdef GTK_TYPE_LIST_STORE


#ifdef GTK_TYPE_TREE_STORE
MIGLIB_SUBCLASS_ITF_2(GtkTreeStore, GObject, GtkTreeModel, GtkTreeSortable, GTK_TYPE_TREE_STORE)
#endif //ifdef GTK_TYPE_TREE_STORE


#ifdef GTK_TYPE_TREE_SELECTION
MIGLIB_SUBCLASS(GtkTreeSelection, GObject, GTK_TYPE_TREE_SELECTION)
// void changed(GtkTreeSelection*)
#define MIGTK_TREE_SELECTION_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkTreeSelection*>(obj), "changed", void, GtkTreeSelection*, cls, fun, ptr)
#define MIGTK_S_TREE_SELECTION_changed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkTreeSelection*>(obj), "changed", reinterpret_cast<GCallback>(static_cast<void(*)(GtkTreeSelection*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_TREE_SELECTION


#ifdef GTK_TYPE_STATUS_ICON
MIGLIB_SUBCLASS(GtkStatusIcon, GObject, GTK_TYPE_STATUS_ICON)
// void activate(GtkStatusIcon*)
#define MIGTK_STATUS_ICON_activate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkStatusIcon*>(obj), "activate", void, GtkStatusIcon*, cls, fun, ptr)
#define MIGTK_S_STATUS_ICON_activate(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkStatusIcon*>(obj), "activate", reinterpret_cast<GCallback>(static_cast<void(*)(GtkStatusIcon*, gpointer)>(fun)), ptr)
// void popup_menu(GtkStatusIcon*, guint, guint32)
#define MIGTK_STATUS_ICON_popup_menu(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(static_cast<GtkStatusIcon*>(obj), "popup-menu", void, GtkStatusIcon*, guint, guint32, cls, fun, ptr)
#define MIGTK_S_STATUS_ICON_popup_menu(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkStatusIcon*>(obj), "popup-menu", reinterpret_cast<GCallback>(static_cast<void(*)(GtkStatusIcon*, guint, guint32, gpointer)>(fun)), ptr)
// gboolean size_changed(GtkStatusIcon*, gint)
#define MIGTK_STATUS_ICON_size_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(static_cast<GtkStatusIcon*>(obj), "size-changed", gboolean, GtkStatusIcon*, gint, cls, fun, ptr)
#define MIGTK_S_STATUS_ICON_size_changed(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkStatusIcon*>(obj), "size-changed", reinterpret_cast<GCallback>(static_cast<gboolean(*)(GtkStatusIcon*, gint, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_STATUS_ICON


#ifdef GTK_TYPE_UIMANAGER
MIGLIB_SUBCLASS(GtkUIManager, GObject, GTK_TYPE_UIMANAGER)
#endif //ifdef GTK_TYPE_UIMANAGER


#ifdef GTK_TYPE_ACTION_GROUP
MIGLIB_SUBCLASS_ITF_1(GtkActionGroup, GObject, GtkBuildable, GTK_TYPE_ACTION_GROUP)
#endif //ifdef GTK_TYPE_ACTION_GROUP


#ifdef GTK_TYPE_ICON_FACTORY
MIGLIB_SUBCLASS(GtkIconFactory, GObject, GTK_TYPE_ICON_FACTORY)
#endif //ifdef GTK_TYPE_ICON_FACTORY


#ifdef GTK_TYPE_SIZE_GROUP
MIGLIB_SUBCLASS(GtkSizeGroup, GObject, GTK_TYPE_SIZE_GROUP)
#endif //ifdef GTK_TYPE_SIZE_GROUP


#ifdef GTK_TYPE_ACTION
MIGLIB_SUBCLASS_ITF_1(GtkAction, GObject, GtkBuildable, GTK_TYPE_ACTION)
// void activate(GtkAction*)
#define MIGTK_ACTION_activate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(static_cast<GtkAction*>(obj), "activate", void, GtkAction*, cls, fun, ptr)
#define MIGTK_S_ACTION_activate(obj, fun, ptr) \
    g_signal_connect(static_cast<GtkAction*>(obj), "activate", reinterpret_cast<GCallback>(static_cast<void(*)(GtkAction*, gpointer)>(fun)), ptr)
#endif //ifdef GTK_TYPE_ACTION


#ifdef GTK_TYPE_TOGGLE_ACTION
MIGLIB_SUBCLASS(GtkToggleAction, GtkAction, GTK_TYPE_TOGGLE_ACTION)
#endif //ifdef GTK_TYPE_TOGGLE_ACTION


#ifdef GTK_TYPE_RADIO_ACTION
MIGLIB_SUBCLASS(GtkRadioAction, GtkToggleAction, GTK_TYPE_RADIO_ACTION)
#endif //ifdef GTK_TYPE_RADIO_ACTION


#ifdef GTK_TYPE_BUILDER
MIGTK_SUBCLASS(GtkBuilder, GObject, GTK_TYPE_BUILDER)
#endif //ifdef GTK_TYPE_BUILDER


#ifdef GDK_TYPE_PIXBUF
MIGLIB_SUBCLASS(GdkPixbuf, GObject, GDK_TYPE_PIXBUF)
#endif //ifdef GDK_TYPE_PIXBUF


#ifdef GDK_TYPE_GC
MIGLIB_SUBCLASS(GdkGC, GObject, GDK_TYPE_GC)
#endif //ifdef GDK_TYPE_GC


#ifdef GDK_TYPE_CURSOR

#if GTK_MAJOR_VERSION >= 3
MIGLIB_SUBCLASS(GdkCursor, GObject, GDK_TYPE_CURSOR)
#endif //if GTK_MAJOR_VERSION >= 3

#endif //ifdef GDK_TYPE_CURSOR


#if GTK_MAJOR_VERSION < 3
MIGLIB_REF_UNREF_PTR(GdkCursor, gdk_cursor_ref, gdk_cursor_unref)
#endif //if GTK_MAJOR_VERSION < 3

MIGLIB_REF_UNREF_PTR(GtkIconSet, gtk_icon_set_ref, gtk_icon_set_unref)
MIGLIB_UNREF_PTR(GtkTreePath, gtk_tree_path_free)
MIGLIB_UNREF_PTR(GtkTreeRowReference, gtk_tree_row_reference_free)
#define MIGTK_CELL_DATA_FUNC(cls, fun) \
    MIGLIB_FUNC_4(void, GtkTreeViewColumn*, GtkCellRenderer*, GtkTreeModel*, GtkTreeIter*, cls, fun)
#define MIGTK_CELL_DATA_SFUNC(fun) \
    MIGLIB_SFUNC_4(void, GtkTreeViewColumn*, GtkCellRenderer*, GtkTreeModel*, GtkTreeIter*, fun)
