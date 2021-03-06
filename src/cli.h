#ifndef __NEREON_CMDLINE_H__
#define __NEREON_CMDLINE_H__

/*
 * parse command line
 */

int nereon_cli_parse(struct nereon_nos_option *nos_opts, int nos_opts_count, int argc, char **argv, bool *require_exit);

/*
 * print command line usage
 */

void nereon_cli_print_usage(struct nereon_nos_option *nos_opts, int nos_opts_count);

#endif /* __NEREON_CMDLINE_H__ */
