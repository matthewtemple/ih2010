#ifndef ih_matih_stats_h
#define ih_matih_stats_h

#include "ih/external/external.h"

double ih_matih_stats_get_precision(unsigned long true_positives,
        unsigned long false_positives);

double ih_matih_stats_get_recall(unsigned long true_positives,
        unsigned long false_negatives);

double ih_matih_stats_get_f_measure(double precision, double recall,
        double recall_weight);

void ih_matih_stats_get_prf(unsigned long true_positives,
        unsigned long false_positives, unsigned long false_negatives,
        double recall_weight, double *precision, double *recall,
        double *f_measure);

#endif
