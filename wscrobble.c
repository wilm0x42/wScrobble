#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <gio/gio.h>

#include <mpv/client.h>

extern "C"
{
	int mpv_open_cplugin(mpv_handle* handle)
	{		
		mpv_observe_property(handle, 0, "path", MPV_FORMAT_STRING);

		while (true)
		{
			mpv_event* event = mpv_wait_event(handle, 1);
			
			if (event->event_id == MPV_EVENT_PROPERTY_CHANGE)
			{
				mpv_event_property* property = (mpv_event_property*)event->data;
				
				if (property->data == NULL)
					continue;
				
				char* file_path = *(char**)property->data;
				
				
				// Grab a GFile for the playing file

				GFile* file = g_file_new_for_path(file_path);
				GFileInfo* file_info = g_file_query_info(file, "*", G_FILE_QUERY_INFO_NONE, NULL, NULL);
				
				if (file_info == NULL)
				{
					printf("wScrobble: Failed to query file info\n");
					continue;
				}
				
				
				// Query the playcount attribute, and increment it
				
				const char* playcount_string = g_file_info_get_attribute_string(file_info, "metadata::playcount");
				
				if (playcount_string == NULL)
					playcount_string = "0";
				
				int playcount = strtol(playcount_string, NULL, 10);
				playcount++;
				
				char playcount_output_string[32];
				snprintf(playcount_output_string, 32, "%d", playcount);
				
				g_file_info_set_attribute_string(file_info, "metadata::playcount", playcount_output_string);
				
				
				// Update emblem depending on playcount
				
				char* output_emblems[4] = {
					"emblem-colors-white",
					"emblem-colors-blue",
					"emblem-colors-violet",
					NULL
				};
				
				if (playcount < 2)
					output_emblems[0] = NULL;
				if (playcount < 9)
					output_emblems[1] = NULL;
				if (playcount < 29)
					output_emblems[2] = NULL;
				
				g_file_info_set_attribute_stringv(file_info, "metadata::emblems", output_emblems);
				
				GError* error = NULL;
				g_file_set_attributes_from_info(file, file_info, G_FILE_QUERY_INFO_NONE, NULL, &error);
				
				//if (error)
				//	printf("wScrobble: Couldn't update playcount - %s\n", error->message);
				
				//printf("wScrobble: %d\n", playcount);
				
				
				// Clean up
				
				g_object_unref(file_info);
				g_object_unref(file);
			}
			
			if (event->event_id == MPV_EVENT_SHUTDOWN)
				break;
		}
		return 0;
	}
}
