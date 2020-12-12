#include "dam.h"

/* Count years recursively */
uint16_t damCountYears(int64_t *p_time, uint16_t years, int n, int *p_is_leap_year) {
    if(n < 3 && (*p_time) - SECONDS_PER_YEAR >= 0) {
        n++;
        years++;
        (*p_time) -= SECONDS_PER_YEAR;
        *p_is_leap_year = false;
        return damCountYears(p_time, years, n, p_is_leap_year);
    }
    
    else if((*p_time) - SECONDS_PER_LEAP_YEAR >= 0) {
        years++;
        (*p_time) -= SECONDS_PER_LEAP_YEAR;
        n = 0;
        *p_is_leap_year = true;
        return damCountYears(p_time, years, n, p_is_leap_year);
    }

    return years;
}


/* Count months recursively */
uint16_t damCountMonths(int64_t *p_time, uint16_t months, int is_leap_year) {
    if(months <= 12) {
        // Check if month has 31 days
        if(months % 2 && (*p_time) - SECONDS_PER_31_DAY_MONTH >= 0) {
            months++;
            (*p_time) -= SECONDS_PER_31_DAY_MONTH;
            return damCountMonths(p_time, months, is_leap_year);
        } 

        // Check if month has 30 days
        else if(!(months % 2) && months != 2 && (*p_time) - SECONDS_PER_30_DAY_MONTH >= 0) {
            months++;
            (*p_time) -= SECONDS_PER_30_DAY_MONTH;
            return damCountMonths(p_time, months, is_leap_year);
        }

        // No leap year february
        else if(months == 2 && !is_leap_year && (*p_time) - SECONDS_IN_FEBRUARY >= 0) {
            months++;
            (*p_time) -= SECONDS_IN_FEBRUARY;
            return damCountMonths(p_time, months, is_leap_year);
        }

        // Leap year february
        else if(months == 2 && is_leap_year && (*p_time) - SECONDS_IN_LEAP_YEAR_FEBRUARY >= 0) {
            months++;
            (*p_time) -= SECONDS_IN_LEAP_YEAR_FEBRUARY;
            return damCountMonths(p_time, months, is_leap_year);
        }
    }

    return months;
}


/* Format date from time from epoch */
void damFormatDate(char *date, char *time, int64_t time_from_epoch) {
    int is_leap_year;
    char *ch_month, *ch_day;
    ch_month = (char*) calloc(2, sizeof(char));
    ch_day = (char*) calloc(2, sizeof(char));
    
    char *ch_hour, *ch_minute, *ch_second;
    ch_hour = (char*) calloc(2, sizeof(char));
    ch_minute = (char*) calloc(2, sizeof(char));
    ch_second = (char*) calloc(2, sizeof(char));

    // Count years since epoch
    time_from_epoch -= 2 * SECONDS_PER_YEAR;
    uint16_t year_count = 1972 + damCountYears(&time_from_epoch, 0, 0, &is_leap_year);

    // Count months from remaining time
    uint16_t month = damCountMonths(&time_from_epoch, 1, is_leap_year);
    if(month < 10) sprintf(ch_month, "0%d", month);
    else sprintf(ch_month, "%d", month);

    // Count day
    uint16_t day = (uint16_t) (time_from_epoch / 86400) + 1;
    time_from_epoch %= 86400;
    if(day < 10) sprintf(ch_day, "0%d", day);
    else sprintf(ch_day, "%d", day);
    
    sprintf(date, "%d/%s/%s", year_count, ch_month, ch_day);
    free(ch_month);
    free(ch_day);

    // Count hours
    uint16_t hour = (uint16_t) (time_from_epoch / 3600);
    time_from_epoch %= 3600;
    if(hour < 10) sprintf(ch_hour, "0%d", hour); 
    else sprintf(ch_hour, "%d", hour);

    // Count minutes
    uint16_t minute = (uint16_t) (time_from_epoch / 60);
    time_from_epoch %= 60;
    if(minute < 10) sprintf(ch_minute, "0%d", minute);
    else sprintf(ch_minute, "%d", minute);

    if(time_from_epoch < 10) sprintf(ch_second, "0%ld", time_from_epoch);
    else sprintf(ch_second, "%ld", time_from_epoch);
    
    sprintf(time, "%s:%s:%s", ch_hour, ch_minute, ch_second);
    free(ch_hour);
    free(ch_minute);
    free(ch_second);
}


