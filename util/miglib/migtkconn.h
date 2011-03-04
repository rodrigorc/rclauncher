#ifndef MIGTKCONN_H_INCLUDED
#define MIGTKCONN_H_INCLUDED

//#define MIGTK_XXX_?(obj, cls, fun, ptr)
//    MIGLIB_CONNECT_?(obj, "?", ?, GtkXXX*, ?, cls, fun, ptr)

#if GTK_MAJOR_VERSION < 3
//GtkObject
#define MIGTK_OBJECT_destroy(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "destroy", void, GtkObject*, cls, fun, ptr)
#endif

//GtkWidget
#define MIGTK_WIDGET_destroy(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "destroy", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_button_press_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "button-press-event", gboolean, GtkWidget*, GdkEventButton*, cls, fun, ptr)
#define MIGTK_WIDGET_button_release_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "button-release-event", gboolean, GtkWidget*, GdkEventButton*, cls, fun, ptr)
#define MIGTK_WIDGET_can_activate_accel(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "can-activate-accel", gboolean, GtkWidget*, guint, cls, fun, ptr)
#define MIGTK_WIDGET_child_notify(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "child-notify", void, GtkWidget*, GParamSpec*, cls, fun, ptr)
#define MIGTK_WIDGET_client_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "client-event", gboolean, GtkWidget*, GdkEventClient*, cls, fun, ptr)
#define MIGTK_WIDGET_composited_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "composited-changed", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_configure_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "configure-event", gboolean, GtkWidget*, GdkEventConfigure*, cls, fun, ptr)
#define MIGTK_WIDGET_delete_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "delete-event", gboolean, GtkWidget*, GdkEventAny*, cls, fun, ptr)
#define MIGTK_WIDGET_destroy_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "destroy-event", gboolean, GtkWidget*, GdkEventAny*, cls, fun, ptr)
#define MIGTK_WIDGET_direction_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "direction-changed", void, GtkWidget*, GtkTextDirection, cls, fun, ptr)
#define MIGTK_WIDGET_drag_begin(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "drag-begin", void, GtkWidget*, GdkDragContext*, cls, fun, ptr)
#define MIGTK_WIDGET_drag_data_delete(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "drag-data-delete", void, GtkWidget*, GdkDragContext*, cls, fun, ptr)
#define MIGTK_WIDGET_drag_data_get(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_5(obj, "drag-data-get", void, GtkWidget*, GdkDragContext*, GtkSelectionData*, guint, guint, cls, fun, ptr)
#define MIGTK_WIDGET_drag_data_received(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_7(obj, "drag-data-received", void, GtkWidget*, GdkDragContext*, gint, gint, GtkSelectionData*, guint, guint, cls, fun, ptr)
#define MIGTK_WIDGET_drag_drop(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_5(obj, "drag-drop", gboolean, GtkWidget*, GdkDragContext*, gint, gint, guint, cls, fun, ptr)
#define MIGTK_WIDGET_drag_end(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "drag-end", void, GtkWidget*, GdkDragContext*, cls, fun, ptr)
#define MIGTK_WIDGET_drag_leave(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(obj, "drag-leave", void, GtkWidget*, GdkDragContext*, guint, cls, fun, ptr)
#define MIGTK_WIDGET_drag_motion(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_5(obj, "drag-motion", gboolean, GtkWidget*, GdkDragContext*, gint, gint, guint, cls, fun, ptr)
#define MIGTK_WIDGET_enter_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "enter-notify-event", gboolean, GtkWidget*, GdkEventCrossing*, cls, fun, ptr)
#define MIGTK_WIDGET_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "event", gboolean, GtkWidget*, GdkEvent*, cls, fun, ptr)
#define MIGTK_WIDGET_focus(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "focus", gboolean, GtkWidget*, GtkDirectionType, cls, fun, ptr)
#define MIGTK_WIDGET_focus_in_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "focus-in-event", gboolean, GtkWidget*, GdkEventFocus*, cls, fun, ptr)
#define MIGTK_WIDGET_focus_out_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "focus-out-event", gboolean, GtkWidget*, GdkEventFocus*, cls, fun, ptr)
#define MIGTK_WIDGET_grab_broken_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "grab-broken-event", gboolean, GtkWidget*, GdkEventGrabBroken*, cls, fun, ptr)
#define MIGTK_WIDGET_grab_focus(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "grab-focus", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_grab_notify(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "grab-notify", void, GtkWidget*, gboolean, cls, fun, ptr)
#define MIGTK_WIDGET_hide(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "hide", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_hierarchy_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "hierarchy-changed", void, GtkWidget*, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_key_press_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "key-press-event", gboolean, GtkWidget*, GdkEventKey*, cls, fun, ptr)
#define MIGTK_WIDGET_key_release_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "key-release-event", gboolean, GtkWidget*, GdkEventKey*, cls, fun, ptr)
#define MIGTK_WIDGET_leave_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "leave-notify-event", gboolean, GtkWidget*, GdkEventCrossing*, cls, fun, ptr)
#define MIGTK_WIDGET_map(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "map", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_map_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "map-event", gboolean, GtkWidget*, GdkEventAny*, cls, fun, ptr)
#define MIGTK_WIDGET_mnemonic_activate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "mnemonic-activate", gboolean, GtkWidget*, gboolean, cls, fun, ptr)
#define MIGTK_WIDGET_motion_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "motion-notify-event", gboolean, GtkWidget*, GdkEventMotion*, cls, fun, ptr)
#define MIGTK_WIDGET_parent_set(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "parent-set", void, GtkWidget*, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_popup_menu(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "popup-menu", gboolean, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_property_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "property-notify-event", gboolean, GtkWidget*, GdkEventProperty*, cls, fun, ptr)
#define MIGTK_WIDGET_proximity_in_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "proximity-in-event", gboolean, GtkWidget*, GdkEventProximity*, cls, fun, ptr)
#define MIGTK_WIDGET_proximity_out_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "proximity-out-event", gboolean, GtkWidget*, GdkEventProximity*, cls, fun, ptr)
#define MIGTK_WIDGET_realize(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "realize", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_screen_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "screen-changed", void, GtkWidget*, GdkScreen*, cls, fun, ptr)
#define MIGTK_WIDGET_scroll_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "scroll-event", gboolean, GtkWidget*, GdkEventScroll*, cls, fun, ptr)
#define MIGTK_WIDGET_selection_clear_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "selection-clear-event", gboolean, GtkWidget*, GdkEventSelection*, cls, fun, ptr)
#define MIGTK_WIDGET_selection_get(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_4(obj, "selection-get", void, GtkWidget*, GtkSelectionData*, guint, guint, cls, fun, ptr)
#define MIGTK_WIDGET_selection_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "selection-notify-event", gboolean, GtkWidget*, GdkEventSelection*, cls, fun, ptr)
#define MIGTK_WIDGET_selection_received(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(obj, "selection-received", void, GtkWidget*, GtkSelectionData*, guint, cls, fun, ptr)
#define MIGTK_WIDGET_selection_request_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "selection-request-event", gboolean, GtkWidget*, GdkEventSelection*, cls, fun, ptr)
#define MIGTK_WIDGET_show(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "show", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_show_help(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "show-help", gboolean, GtkWidget*, GtkWidgetHelpType, cls, fun, ptr)
#define MIGTK_WIDGET_size_allocate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "size-allocate", void, GtkWidget*, GtkAllocation*, cls, fun, ptr)
#define MIGTK_WIDGET_size_request(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "size-request", void, GtkWidget*, GtkRequisition*, cls, fun, ptr)
#define MIGTK_WIDGET_state_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "state-changed", void, GtkWidget*, GtkStateType, cls, fun, ptr)
#define MIGTK_WIDGET_style_set(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "style-set", void, GtkWidget*, GtkStyle*, cls, fun, ptr)
#define MIGTK_WIDGET_unmap(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "unmap", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_unmap_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "unmap-event", gboolean, GtkWidget*, GdkEventAny*, cls, fun, ptr)
#define MIGTK_WIDGET_unrealize(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "unrealize", void, GtkWidget*, cls, fun, ptr)
#define MIGTK_WIDGET_visibility_notify_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "visibility-notify-event", gboolean, GtkWidget*, GdkEventVisibility*, cls, fun, ptr)
#define MIGTK_WIDGET_window_state_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "window-state-event", gboolean, GtkWidget*, GdkEventWindowState*, cls, fun, ptr)

