#define __DAM_C
#define __DAM_USAGE
#include <das/dam.h>


/* Count years recursively */
deng_ui16_t damCountYears (
    deng_i64_t *p_time, 
    deng_ui16_t years, 
    int n, 
    int *p_is_leap_year
) {
    if(n < 3 && (*p_time) - SECONDS_PER_YEAR >= 0) {
        n++;
        years++;
        (*p_time) -= SECONDS_PER_YEAR;
        *p_is_leap_year = false;
        return damCountYears (
            p_time, 
            years, 
            n, 
            p_is_leap_year
        );
    }
    
    else if((*p_time) - SECONDS_PER_LEAP_YEAR >= 0) {
        years++;
        (*p_time) -= SECONDS_PER_LEAP_YEAR;
        n = 0;
        *p_is_leap_year = true;
        return damCountYears (
            p_time, 
            years, 
            n, 
            p_is_leap_year
        );
    }

    return years;
}


/* Count months recursively */
deng_ui16_t damCountMonths (
    deng_i64_t *p_time, 
    deng_ui16_t months, 
    int is_leap_year
) {
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
void damFormatDate(char *date, char *time, deng_i64_t time_from_epoch) {
    int is_leap_year = false;
    char *ch_month, *ch_day;
    ch_month = (char*) calloc(7, sizeof(char));
    ch_day = (char*) calloc(7, sizeof(char));
    
    char *ch_hour, *ch_minute, *ch_second;
    ch_hour = (char*) calloc(7, sizeof(char));
    ch_minute = (char*) calloc(7, sizeof(char));
    ch_second = (char*) calloc(7, sizeof(char));

    // Count years since epoch
    time_from_epoch -= 2 * SECONDS_PER_YEAR;
    deng_ui16_t year_count = 1972 + damCountYears(&time_from_epoch, 0, 0, &is_leap_year);

    // Count months from remaining time
    deng_ui16_t month = damCountMonths(&time_from_epoch, 1, is_leap_year);
    if(month < 10) sprintf(ch_month, "0%d", month);
    else sprintf(ch_month, "%d", month);

    // Count day
    deng_ui16_t day = (deng_ui16_t) (time_from_epoch / 86400);
    time_from_epoch %= 86400;
    if(day < 10) sprintf(ch_day, "0%d", day);
    else sprintf(ch_day, "%d", day);
    
    sprintf(date, "%d/%s/%s", year_count, ch_month, ch_day);
    free(ch_month);
    free(ch_day);

    // Count hours
    deng_ui16_t hour = (deng_ui16_t) (time_from_epoch / 3600);
    time_from_epoch %= 3600;
    if(hour < 10) sprintf(ch_hour, "0%d", hour); 
    else sprintf(ch_hour, "%d", hour);

    // Count minutes
    deng_ui16_t minute = (deng_ui16_t) (time_from_epoch / 60);
    time_from_epoch %= 60;
    if(minute < 10) sprintf(ch_minute, "0%d", minute);
    else sprintf(ch_minute, "%d", minute);

    if(time_from_epoch < 10) sprintf(ch_second, "0%d", (deng_ui16_t) time_from_epoch);
    else sprintf(ch_second, "%d", (deng_ui16_t) time_from_epoch);
    
    sprintf(time, "%s:%s:%s", ch_hour, ch_minute, ch_second);
    free(ch_hour);
    free(ch_minute);
    free(ch_second);
}


/* List all available assets in local repositories */
void damListAsset (char *asset_file) {
    FILE *file;
    file = fopen(asset_file, "rb");
    
    if(!file) {
        printf("Failed to read file '%s'\n", asset_file);
        exit(EXIT_FAILURE);
    }

    deng_ui64_t timestamp;
    deng_AssetMode asset_mode;
    char *name;

    dasReadINFOHDR (
        &name,
        &timestamp,
        &asset_mode,
        asset_file,
        file
    );

    char date[24];
    char time[24];

    memset(date, 0, 24);
    memset(time, 0, 24);

    printf (
        "Name: %s\n", 
        name
    );

    damFormatDate (
        date, 
        time, 
        timestamp
    );

    printf (
        "Date and time of creation: %s %s(UTC)\n", 
        date,
        time
    );

    switch(asset_mode)
    {
    case DENG_ASSET_MODE_3D_UNMAPPED:
        printf("Vertex type: vertices only\n");
        break;

    case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
        printf("Vertex type: normalized vertices\n");
        break;

    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
        printf("Vertex type: textured vertices\n");
        break;
    
    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
        printf("Vertex type: normalized textured vertices\n");
        break;

    default:
        break;
    }
}


/* Assemble name and description to asset file */
void damAssetAssemblyCaller (
    char *asset_name,
    char *file_path,
    char *out_file,
    char *asset_use_str
) {
    char *ext = cm_GetFileExtName(out_file);
    
    // Check if file extension needs to be added
    if(!ext || (ext && strcmp(ext, "das"))) {
        char *tmp = (char*) calloc (
            strlen(out_file) + 5,
            sizeof(char)
        );

        sprintf (
            tmp,
            "%s.das",
            out_file
        );

        free(out_file);
        out_file = tmp;
    }
    
    deng_Asset asset;
    asset.id = asset_name;
    if(!strcmp(asset_use_str, "v"))
        asset.asset_mode = DENG_ASSET_MODE_3D_UNMAPPED;
    else if(!strcmp(asset_use_str, "vn"))
        asset.asset_mode = DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED;
    else if(!strcmp(asset_use_str, "vt"))
        asset.asset_mode = DENG_ASSET_MODE_3D_TEXTURE_MAPPED;
    else if(!strcmp(asset_use_str, "vtn"))
        asset.asset_mode = DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED;

    dasLoadModel (
        &asset,
        file_path
    );
    
    dasAssemble (
        &asset, 
        out_file
    );
    
    // Cleanup
    switch(asset.asset_mode) 
    {
    case DENG_ASSET_MODE_3D_UNMAPPED:
        free(asset.vertices.p_unmapped_unnormalized_vert);
        break;

    case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
        free(asset.vertices.p_unmapped_normalized_vert);
        break;

    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
        free(asset.vertices.p_tex_mapped_unnormalized_vert);
        break;

    case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
        free(asset.vertices.p_tex_mapped_normalized_vert);
        break;

    default:
        break;
    }

    free(asset.indices.p_indices);
}


int main(int argc, char *argv[]) {
    size_t index;
    
    // Main command line flags handling
    for(index = 1; index < (size_t) argc; index++) {
        // List information about the asset
        if(!strcmp(argv[index], "-i") || !strcmp(argv[index], "--info")) {
            // Check if asset file is specified
            if(index + 1 < 1) {
                printf("No asset file specified\n");
                TO_HELP;
                return 0;
            }

            index++;
            damListAsset(argv[index]);
            return 0;
        }
        
        else if(!strcmp(argv[index], "-m")) {
            // Find specified asset name
            char *name = NULL;
            char *file = NULL;
            char *type = NULL;
            char *out_file = NULL;

            if(index + 1 >= (size_t) argc) {
                printf("No asset name specified\n");
                TO_HELP;
                exit(EXIT_FAILURE);
            }

            else {
                out_file = argv[index + 1];
                index++;
            }

            for(; index < (size_t) argc; index++) {
                if
                (
                    !strcmp(argv[index], "-n") || 
                    !strcmp(argv[index], "--name")
                ) {
                    // No name specified error
                    if(index + 1 >= (size_t) argc) {
                        printf("Name flag requires name as a parameter\n");
                        TO_HELP;
                        exit(EXIT_FAILURE);
                    }
                    
                    index++;
                    name = argv[index];
                }
                
                else if
                (
                    !strcmp(argv[index], "-f") ||
                    !strcmp(argv[index], "--file")
                ) {
                    // No file specified error
                    if(index + 1 >= (size_t) argc) {
                        printf("File flag requires file name as a parameter\n");
                        TO_HELP;
                        exit(EXIT_FAILURE);
                    }

                    index++;
                    file = argv[index];
                }

                else if
                (
                    !strcmp(argv[index], "-t") |\
                    !strcmp(argv[index], "--type")
                ) {
                    // No type specified
                    if(index + 1 >= (size_t) argc) {
                        printf("Asset type requires parameter\n");
                        TO_HELP;
                        exit(EXIT_FAILURE);
                    }

                    index++;
                    type = argv[index];
                }

                if(name && file && type && out_file) 
                    break;
            }

            // Asset file name not specified error
            if(!file) {
                printf("No model file specified for asset creation\n");
                TO_HELP;
                exit(EXIT_FAILURE);
            }

            if(!name) {
                printf("No asset name specified for asset creation\n");
                TO_HELP;
                exit(EXIT_FAILURE);
            }

            if(!type) {
                printf("No asset type specified for asset creation\n");
                TO_HELP;
                exit(EXIT_FAILURE);
            }

            if(!out_file) 
                out_file = "asset.das";
            
            printf (
                "Making asset '%s'\n",
                name
            );

            damAssetAssemblyCaller (
                name,
                file,
                out_file,
                type
            );

            printf("Done\n");
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
        printf("DENG asset manager %s\n", DAM_VERSION);
        printf("For more information use --help\n");
        return 0;
    }

    return 0;
}
