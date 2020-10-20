#ifndef CSS_DEFINITIONS_H
#define CSS_DEFINITIONS_H

#define DENG_CSS_CLASS_HEAD "head"
#define DENG_CSS_CLASS_BODY "body"

#define DENG_CSS_SUPPORTED_PROPERTIES_COUNT 21

enum dengCSSIndexDefinitions {
    DENG_CSS_PROPERTY_BACKGROUND_COLOR = 0,
    DENG_CSS_PROPERTY_BORDER = 1,
    DENG_CSS_PROPERTY_BORDER_COLOR = 2,
    DENG_CSS_PROPERTY_BORDER_WIDTH = 3,
    DENG_CSS_PROPERTY_BORDER_BOTTOM_COLOR = 4,
    DENG_CSS_PROPERTY_BORDER_BOTTOM_WIDTH = 5,
    DENG_CSS_PROPERTY_BORDER_LEFT_COLOR = 6,
    DENG_CSS_PROPERTY_BORDER_LEFT_WIDTH = 7,
    DENG_CSS_PROPERTY_BORDER_RIGHT_COLOR = 8,
    DENG_CSS_PROPERTY_BORDER_RIGHT_WIDTH = 9,
    DENG_CSS_PROPERTY_BORDER_TOP_COLOR = 10,
    DENG_CSS_PROPERTY_BORDER_TOP_WIDTH = 11,
    DENG_CSS_PROPERTY_FONT_SIZE = 12,
    DENG_CSS_PROPERTY_FONT_WEIGHT = 13,
    DENG_CSS_PROPERTY_HEIGHT = 14,
    DENG_CSS_PROPERTY_MARGIN = 15,
    DENG_CSS_PROPERTY_MARGIN_TOP = 16,
    DENG_CSS_PROPERTY_MARGIN_BOTTOM = 17, 
    DENG_CSS_PROPERTY_MARGIN_RIGHT = 18,
    DENG_CSS_PROPERTY_MARGIN_LEFT = 19,
    DENG_CSS_PROPERTY_WIDTH = 20
};

enum dengCSSClassReadMode {
    DENG_CSS_CLASS_READ_MODE_MAIN_CLASS = 0,
    DENG_CSS_CLASS_READ_MODE_CUSTOM_CLASS = 1
};

enum dengCSSFontWeight {
    DENG_CSS_FONT_WEIGHT_NORMAL = 0,
    DENG_CSS_FONT_WEIGHT_BOLD = 1
};

namespace dengCSS {

    struct ColorDefinitions {
        std::unordered_map<std::string, dengMath::vec3<float>> deng_css_colors;
        