#if GTK_MAJOR_VERSION < 3
#define MIGTK_WIDGET_expose_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "expose-event", gboolean, GtkWidget*, GdkEventExpose*, cls, fun, ptr)
#define MIGTK_WIDGET_no_expose_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "no-expose-event", gboolean, GtkWidget*, GdkEventAny*, cls, fun, ptr)
#else
#define MIGTK_WIDGET_draw(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "draw", gboolean, GtkWidget*, cairo_t*, cls, fun, ptr)
#endif

//GtkContainer
#define MIGTK_CONTAINER_add(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "add", void, GtkContainer*, GtkWidget*, cls, fun, ptr)
#define MIGTK_CONTAINER_remove(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "remove", void, GtkContainer*, GtkWidget*, cls, fun, ptr)
#define MIGTK_CONTAINER_check_resize(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "check-resize", void, GtkContainer*, cls, fun, ptr)
#define MIGTK_CONTAINER_set_focus_child(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "set-focus-child", void, GtkContainer*, GtkWidget*, cls, fun, ptr)

//GtkBin

//GtkWindow
#define MIGTK_WINDOW_set_focus(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "set-focus", void, GtkWindow*, GtkWidget*, cls, fun, ptr)
#define MIGTK_WINDOW_frame_event(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "frame-event", gboolean, GtkWindow*, GdkEvent*, cls, fun, ptr)
#define MIGTK_WINDOW_activate_focus(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "activate-focus", void, GtkWindow*, cls, fun, ptr)
#define MIGTK_WINDOW_activate_default(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "activate-default", void, GtkWindow*, cls, fun, ptr)
#define MIGTK_WINDOW_move_focus(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "move-focus", void, GtkWindow*, GtkDirectionType, cls, fun, ptr)
#define MIGTK_WINDOW_keys_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "keys-changed", void, GtkWindow*, cls, fun, ptr)

