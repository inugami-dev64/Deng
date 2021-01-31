#ifndef MAIN_H
#define MAIN_H

#define DAS_EXT_HANDLER
#define DAS_GENERIC_HELPERS

#ifdef __linux__
    #include <dirent.h>
#endif

#ifdef _WIN32
    #include "../win32_dirent_dep/dirent.h"
#endif
#define DAM_VERSION "0.0.1"


#define TO_HELP printf("Use --help for more information\n")
char *help_text = 
    "General usage of dam:\n"
    " [ASSET] --info       Display information about asset\n"
    " -m [ASSET]           Create DENG new asset\n"
    "   -f / --file [MODEL]  Specify 3D model for asset creation\n"
    "   -n / --name [NAME]   Specify asset name\n"
    "   -t / --type v, vt, vn, vtr  Specify asset type\n";

char *repo_conf_path = "repos.conf";

// Method declarations
deng_ui16_t damCountMonths (
    deng_i64_t *p_time, 
    deng_ui16_t months, 
    int is_leap_year
);

void damListAsset(char *asset_file);

void damAssetAssemblyCaller (
    char *asset_name,
    char *file_path,
    char *out_file,
    char *asset_use_str
);

// Date and time formating
#define SECONDS_PER_YEAR 31536000 
#define SECONDS_PER_LEAP_YEAR 31622400
#define SECONDS_PER_31_DAY_MONTH 2678400
#define SECONDS_PER_30_DAY_MONTH 2592000
#define SECONDS_IN_FEBRUARY 2419200
#define SECONDS_IN_LEAP_YEAR_FEBRUARY 2505600
deng_ui16_t damCountYears (
    deng_i64_t *p_time, 
    deng_ui16_t years, 
    int n, 
    int *p_is_leap_year
);

deng_ui16_t damCountMonths (
    deng_i64_t *p_time, 
    deng_ui16_t months, 
    int is_leap_year
);

void damFormatDate (
    char *date, 
    char *time, 
    deng_i64_t time_from_epoch
);
#endif
