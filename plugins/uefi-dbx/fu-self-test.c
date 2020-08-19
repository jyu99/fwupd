/*
 * Copyright (C) 2020 Richard Hughes <richard@hughsie.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#include "config.h"

#include <fwupd.h>

#include "fu-common.h"
#include "fu-uefi-dbx-common.h"
#include "fu-efi-image.h"
#include "fu-efi-signature-parser.h"

static gchar *
fu_test_get_filename (const gchar *filename)
{
	g_autofree gchar *path = NULL;
	path = g_build_filename (TESTDATADIR, filename, NULL);
	return fu_common_realpath (path, NULL);
}

static void
fu_efi_image_func (void)
{
	const gchar *csum = NULL;
	g_autofree gchar *fn = NULL;
	g_autoptr(FuEfiImage) img = NULL;
	g_autoptr(GBytes) bytes = NULL;
	g_autoptr(GError) error = NULL;

	fn = fu_test_get_filename ("fwupdx64.efi");
	g_assert_nonnull (fn);
	bytes = fu_common_get_contents_bytes (fn, &error);
	g_assert_no_error (error);
	g_assert_nonnull (bytes);

	img = fu_efi_image_new (bytes, &error);
	g_assert_no_error (error);
	g_assert_nonnull (img);
	csum = fu_efi_image_get_checksum (img);
	g_assert_cmpstr (csum, ==, "e99707d4378140c01eb3f867240d5cc9e237b126d3db0c3b4bbcd3da1720ddff");
}

int
main (int argc, char **argv)
{
	g_test_init (&argc, &argv, NULL);

	/* only critical and error are fatal */
	g_log_set_fatal_mask (NULL, G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL);
	g_setenv ("G_MESSAGES_DEBUG", "all", TRUE);

	/* tests go here */
	g_test_add_func ("/uefi-dbx/image", fu_efi_image_func);
	return g_test_run ();
}