//GtkDialog
#define MIGTK_DIALOG_close(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "close", void, GtkDialog*, cls, fun, ptr)
#define MIGTK_DIALOG_response(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "response", void, GtkDialog*, gint, cls, fun, ptr)

//GtkTreeView
#define MIGTK_TREE_VIEW_columns_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "columns-changed", void, GtkTreeView*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_cursor_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "cursor-changed", void, GtkTreeView*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_expand_collapse_cursor_row(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_4(obj, "expand-collapse-cursor-row", gboolean, GtkTreeView*, gboolean, gboolean, gboolean, cls, fun , ptr)
#define MIGTK_TREE_VIEW_move_cursor(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(obj, "move-cursor", gboolean, GtkTreeView*, GtkMovementStep, gint, cls, fun, ptr)
#define MIGTK_TREE_VIEW_row_activated(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(obj, "row-activated", void, GtkTreeView*, GtkTreePath*, GtkTreeViewColumn*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_row_collapsed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(obj, "row-collapsed", void, GtkTreeView*, GtkTreeIter*, GtkTreePath*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_row_expanded(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(obj, "row-expanded", void, GtkTreeView*, GtkTreeIter*, GtkTreePath*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_select_all(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "select-all", gboolean, GtkTreeView*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_select_cursor_parent(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "select-cursor-parent", gboolean, GtkTreeView*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_select_cursor_row(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "select-cursor-row", gboolean, GtkTreeView*, gboolean, cls, fun, ptr)
#define MIGTK_TREE_VIEW_set_scroll_adjustments(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(obj, "set-scroll-adjustments", void, GtkTreeView*, GtkAdjustment*, GtkAdjustment*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_start_interactive_search(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "start-interactive-search", gboolean, GtkTreeView*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_test_collapse_row(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(obj, "test-collapse-row", gboolean, GtkTreeView*, GtkTreeIter*, GtkTreePath*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_test_expand_row(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(obj, "test-expand-row", gboolean, GtkTreeView*, GtkTreeIter*, GtkTreePath*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_toggle_cursor_row(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "toggle-cursor-row", gboolean, GtkTreeView*, cls, fun, ptr)
#define MIGTK_TREE_VIEW_unselect_all(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "unselect_all", gboolean, GtkTreeView*, cls, fun, ptr)

//GtkTreeViewColumn
#define MIGTK_TREE_VIEW_COLUMN_clicked(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "clicked", void, GtkTreeViewColumn*, cls, fun, ptr)

//GtkTreeSelection
#define MIGTK_TREE_SELECTION_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "changed", void, GtkTreeSelection*, cls, fun, ptr)

//GtkStatusIcon
#define MIGTK_STATUS_ICON_activate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "activate", void, GtkStatusIcon*, cls, fun, ptr)
#define MIGTK_STATUS_ICON_popup_menu(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_3(obj, "popup-menu", void, GtkStatusIcon*, guint, guint32, cls, fun, ptr)
#define MIGTK_STATUS_ICON_size_changed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "size-changed", gboolean, GtkStatusIcon*, gint, cls, fun, ptr)

