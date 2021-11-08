
#ifndef CONFIG_H
#define CONFIG_H

/* This output flag enables sending of DNS
 * requests when pafish detects products.
 */
#define ENABLE_DNS_TRACE 1

/* This output flag enables writing traces of
 * detections to a PE section of the pafish
 * image in memory. Memory dumps of unpacked executables
 * may reveal the detected products.
 * Output format: "analysis-start trace1,trace2, analysis-end"
 */
#define ENABLE_PE_IMG_TRACE 1

#endif
