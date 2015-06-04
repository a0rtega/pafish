
#ifndef WMI_H
#define WMI_H

/**
 * Structure that contains the name of the class to query and the respective
 * properties. Note that the maximum number of properties is forty.
 */
struct wmitarget {
	wchar_t *caption;
	wchar_t *classname;
	wchar_t *properties[40];
};

int wmi_initialize(IWbemServices **);

void wmi_cleanup(IWbemServices *);

int wmi_execute_query(IWbemServices *, wchar_t *, wchar_t *, wchar_t *[]);

#endif