//GtkMenuItem
#define MIGTK_MENU_ITEM_activate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "activate", void, GtkMenuItem*, cls, fun, ptr)
#define MIGTK_MENU_ITEM_activate_item(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "activate-item", void, GtkMenuItem*, cls, fun, ptr)
#define MIGTK_MENU_ITEM_toggle_size_allocate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "toggle-size-request", void, GtkMenuItem*, gint, cls, fun, ptr)
#define MIGTK_MENU_ITEM_toggle_size_request(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "toggle-size-allocate", void, GtkMenuItem*, gint, cls, fun, ptr)

//GtkAction
#define MIGTK_ACTION_activate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "activate", void, GtkAction*, cls, fun, ptr)

//GtkButton
#define MIGTK_BUTTON_activate(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "activate", void, GtkButton*, cls, fun, ptr)
#define MIGTK_BUTTON_clicked(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "clicked", void, GtkButton*, cls, fun, ptr)
#define MIGTK_BUTTON_enter(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "enter", void, GtkButton*, cls, fun, ptr)
#define MIGTK_BUTTON_leave(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "leave", void, GtkButton*, cls, fun, ptr)
#define MIGTK_BUTTON_pressed(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "pressed", void, GtkButton*, cls, fun, ptr)
#define MIGTK_BUTTON_released(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "released", void, GtkButton*, cls, fun, ptr)

//GtkLabel
#define MIGTK_LABEL_move_cursor(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_4(obj, "move_cursor", void, GtkLabel*, GtkMovementStep, gint, gboolean, cls, fun, ptr)
#define MIGTK_LABEL_copy_clipboard(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "copy_clipboard", void, GtkLabel*, cls, fun, ptr)
#define MIGTK_LABEL_populate_popup(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "populate_popup", void, GtkLabel*, GtkMenu, cls, fun, ptr)

//GtkEntry
#define MIGTK_ENTRY_activate(obj, cls, fun, ptr) \
 MIGLIB_CONNECT_1(obj, "activate", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_ENTRY_move_cursor(obj, cls, fun, ptr) \
 MIGLIB_CONNECT_4(obj, "move_cursor", void, GtkEntry*, GtkMovementStep, gint , gboolean, cls, fun, ptr)
#define MIGTK_ENTRY_insert_at_cursor(obj, cls, fun, ptr) \
 MIGLIB_CONNECT_2(obj, "insert_at_cursor", void, GtkEntry*, const gchar *, cls, fun, ptr)
#define MIGTK_ENTRY_delete_from_cursor(obj, cls, fun, ptr) \
 MIGLIB_CONNECT_3(obj, "delete_from_cursor", void, GtkEntry*, GtkDeleteType, gint, cls, fun, ptr)
#define MIGTK_ENTRY_backspace(obj, cls, fun, ptr) \
 MIGLIB_CONNECT_1(obj, "backspace", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_ENTRY_cut_clipboard(obj, cls, fun, ptr) \
 MIGLIB_CONNECT_1(obj, "cut_clipboard", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_ENTRY_copy_clipboard(obj, cls, fun, ptr) \
 MIGLIB_CONNECT_1(obj, "copy_clipboard", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_ENTRY_paste_clipboard(obj, cls, fun, ptr) \
 MIGLIB_CONNECT_1(obj, "paste_clipboard", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_ENTRY_toggle_overwrite(obj, cls, fun, ptr) \
 MIGLIB_CONNECT_1(obj, "toggle_overwrite", void, GtkEntry*, cls, fun, ptr)
#define MIGTK_ENTRY_get_text_area_size(obj, cls, fun, ptr) \
 MIGLIB_CONNECT_5(obj, "get_text_area_size", void, GtkEntry*, gint*, gint*, gint*, gint*, cls, fun, ptr)

//GtkCellRenderer
#define MIGTK_CELL_RENDERER_editing_canceled(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_1(obj, "editing_canceled", void, GtkCellRenderer*, cls, fun, ptr)
#define MIGTK_CELL_RENDERER_editing_started(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "editing_started", void, GtkCellRenderer*, gchar*, cls, fun, ptr)

//GtkCellRendererToggle
#define MIGTK_CELL_RENDERER_TOGGLE_toggled(obj, cls, fun, ptr) \
    MIGLIB_CONNECT_2(obj, "toggled", void, GtkCellRendererToggle*, gchar*, cls, fun, ptr)

#endif //MIGTKCONN_H_INCLUDED
