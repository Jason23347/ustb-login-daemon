#include <stdio.h>
#include <getopt.h>
#include <curl/curl.h>
#include <libnotify/notify.h>
#include <readline/readline.h>

#include "debug.h"
#include "log.h"
#include "http_request.h"

int main(int argc, char *argv[])
{
	static struct option options[] = {
		{"no-ipv6", no_argument, NULL, 'n'},
	};
	int index = 0;
	char opt;

	/* default settings */
	bool has_ipv6 = 1;
	int time_gap = 60;

	while ((opt = getopt_long(argc, argv, "nt:", options, &index)) > 0){
		switch (opt) {
			case 'n':
				has_ipv6 = 0;
				break;
			case 't':
				time_gap = atof(optarg);
				if (time_gap < 10) {
					printf("time gap should be larger than 9.\n");
					exit(1);
				}
				break;
			case '?':
				break;
			default:
				debug("getopt returned character code 0%o\n", opt);
		}
	}

	debug((has_ipv6) ? "has ipv6" : "no ipv6");
	debug("time gap is: %d", time_gap);

	/* input username and password */
	char *username, *password;
	username = readline("username:");
	/* TODO: no echoing when input password */
	password = readline("password:");

	/* daemon starts here */
	if (daemon(1, 1) < 0)
	{
		perror("create daemon\n");
		exit(1);
	}

	write_log("daemon started.");

	char data[256];
	sprintf(data, "DDDDD=%s&upass=%s&0MKKey=123456789&v6ip=", username, password);
	if (has_ipv6) {
		strcat(data, get_ipv6_addr());
	}

	debug("data: %s", data);

#if (DEBUG == 1)
	int times = 0;
#endif

	while (1) {
		post_login(data);
		
#if (DEBUG == 1)
		debug("rendered for %d time(s)", times);
		if (++times > 2)
			break;
#endif
		/* TODO: reset time gap with response code */
		sleep(time_gap);
	}

	return 0;
}
