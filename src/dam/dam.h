#ifndef MAIN_H
#define MAIN_H

#define DAS_HANDLER_ONLY
#include "dam_core.h"
#include <dirent.h> 
#include <math.h>

#ifdef __linux__
    #include <dirent.h>
#endif

#ifdef _WIN32
    #include "../win32_dirent_dep/dirent.h"
#endif

static char *help_text = 
    "General usage of dam:\n"
    " --local         Use local asset repository\n"
    "   -l / --list     List all available local assets\n"
    "   --add-repo [DIR]  Add new repository path to repository list"
    "   --set-default-repo-id [DIR_ID]  Set default repository directory\n"
    "   --list-repos    List all available repositories\n"
    "   --assemble	   Assemble asset\n"
    "     -f / --frag         Specify the fragment type\n"
    "       [0] --color [R] [G] [B] [A]	Define static color as the fragment of the asset\n"
    "       [1] --tex-path [PATH]		Define texture as the fragment of the asset\n"
    "     -o / --obj-path    Specify the path to the model file\n";

static char *repo_conf_path = "local_repos.conf";

// Method declarations
static char *get_extension(char *file_name);
static void read_local_repo_conf(char ***ppp_repo_paths, size_t *p_repo_count, int *p_write_repo_id);
static void list_assets(char **pp_repo_paths, size_t repo_count);
static int add_repo(char **pp_repo_path, size_t *p_repo_count, char *new_repo, int default_id);
static void set_default_repo(char **pp_repo_paths, size_t repo_count, int id);
static void list_local_repos(char **pp_repo_paths, size_t repo_count, int default_id);
static void asset_assembly_caller(Asset *p_asset, char **pp_repo_paths, int repo_id);

// Date and time formating
#define SECONDS_PER_YEAR 31536000 
#define SECONDS_PER_LEAP_YEAR 31622400
#define SECONDS_PER_31_DAY_MONTH 2678400
#define SECONDS_PER_30_DAY_MONTH 2592000
#define SECONDS_IN_FEBRUARY 2419200
#define SECONDS_IN_LEAP_YEAR_FEBRUARY 2505600
static void format_date(char *date, char *time, int64_t time_from_epoch);
#endif