        ColorDefinitions() {
            this->deng_css_colors["aliceblue"] = {0.94118f, 0.97255f, 1.0f};
            this->deng_css_colors["antiquewhite"] = {0.98039f, 0.92159f, 0.84313f};
            this->deng_css_colors["aqua"] = {0.0f, 1.0f, 1.0f};
            this->deng_css_colors["aquamarine"] = {0.49804f, 1.0f, 0.83137f};
            this->deng_css_colors["azure"] = {0.94118f, 1.0f, 1.0f};
            this->deng_css_colors["beige"] = {0.96078f, 0.96078f, 0.86275f};
            this->deng_css_colors["bique"] = {1.0f, 0.89412f, 0.76863f};
            this->deng_css_colors["black"] = {0.0f, 0.0f, 0.0f};
            this->deng_css_colors["blanchedalmond"] = {1.0f, 0.92157f, 0.80392f};
            this->deng_css_colors["blue"] = {0.0f, 0.0f, 1.0f};
            this->deng_css_colors["blueviolet"] = {0.54118f, 0.16863f, 0.88627f};
            this->deng_css_colors["brown"] = {0.64706f, 0.16471f, 0.16471f};
            this->deng_css_colors["burlywood"] = {0.87059f, 0.72157f, 0.52941f};
            this->deng_css_colors["cadetblue"] = {0.37255f, 0.61961f, 0.62745f};
            this->deng_css_colors["chartreuse"] = {0.49804f, 1.0f, 0.0f};
            this->deng_css_colors["chocolate"] = {0.82353f, 0.41176f, 0.11765f};
            this->deng_css_colors["coral"] = {1.0f, 0.49804f, 0.31323f};
            this->deng_css_colors["cornflowerblue"] = {0.39216f, 0.58431f, 0.92941f};
            this->deng_css_colors["cornsilk"] = {1.0f, 0.97255f, 0.86275f};
            this->deng_css_colors["crimson"] = {0.86275f, 0.07843f, 0.23529f};
            this->deng_css_colors["cyan"] = {0.0f, 1.0f, 1.0f};
            this->deng_css_colors["darkblue"] = {0.0f, 0.0f, 0.5451f};
            this->deng_css_colors["darkcyan"] = {0.0f, 0.5451f, 0.5451f};
            this->deng_css_colors["darkgoldenrod"] = {0.72157f, 0.52549f, 0.04314f};
            this->deng_css_colors["darkgray"] = {0.66275f, 0.66275f, 0.66275f};
            this->deng_css_colors["darkkhaki"] = {0.74118f, 0.71765f, 0.41961f};
            this->deng_css_colors["darkmagneta"] = {0.5451f, 0.0f, 0.5451f};
            this->deng_css_colors["darkolivegreen"] = {0.33333f, 0.41961f, 0.18431f};
            this->deng_css_colors["darkorange"] = {1.0f, 0.54902f, 0.0f};
            this->deng_css_colors["darkorchid"] = {0.6f, 0.19607f, 0.8f};
            this->deng_css_colors["darkred"] = {0.5451f, 0.0f, 0.0f};
            this->deng_css_colors["darksalmon"] = {0.91373f, 0.58824f, 0.47843f};
            this->deng_css_colors["darkseagreen"] = {0.56078f, 0.73725f, 0.56078f};
            this->deng_css_colors["darkslateblue"] = {0.28235f, 0.23922f, 0.5451f};
            this->deng_css_colors["darkslategrey"] = {0.18431f, 0.3098f, 0.3098f};
            this->deng_css_colors["darkslategray"] = {0.18431f, 0.3098f, 0.3098f};
            this->deng_css_colors["darkturquoise"] = {0.0f, 0.80784f, 0.81961f};
            this->deng_css_colors["darkviolet"] = {0.58039f, 0.0f, 0.82745f};
            this->deng_css_colors["deeppink"] = {1.0f, 0.07843f, 0.57647f};
            this->deng_css_colors["deepskyblue"] = {0.0f, 0.74901f, 1.0f};
            this->deng_css_colors["dimgray"] = {0.41176f, 0.41176f, 0.41176f};
            this->deng_css_colors["dodgerblue"] = {0.11765f, 0.56471f, 1.0f};
            this->deng_css_colors["firebrick"] = {0.69804f, 0.13333f, 0.13333f};
            this->deng_css_colors["floralwhite"] = {1.0f, 0.98039f, 0.94118f};
            this->deng_css_colors["forestgreen"] = {0.13333f, 0.5451f, 0.13333f};
            this->deng_css_colors["fuchsia"] = {1.0f, 0.0f, 1.0f};
            this->deng_css_colors["gainsboro"] = {0.86275f, 0.86275f, 0.86275f};
            this->deng_css_colors["ghostwhite"] = {0.97255f, 0.97255f, 1.0f};
            this->deng_css_colors["gold"] = {1.0f, 0.84314f, 0.0f};
            this->deng_css_colors["goldenrod"] = {0.8549f, 0.64706f, 0.12549f};
            this->deng_css_colors["gray"] = {0.50196f, 0.50196f, 0.50196f};
            this->deng_css_colors["green"] = {0.0f, 0.50196f, 0.0f};
            this->deng_css_colors["greenyellow"] = {0.67843f, 1.0f, 0.18431f};
            this->deng_css_colors["grey"] = {0.50196f, 0.50196f, 0.50196f};
            this->deng_css_colors["honeydew"] = {0.94118f, 1.0f, 0.94118f};
            this->deng_css_colors["hotpink"] = {1.0f, 0.41176f, 0.70588f};
            this->deng_css_colors["indianred"] = {0.80392f, 0.36078f, 0.36078f};
            this->deng_css_colors["indigo"] = {0.29412f, 0.0f, 0.5098f};
            this->deng_css_colors["ivory"] = {1.0f, 1.0f, 0.94118f};
            this->deng_css_colors["khaki"] = {0.94118f, 0.90196f, 0.54902f};
            this->deng_css_colors["lavendel"] = {0.90196f, 0.90196f, 0.98039f};
            this->deng_css_colors["lavendelblush"] = {1.0f, 0.94118f, 0.96078f};
            this->deng_css_colors["lawngreen"] = {0.48627f, 0.98924f, 0.0f};
            this->deng_css_colors["lemonchiffon"] = {1.0f, 0.98039f, 0.80392f};
            this->deng_css_colors["lightblue"] = {0.67843f, 0.84706f, 0.90196f};
            this->deng_css_colors["lightcoral"] = {0.94118f, 0.50196f, 0.50196f};
            this->deng_css_colors["lightcyan"] = {0.87843f, 1.0f, 1.0f};
            this->deng_css_colors["lightgolderrodyellow"] = {0.98039f, 0.98039f, 0.82353f};
            this->deng_css_colors["lightgray"] = {0.82745f, 0.82745f, 0.82745f};
            this->deng_css_colors["lightgreen"] = {0.56471f, 0.93333f, 0.56471f};
            this->deng_css_colors["lightpink"] = {1.0f, 0.71373f, 0.75686f};
            this->deng_css_colors["lightsalmon"] = {1.0f, 0.62745f, 0.47843f};
            this->deng_css_colors["lightseagreen"] = {0.12549f, 0.69804f, 0.66667f};
            this->deng_css_colors["lightskyblue"] = {0.52941f, 0.80784f, 0.98039f};
            this->deng_css_colors["lightslategray"] = {0.46667f, 0.53333f, 0.6f};
            this->deng_css_colors["lightslategrey"] = {0.46667f, 0.53333f, 0.6f};
            this->deng_css_colors["lightsteelblue"] = {0.6902f, 0.76863f, 0.87059f};
            this->deng_css_colors["lightyellow"] = {1.0f, 1.0f, 0.87843f};
            this->deng_css_colors["lime"] = {0.0f, 1.0f, 0.0f};
            this->deng_css_colors["limegreen"] = {0.19608f, 0.80392f, 0.19608f};
            this->deng_css_colors["linen"] = {0.98039f, 0.94118f, 0.90196f};
            this->deng_css_colors["magneta"] = {1.0f, 0.0f, 1.0f};
            this->deng_css_colors["maroon"] = {0.50196f, 0.0f, 0.0f};
            this->deng_css_colors["mediumaquamarine"] = {0.4f, 0.80392f, 0.66667f};
            this->deng_css_colors["mediumblue"] = {0.0f, 0.0f, 0.80392f};
            this->deng_css_colors["mediumorchid"] = {0.72941f, 0.33333f, 0.82745f};
            this->deng_css_colors["mediumpurple"] = {0.57647f, 0.43922f, 0.85882f};
            this->deng_css_colors["mediumseagreen"] = {0.23529f, 0.70196f, 0.44314f};
            this->deng_css_colors["mediumslateblue"] = {0.48235f, 0.40784f, 0.93333f};
            this->deng_css_colors["mediumspringgreen"] = {0.0f, 0.98039f, 0.60392f};
            this->deng_css_colors["mediumturquoise"] = {0.28235f, 0.81961f, 0.8f};
            this->deng_css_colors["mediumvioletred"] = {0.78039f, 0.08235f, 0.52157f};
            this->deng_css_colors["midnightblue"] = {0.09804f, 0.09804f, 0.43922f};
            this->deng_css_colors["mintcream"] = {0.96078f, 1.0f, 0.98039f};
            this->deng_css_colors["mistyrose"] = {1.0f, 0.89412f, 0.88235f};
            this->deng_css_colors["moccasin"] = {1.0f, 0.89412f, 0.7098f};
            this->deng_css_colors["navajowhite"] = {1.0f, 0.87059f, 0.67843f};
            this->deng_css_colors["navy"] = {0.0f, 0.0f, 0.50196f};
            this->deng_css_colors["oldlace"] = {0.99216f, 0.96078f, 0.90196f};
            this->deng_css_colors["olive"] = {0.50196f, 0.50196f, 0.0f};
            this->deng_css_colors["olivedrab"] = {0.41961f, 0.55686f, 0.13725f};
            this->deng_css_colors["orange"] = {1.0f, 0.64706f, 0.0f};
            this->deng_css_colors["orangered"] = {1.0f, 0.27059f, 0.0f};
            this->deng_css_colors["orchid"] = {0.8549f, 0.43922f, 0.83922f};
            this->deng_css_colors["palegoldenrod"] = {0.93333f, 0.9098f, 0.66667f};
            this->deng_css_colors["palegreen"] = {0.59608f, 0.98431f, 0.59608f};
            this->deng_css_colors["paleturquoise"] = {0.68627f, 0.93333f, 0.93333f};
            this->deng_css_colors["palevioletred"] = {0.85882f, 0.43922f, 0.57647f};
            this->deng_css_colors["papayawhip"] = {1.0f, 0.93725f, 0.83529f};
            this->deng_css_colors["peachpuff"] = {1.0f, 0.8549f, 0.72549f};
            this->deng_css_colors["peru"] = {0.80392f, 0.52157f, 0.24706f};
            this->deng_css_colors["pink"] = {1.0f, 0.75294f, 0.79608f};
            this->deng_css_colors["plum"] = {0.86667f, 0.62745f, 0.86667f};
            this->deng_css_colors["powderblue"] = {0.6902f, 0.87843f, 0.90196f};
            this->deng_css_colors["purple"] = {0.50196f, 0.0f, 0.50196f};
            this->deng_css_colors["rebeccapurple"] = {0.4f, 0.2f, 0.6f};
            this->deng_css_colors["red"] = {1.0f, 0.0f, 0.0f};
            this->deng_css_colors["rosybrown"] = {0.73725f, 0.56078f, 0.56078f};
            this->deng_css_colors["royalblue"] = {0.2549f, 0.41176f, 0.88235f};
            this->deng_css_colors["saddlebrown"] = {0.5451f, 0.27059f, 0.0745f};
            this->deng_css_colors["salmon"] = {0.98039f, 0.50196f, 0.44706f};
            this->deng_css_colors["sandybrown"] = {0.95686f, 0.64314f, 0.37647f};
            this->deng_css_colors["seagreen"] = {0.18039f, 0.5451f, 0.34118f};
            this->deng_css_colors["seashell"] = {1.0f, 0.96078f, 0.93333f};
            this->deng_css_colors["sienna"] = {0.62745f, 0.32157f, 0.17647f};
            this->deng_css_colors["silver"] = {0.75294f, 0.75294f, 0.75294f};
            this->deng_css_colors["skyblue"] = {0.52941f, 0.80784f, 0.92157f};
            this->deng_css_colors["slateblue"] = {0.41569f, 0.35294f, 0.80392f};
            this->deng_css_colors["slategray"] = {0.43922f, 0.50196f, 0.56471f};
            this->deng_css_colors["slategrey"] = {0.43922f, 0.50196f, 0.56471f};
            this->deng_css_colors["snow"] = {1.0f, 0.98039f, 0.98039f};
            this->deng_css_colors["springgreen"] = {0.0f, 1.0f, 0.49804f};
            this->deng_css_colors["steelblue"] = {0.27451f, 0.5098f, 0.70588f};
            this->deng_css_colors["tan"] = {0.82353f, 0.70588f, 0.54902f};
            this->deng_css_colors["teal"] = {0.0f, 0.50196f, 0.50196f};
            this->deng_css_colors["thistle"] = {0.84706f, 0.74902f, 0.84706f};
            this->deng_css_colors["tomato"] = {1.0f, 0.38824f, 0.27843f};
            this->deng_css_colors["turquoise"] = {0.25098f, 0.87843f, 0.81569f};
            this->deng_css_colors["violet"] = {0.93333f, 0.5098f, 0.93333f};
            this->deng_css_colors["wheat"] = {0.96078f, 0.87059f, 0.70196f};
            this->deng_css_colors["white"] = {1.0f, 1.0f, 1.0f};
            this->deng_css_colors["whitesmoke"] = {0.96078f, 0.96078f, 0.96078f};
            this->deng_css_colors["yellow"] = {1.0f, 1.0f, 0.0f};
            this->deng_css_colors["yellowgreen"] = {0.60392f, 0.80392f, 0.19608f};
        }
    };

    struct PropertiesDefinitions {
        std::vector<std::string> properties_names;

        PropertiesDefinitions() {
            this->properties_names.resize(DENG_CSS_SUPPORTED_PROPERTIES_COUNT);
            this->properties_names[0] = "background-color";
            this->properties_names[1] = "border";
            this->properties_names[2] = "border-color";
            this->properties_names[3] = "border-width";
            this->properties_names[4] = "border-bottom-color";
            this->properties_names[5] = "border-bottom-width";
            this->properties_names[6] = "border-left-color";
            this->properties_names[7] = "border-left-width";
            this->properties_names[8] = "border-right-color";
            this->properties_names[9] = "border-right-width";
            this->properties_names[10] = "border-top-color";
            this->properties_names[11] = "border-top-width";
            this->properties_names[12] = "font-size";
            this->properties_names[13] = "font-weight";
            this->properties_names[14] = "height";
            this->properties_names[15] = "margin";
            this->properties_names[16] = "margin-top";
            this->properties_names[17] = "margin-bottom";
            this->properties_names[18] = "margin-right";
            this->properties_names[19] = "margin-left";
            this->properties_names[20] = "width";
        }
    };
}

#endif