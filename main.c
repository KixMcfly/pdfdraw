/*
 * main.c
 * 
 * Copyright 2022 Shawn Achimasi
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


#include <glib.h>
#include <poppler.h>
#include <cairo.h>

#define PARAM_ERROR "Need 1 param (PDF file name)"

int
main (int argc, char **argv)
{
	GFile *f = NULL;
	GError *err = NULL;
	PopplerDocument *doc = NULL;
	gchar *pdf_file_name = NULL;
	gint64 np;
	
	if (argc < 2){
		g_print (PARAM_ERROR);
		return 0;
	}
	
	PopplerRectangle *address = poppler_rectangle_new ();
	address->x1 = 5;
	address->y1 = 5;
	address->x2 = 100;
	address->y2 = 100;
	
	PopplerColor *color = poppler_color_new ();
	color->red = 0;
	color->blue = 150;
	color->green = 60;
	
	/* Form PDF file name from print job number */
	g_print ("PDF file name: %s\n", argv[1]);
	f = g_file_new_for_path (argv[1]);
	doc = poppler_document_new_from_gfile (f, NULL, NULL, NULL);
	g_object_unref (f);
	
	PopplerAnnot *annot_addr = poppler_annot_square_new (doc, address);
	 
	poppler_annot_square_set_interior_color ((PopplerAnnotSquare *)annot_addr, color);

	np = poppler_document_get_n_pages (doc);
	
	g_print ("Number of pages: %ld\n", np);
	
	for (gint64 i = 0; i < np; i++){
		PopplerPage *page = poppler_document_get_page (doc, i);
		
		poppler_page_add_annot (page, annot_addr);
		
		g_object_unref (doc);
		
	}
	
	poppler_document_save (doc, "file:///home/shawn/projects/pdfdraw/done.pdf", &err);
	
	if (err){
		g_print ("POPPLER ERROR: %s\n", err->message);
		g_error_free (err);
	}
	
	g_object_unref (doc);
	
	poppler_rectangle_free (address);
	poppler_color_free (color);
	
	doc = NULL;
	
	/* Ensure doc doesn't does not have any refs*/
	g_assert_null (doc);
	
	



	return 0;
}
