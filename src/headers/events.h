#include "window.h"

namespace Deng {
    class Events
    {
        private:
            Window *window;

        public:
            void update();
            void handleExit();

            Events(Window *win);
    };
}