#ifndef MAIN_H
#define MAIN_H

#define DAS_EXT_HANDLER
#define DAS_GENERIC_HELPERS

#include <math.h>

#ifdef __linux__
    #include <dirent.h>
#endif

#ifdef _WIN32
    #include "../win32_dirent_dep/dirent.h"
#endif

#include "das_core.h"

char *help_text = 
    "General usage of dam:\n"
    " --local         Use local asset repository\n"
    "   -l / --list     List all available local assets\n"
    "   --add-repo [DIR]  Add new repository path to repository list\n"
    "   --set-default-repo-id [DIR_ID]  Set default repository directory\n"
    "   --list-repos    List all available repositories\n"
    "   --assemble	   Assemble asset\n"
    "     -o / --obj-path    Specify the path to the model file\n";

char *repo_conf_path = "local_repos.conf";

// Method declarations
void damReadLocalRepoConf(char ***ppp_repo_paths, size_t *p_repo_count, int *p_write_repo_id);
void damListAssets(char **pp_repo_paths, size_t repo_count);
int damAddRepo(char **pp_repo_path, size_t *p_repo_count, char *new_repo, int default_id);
void damSetDefaultRepo(char **pp_repo_paths, size_t repo_count, int id);
void damListLocalRepos(char **pp_repo_paths, size_t repo_count, int default_id);
void damAssetAssemblyCaller(DENGAsset *p_asset, char **pp_repo_paths, int repo_id);

// Date and time formating
#define SECONDS_PER_YEAR 31536000 
#define SECONDS_PER_LEAP_YEAR 31622400
#define SECONDS_PER_31_DAY_MONTH 2678400
#define SECONDS_PER_30_DAY_MONTH 2592000
#define SECONDS_IN_FEBRUARY 2419200
#define SECONDS_IN_LEAP_YEAR_FEBRUARY 2505600
uint16_t damCountYears(int64_t *p_time, uint16_t years, int n, int *p_is_leap_year);
uint16_t damCountMonths(int64_t *p_time, uint16_t months, int is_leap_year);
void damFormatDate(char *date, char *time, int64_t time_from_epoch);
#endif