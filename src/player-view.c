/*
 * player-view.c
 * 
 * Copyright 2013 Ikey Doherty <ikey.doherty@gmail.com>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include "player-view.h"

G_DEFINE_TYPE_WITH_PRIVATE(PlayerView, player_view, GTK_TYPE_BIN);

/* Used only in append_track */
struct TrackAddStore {
        GtkListStore *model;
        GtkTreeIter *iter;
};

/* Initialisation */
static void player_view_class_init(PlayerViewClass *klass)
{
        GObjectClass *g_object_class;

        g_object_class = G_OBJECT_CLASS(klass);
        g_object_class->dispose = &player_view_dispose;
}

static void player_view_init(PlayerView *self)
{
        GtkWidget *scroller;
        GtkWidget *tree;
        GtkTreeViewColumn *column;
        GtkCellRenderer *cell_text;

        /* Create our TreeView */
        tree = gtk_tree_view_new();
        self->tree = tree;
        cell_text = gtk_cell_renderer_text_new();

        /* Name */
        column = gtk_tree_view_column_new_with_attributes("Name",
                cell_text, "text", PLAYER_COLUMN_NAME);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
        gtk_tree_view_column_set_resizable(column, TRUE);

        /* Time */
        column = gtk_tree_view_column_new_with_attributes("Time",
                cell_text, "text", PLAYER_COLUMN_TIME);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
        gtk_tree_view_column_set_resizable(column, TRUE);

        /* Artist */
        column = gtk_tree_view_column_new_with_attributes("Artist",
                cell_text, "text", PLAYER_COLUMN_ARTIST);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
        gtk_tree_view_column_set_resizable(column, TRUE);

        /* Album */
        column = gtk_tree_view_column_new_with_attributes("Album",
                cell_text, "text", PLAYER_COLUMN_ALBUM);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
        gtk_tree_view_column_set_resizable(column, TRUE);

        /* Genre */
        column = gtk_tree_view_column_new_with_attributes("Genre",
                cell_text, "text", PLAYER_COLUMN_GENRE);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
        gtk_tree_view_column_set_resizable(column, TRUE);

        scroller = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroller),
                GTK_SHADOW_ETCHED_IN);
        gtk_container_add(GTK_CONTAINER(scroller), tree);
        gtk_container_add(GTK_CONTAINER(self), scroller);
}

static void player_view_dispose(GObject *object)
{
        PlayerView *self;

        self = PLAYER_VIEW(object);

        /* Destruct */
        G_OBJECT_CLASS (player_view_parent_class)->dispose (object);
}

/* Utility; return a new PlayerView */
GtkWidget* player_view_new(void)
{
        PlayerView *self;

        self = g_object_new(PLAYER_VIEW_TYPE, NULL);
        return GTK_WIDGET(self);
}

/* Append a track to our model */
static void append_track(gpointer data, gpointer p_store)
{
        struct TrackAddStore* store;

        /* Currently only dealing with a string */
        gchar *track;

        track = (gchar*)data;
        store = (struct TrackAddStore*)p_store;

        gtk_list_store_append(store->model, &store->iter);
        gtk_list_store_set(store->model, &store->iter,
                PLAYER_COLUMN_NAME, track, -1);
}

void player_view_set_list(PlayerView *self, GSList* list)
{
        struct TrackAddStore store;

        if (!list)
                return;

        store.model = gtk_list_store_new(PLAYER_COLUMNS,
                G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                G_TYPE_STRING, G_TYPE_STRING);
        store.iter = 0;

        g_slist_foreach(list, &append_track, (gpointer)&store);
        gtk_tree_view_set_model(self->tree, store.model);
}
