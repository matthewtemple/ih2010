#include "ih/config/tools.h"
#include "ih/core/constants.h"
#include "ih/core/period.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/starclient.h"
#include "ih/net/starclient_conf.h"

static void handle_signal(int signal);

void handle_signal(int signal)
{
  switch (signal) {
    case SIGUSR1:
      break;
    case SIGUSR2:
      break;
    default:
      ih_core_stop_requested = ih_core_bool_true;
      break;
  }
}

int main(int argc, char *argv[])
{
  ih_config_disable_if_running_batcih_tests(argc, argv);

  ih_net_starclient_t *starclient;
  ih_net_starclient_conf_t *starclient_conf;
  ih_core_period_t *period;
  void *custom_client_context;
  ih_audit_log_t *log;

  custom_client_context = NULL;

  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  log = ih_audit_log_create(stdout);
  if (!log) {
    ih_core_trace_exit("ih_audit_log_create");
  }

  period = ih_core_period_create(8);
  if (!period) {
    ih_core_trace_exit("ih_core_period_create");
  }

  starclient_conf = ih_net_starclient_conf_create("ih_net_starclient_test.conf");
  if (!starclient_conf) {
    ih_core_trace_exit("ih_net_starclient_conf_create");
  }

  starclient = ih_net_starclient_create(starclient_conf->star_arm_ips,
      starclient_conf->star_arm_port_min,
      starclient_conf->star_arm_port_max,
      starclient_conf->node_server_exclude_ip,
      starclient_conf->node_server_exclude_port,
      starclient_conf->node_server_exclude_port,
      custom_client_context, log);
  if (starclient) {
    if (ih_net_starclient_connect(starclient)) {
      printf("starclient connected to star\n");
      while (!ih_core_stop_requested) {
        ih_net_starclient_process_messages(starclient);

        /*
          TODO: move this to the starclient
        */
        if (!ih_net_starclient_star_available(starclient)) {
          if (ih_core_period_once(period)) {
            printf("star is unavailable\n");
          }
        }

        usleep(IH_CORE_STANDARD_SLEEP_MICROSECONDS);
      }
      printf("starclient shutting down gracefully\n");
    } else {
      printf("starclient could not connect to star\n");
    }
  } else {
    ih_core_trace("ih_net_starclient_create");
  }

  ih_net_starclient_destroy(starclient);
  ih_net_starclient_conf_destroy(starclient_conf);
  ih_core_period_destroy(period);
  ih_audit_log_destroy(log);

  return 0;
}
