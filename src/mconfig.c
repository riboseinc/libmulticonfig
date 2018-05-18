/*
 * Copyright (c) 2017, [Ribose Inc](https://www.ribose.com).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

#include "hcl.h"
#include "cmdline.h"
#include "err.h"
#include "util.h"
#include "cfg.h"

#include "mconfig.h"

/*
 * Intiialize multiconfig context object
 */

int mconfig_ctx_init(mconfig_ctx_t *mctx, const char *hcl_options)
{
	struct mcfg_hcl_options *mcfg_opts = NULL;
	int mcfg_opts_count = 0;

	DEBUG_PRINT("Initializing multiconfig\n");

	memset(mctx, 0, sizeof(mconfig_ctx_t));

	/* parse HCL options */
	if (mcfg_parse_hcl_options(hcl_options, &mcfg_opts, &mcfg_opts_count) != 0) {
		DEBUG_PRINT("Failed to parse HCL options(err:%s)\n", mcfg_get_err());
		return -1;
	}

	mctx->hcl_opts = (void *)mcfg_opts;
	mctx->hcl_opts_count = mcfg_opts_count;

	return 0;
}

/*
 * Finalize multiconfig context object
 */

void mconfig_ctx_finalize(mconfig_ctx_t *mctx)
{
	struct mcfg_hcl_options *hcl_opts = (struct mcfg_hcl_options *)mctx->hcl_opts;
	struct mcfg_cfg_options *cfg_opts = (struct mcfg_cfg_options *)mctx->cfg_opts;

	mconfig_free_cfg_options(cfg_opts);
	mcfg_free_hcl_options(hcl_opts, mctx->hcl_opts_count);
}

/*
 * Parse configuration file
 */

int mconfig_parse_config(mconfig_ctx_t *mctx, const char *cfg_fpath)
{
	struct mcfg_cfg_options *cfg_opt = NULL;
	char *cfg_hcl = NULL;

	int ret;

	DEBUG_PRINT("Parsing configuration file '%s'\n", cfg_fpath);

	/* get configuration contents */
	if (read_file_contents(cfg_fpath, &cfg_hcl) == 0) {
		mcfg_set_err("Could not read configuration file '%s'", cfg_fpath);
		return -1;
	}

	ret = mconfig_parse_cfg_options(cfg_hcl, &cfg_opt);
	if (ret == 0) {
		mctx->cfg_opts = (void *)cfg_opt;
	}
	free(cfg_hcl);

	return ret;
}

/*
 * Parse command line arguments
 */

int mconfig_parse_cmdline(mconfig_ctx_t *mctx, int argc, char **argv)
{
	struct mcfg_hcl_options *hcl_opts = (struct mcfg_hcl_options *)mctx->hcl_opts;

	return mcfg_parse_cmdline(hcl_opts, mctx->hcl_opts_count, argc, argv);
}

/*
 * Print multiconfig command line usage message
 */

void mconfig_print_usage(mconfig_ctx_t *mctx)
{
	struct mcfg_hcl_options *hcl_opts = (struct mcfg_hcl_options *)mctx->hcl_opts;

	mcfg_print_cmdline_usage(hcl_opts, mctx->hcl_opts_count);
}

/*
 * Get the last error message of library
 */

const char *mconfig_get_errmsg()
{
	return mcfg_get_err();
}