/* Read local repository paths */
void damReadLocalRepoConf(char ***ppp_repo_path, size_t *p_repo_count, int *p_write_repo_id) {
    size_t l_index, r_index;
    int is_default_found = false;
    int is_white_space_handled;

    FILE *file = fopen(repo_conf_path, "rb");
    // Create new config file if not created
    if(!file) {
        file = fopen(repo_conf_path, "w");
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = (size_t) ftell(file);
    fseek(file, 0, SEEK_SET);

    if(file_size == 0) {
        fclose(file);
        return;
    }

    long *p_new_line_position = (long*) malloc(sizeof(long));
    *p_repo_count = 0;
    char default_ch[7];
    size_t pos_before_white_space;
    char *buffer;
    char ch;

    // Find the new line positions in config file
    while(ftell(file) < file_size) {
        fread(&ch, 1, 1, file);
        if(ch == 0x0A) {
            (*p_repo_count)++;
            p_new_line_position = (long*) realloc(p_new_line_position, (*p_repo_count) * sizeof(long));
            p_new_line_position[(*p_repo_count) - 1] = (size_t) ftell(file) - 1;
        }
    }

    (*p_repo_count)++;
    p_new_line_position = (long*) realloc(p_new_line_position, (*p_repo_count) * sizeof(long));
    p_new_line_position[(*p_repo_count) - 1] = file_size;
    *ppp_repo_path = (char**) calloc((*p_repo_count), sizeof(char*));
    
    fseek(file, 0, SEEK_SET);

    // Read repository paths
    for(l_index = 0; l_index < (*p_repo_count); l_index++) {
        is_white_space_handled = false;
        (*ppp_repo_path)[l_index] = (char*) calloc((size_t) (p_new_line_position[l_index] - ftell(file)), sizeof(char));

        fread((*ppp_repo_path)[l_index], sizeof(char), p_new_line_position[l_index] - ftell(file), file);
        fseek(file, 1L, SEEK_CUR);
        
        // Find whitespace
        ch = 0x00;
        for(r_index = 0; r_index < strlen((*ppp_repo_path)[l_index]) && ch != 0x20; r_index++)
            ch = (*ppp_repo_path)[l_index][r_index];

        if(ch == 0x20) pos_before_white_space = r_index - 1;
        else is_white_space_handled = true;

        // Check if current repo is default
        if(!is_default_found) {
            // Skip through whitespaces
            for(; r_index < strlen((*ppp_repo_path)[l_index]) && ch == 0x20; r_index++)
                ch = (*ppp_repo_path)[l_index][r_index];
            
            default_ch[0] = ch;
            // Add rest of the chars to default_ch to see if current repository is marked as default
            for(size_t default_i = 1; default_i < 7 && r_index < strlen((*ppp_repo_path)[l_index]); r_index++, default_i++)
                default_ch[default_i] = (*ppp_repo_path)[l_index][r_index];
            
            // Check if repository is default and if needed extract repo name out of it
            if(!strcmp(default_ch, "default")) {
                is_default_found = true;
                is_white_space_handled = true;

                *p_write_repo_id = l_index + 1;
                
                if((*ppp_repo_path)[l_index][pos_before_white_space - 1] != '/') {
                    buffer = (char*) calloc(pos_before_white_space + 1, sizeof(char));
                    buffer[pos_before_white_space] = '/';
                }
            
                else
                    buffer = (char*) calloc(pos_before_white_space, sizeof(char));

                for(r_index = 0; r_index < pos_before_white_space; r_index++)
                    buffer[r_index] = (*ppp_repo_path)[l_index][r_index];

                free((*ppp_repo_path)[l_index]);
                (*ppp_repo_path)[l_index] = (char*) calloc(pos_before_white_space, sizeof(char));
                strncpy((*ppp_repo_path)[l_index], buffer, strlen(buffer));
                free(buffer);
            }
        }

        // Remove unnessecary whitespaces if needed
        if(!is_white_space_handled) {
            if((*ppp_repo_path)[l_index][pos_before_white_space] != '/') {
                buffer = (char*) calloc(pos_before_white_space + 1, sizeof(char));
                buffer[pos_before_white_space] = '/';
            }
            
            else
                buffer = (char*) calloc(pos_before_white_space, sizeof(char));

            
            for(r_index = 0; r_index < pos_before_white_space; r_index++)
                buffer[r_index] = (*ppp_repo_path)[l_index][r_index];
            
            free((*ppp_repo_path)[l_index]);
            (*ppp_repo_path)[l_index] = calloc(pos_before_white_space, sizeof(char));
            strncpy((*ppp_repo_path)[l_index], buffer, pos_before_white_space);
            free(buffer);
        }
    }       

    // Clean new line indices pointer
    free(p_new_line_position);
    
    fclose(file);

    return;
}


/* List all available assets in local repositories */
void damListAssets(char **pp_repo_paths, size_t repo_count) {
    DIR *dir;
    FILE *file;

    size_t l_index, r_index;

    char **pp_repo_contents = (char**) malloc(sizeof(char*));
    size_t repo_contents_size = 0;
    struct dirent *dir_contents = NULL;
    char *file_ext = NULL;
    char *file_path = NULL;
    int found_assets = false;

    // Iterate through local repositories
    for(l_index = 0; l_index < repo_count; l_index++) {
        dir = opendir(pp_repo_paths[l_index]);

        if(!dir) {
            printf("ERROR: Failed to open repository directory %s%s\n", pp_repo_paths[l_index], "!");
            printf("Path does not exist!\n");
            return;
        }

        // Read all files in repositories and select ones with .das extension
        while((dir_contents = readdir(dir)) != NULL) {
            if(dir_contents->d_type == DT_REG) {
                file_ext = cm_GetFileExtName(dir_contents->d_name);
               
                if(file_ext && !strcmp(file_ext, "das")) {
                    repo_contents_size++;
                    pp_repo_contents = (char**) realloc((void*) pp_repo_contents, sizeof(char*));
                    pp_repo_contents[repo_contents_size - 1] = (char*) calloc(strlen(dir_contents->d_name), sizeof(char));
                    strncpy(pp_repo_contents[repo_contents_size - 1], dir_contents->d_name, strlen(dir_contents->d_name));
                }
            }
        }

        // Clean file extension
        if(file_ext) free(file_ext);

        closedir(dir);

        // Display information about the asset
        if(repo_contents_size > 0) found_assets = true;
        for(r_index = 0; r_index < repo_contents_size; r_index++) {
            // Create correct file path to read data from
            if(pp_repo_paths[l_index][strlen(pp_repo_paths[l_index]) - 1] == '/') {
                file_path = (char*) calloc(strlen(pp_repo_paths[l_index]) + strlen(pp_repo_contents[r_index]) - 1, sizeof(char));
                sprintf(file_path, "%s%s", pp_repo_paths[l_index], pp_repo_contents[r_index]);
            }

            else {
                file_path = (char*) calloc(strlen(pp_repo_paths[l_index]) + strlen(pp_repo_contents[r_index]), sizeof(char));
                sprintf(file_path, "%s/%s", pp_repo_paths[l_index], pp_repo_contents[r_index]);
            }

            char *asset_name = 0;
            char *asset_description = 0;
            uint64_t time_point;

            file = fopen(file_path, "rb");
            dasReadINFOHDR(&asset_name, &asset_description, &time_point, file_path, file);

            printf("\n[%s]\n", pp_repo_contents[r_index]);
            printf("Full path: %s\n", file_path);
            printf("Asset name: %s\n", asset_name);
            printf("Asset description: %s\n", asset_description);
            
            int64_t signed_time_stamp = (int64_t) time_point;
            char date[11], time[9];
            damFormatDate(date, time, signed_time_stamp);
            
            printf("Date of creation: %s\n", date);
            printf("Time of creation: %s(UTC)\n", time);


            if(file_path != NULL) free(file_path);
            if(asset_name != NULL) free(asset_name);
            if(asset_description != NULL) free(asset_description);
        }

        // Cleanup the repo contents
        for(r_index = 0; r_index < repo_contents_size; r_index++) 
            if(pp_repo_contents[r_index] != NULL) free(pp_repo_contents[r_index]);
        
        free(pp_repo_contents);
        pp_repo_contents = (char**) malloc(sizeof(char*));
        repo_contents_size = 0;
    }

    free(pp_repo_contents);

    if(!found_assets)
        printf("No assets available in local repositories!\n");
}


/* Add new local repository path to the config file */
int damAddRepo(char **pp_repo_path, size_t *p_repo_count, char *new_repo, int default_id) {
    size_t index;
    FILE *file;
    char *buffer;
    
    // Check if slash exists and if needed create temporary buffer with or without it
    if(new_repo[strlen(new_repo) - 1] == '/') {
        buffer = calloc(strlen(new_repo) - 1, sizeof(char));
        strncpy(buffer, new_repo, strlen(new_repo) - 1);
    }

    else {
        buffer = calloc(strlen(new_repo) + 1, sizeof(char));
        strncpy(buffer, new_repo, strlen(new_repo));
        buffer[strlen(new_repo)] = '/';
    }

    // Confirm that new repository path does not exist
    for(index = 0; index < (*p_repo_count); index++)
        if(!strcmp(buffer, pp_repo_path[index]) || !strcmp(new_repo, pp_repo_path[index]))
            return -1;
    
    free(buffer);
    file = fopen(repo_conf_path, "w");

    // Push new repo to repo paths 
    (*p_repo_count)++;
    pp_repo_path = (char**) realloc(pp_repo_path, (*p_repo_count) * sizeof(char*));
    cm_CheckMemoryAlloc((void*) pp_repo_path);

    pp_repo_path[(*p_repo_count) - 1] = (char*) calloc(strlen(new_repo), sizeof(char));
    strncpy(pp_repo_path[(*p_repo_count) - 1], new_repo, strlen(new_repo));

    // Add repository paths to config file
    for(index = 0; index < (*p_repo_count); index++) {
        fwrite(pp_repo_path[index], sizeof(char), strlen(pp_repo_path[index]), file);
        if(index == (default_id + 1)) fwrite(" default", 1, 8, file);
        if(index != (*p_repo_count) - 1) fwrite("\n", 1, 1, file);
    }

    return 0;
}


/* Set default local repository for assembling assets */
void damSetDefaultRepo(char **pp_repo_paths, size_t repo_count, int id) {
    size_t index;
    FILE *file;

    if(id > repo_count) {
        printf("ID is out of available repositories bounds!\n");
        printf("Update local_repos.conf or select different ID!\n");
        return;
    }

    file = fopen(repo_conf_path, "w");

    for(index = 0; index < repo_count; index++) {
        fwrite(pp_repo_paths[index], sizeof(char), strlen(pp_repo_paths[index]), file);
        if(index == (id - 1)) fwrite(" default", 1, 8, file);      
        if(index != repo_count - 1) fwrite("\n", 1, 1, file);
    }

    fclose(file);

    printf("Successfully updated default repository ID!\n");
}


/* List all local repos */
void damListLocalRepos(char **pp_repo_paths, size_t repo_count, int default_id) {
    size_t index;
    for(index = 0; index < repo_count; index++) {
        if(default_id - 1 == index) printf("[%ld] %s [DEFAULT]\n", index + 1, pp_repo_paths[index]);
        else printf("[%ld] %s\n", index + 1, pp_repo_paths[index]);
    }

    if(!repo_count) {
        printf("No local repositories are configured!\n");
        printf("Add new repository paths with --add-repo usage flag\n");
        printf("For more information use dam --help!\n");
    }
}


/* Assemble name and description to asset file */
void damAssetAssemblyCaller(DENGAsset *p_asset, char **pp_repo_paths, int repo_id) {
    size_t index;
    char *total_file_name;
    char file_name_buffer[24];
    char name_buffer[24];
    char desc_buffer[256];

    // Initialise buffers
    for(index = 0; index < 24; index++) name_buffer[index] = 0x00, file_name_buffer[index] = 0x00;
    for(index = 0; index < 256; index++) desc_buffer[index] = 0x00;
    
    // Get user input for name and description
    printf("Enter the file name (without .das): ");
    fgets(file_name_buffer, 24, stdin);
    printf("Enter descriptive file name: ");
    fgets(name_buffer, 24, stdin);
    printf("Add description to the asset: \n");
    fgets(desc_buffer, 256, stdin);
    
    // Remove all newlines
    for(index = 0; index < 24; index++) 
        if(name_buffer[index] == 0x0A) name_buffer[index] = 0x00;

    for(index = 0; index < 256; index++)
        if(desc_buffer[index] == 0x0A) desc_buffer[index] = 0x00;
    
    // Allocate memory for total name string
    size_t total_name_size = strlen(file_name_buffer) + strlen(pp_repo_paths[repo_id - 1]) - 1;
    total_file_name = (char*) calloc(total_name_size, sizeof(char));
    strncpy(total_file_name, pp_repo_paths[repo_id - 1], total_name_size);

    // Copy to file name to total_file_name
    for(index = strlen(pp_repo_paths[repo_id - 1]); index < total_name_size; index++)
        total_file_name[index] = file_name_buffer[(index - strlen(pp_repo_paths[repo_id - 1]))];

    p_asset->name = name_buffer;
    p_asset->description = desc_buffer;

    dasAssemble(p_asset, total_file_name);

    free(total_file_name);
}


int main(int argc, char *argv[]) {
    int write_repo_id = 0;
    size_t index;
    
    // Read local repository contents
    char **pp_repo_paths;
    size_t repo_count = 0;
    damReadLocalRepoConf(&pp_repo_paths, &repo_count, &write_repo_id);
    
    // Main command line flags handling
    for(index = 1; index < (size_t) argc; index++) {
        /* Currently only local repositories can be used */
        /* Support for global repositories is a project for later */
        if(!strcmp(argv[index], "--local")) {
            index++;
            // No arguments after --local flag
            if(index >= (size_t) argc) {
                printf("Please specify the action you would like do under local repositories!\n");
                printf("For more information use dam --help\n");
                return 0;
            }

            // List all assets
            if(!strcmp(argv[index], "-l") || !strcmp(argv[index], "--list")) {
                damListAssets(pp_repo_paths, repo_count);
                return 0;
            }

            // Add new repository path
            if(!strcmp(argv[index], "--add-repo")) {
                index++;
                DIR *test_dir;

                if(index >= (size_t) argc || !(test_dir = opendir(argv[index]))) {
                    printf("Please specify correct file path!\n");
                    printf("For more information use dam --help!\n");
                    return 0;
                }

                closedir(test_dir);
                if(damAddRepo(pp_repo_paths, &repo_count, argv[index], write_repo_id)) {
                    printf("Failed to add repo!\n");
                    printf("Repository with name %s already exists!\n", argv[index]);
                }
            }

            // Set write repository
            if(!strcmp(argv[index], "--set-default-repo-id")) {
                index++;
                if(index >= (size_t) argc) {
                    printf("Please specify the local repository path!\n");
                    printf("Try dam --local --set-write-repo-dir [DIR_ID]\n");
                    printf("For more information use dam --help!\n");
                    return 0;
                }

                else write_repo_id = atoi(argv[index]);

                damSetDefaultRepo(pp_repo_paths, repo_count, write_repo_id);
                return 0;
            }

            // List all available repositories
            if(!strcmp(argv[index], "--list-repos")) {
                damListLocalRepos(pp_repo_paths, repo_count, write_repo_id);
                return 0;
            }
            
            // Assemble asset
            if(!strcmp(argv[index], "--assemble")) {
                int is_obj_specified = false;
                char *obj_path;
                DENGAsset asset;
                index++;

                // Check is asset object file is specified
                if((!strcmp(argv[index], "-o") || !strcmp(argv[index], "--obj-path")) && (index++) < (size_t) argc) {
                    obj_path = argv[index];
                    is_obj_specified = true;
                }

                // All flags are specified
                if(is_obj_specified && write_repo_id) {
                    printf("Loading asset data...\n");
                    dasLoadModel(&asset, obj_path);
                    damAssetAssemblyCaller(&asset, pp_repo_paths, write_repo_id);
                    printf("Done!\n");
                }

                // Object file is not specified
                else if(!is_obj_specified) {
                    printf("Please specify the source model file!\n");
                    printf("For more information use dam --help!\n");
                }

                // No default write repo specified
                else if(!write_repo_id) {
                    printf("No default repository set!\n");
                    printf("Set default repository with dam --local --set-default-repo-id [ID]\n");
                    printf("For more information use dam --help!\n");
                }
            }
        }

        // Help
        else if(!strcmp(argv[index], "--help")) {
            printf("%s\n", help_text);
            return 0;
        }

        // Unknown usage
        else {
            printf("Unknown option %s!\n", argv[index]);
            printf("For more information use dam --help!\n");
            return 0;
        }
    }

    if(index == 1) {
        printf("Please specify the type of repository you would like to use!\n");
        printf("For more information use dam --help\n");
        return 0;
    }

    return 0;
}