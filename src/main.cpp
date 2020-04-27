#include "Setup.h"

#undef RTTI_SUPPORTED
#include "App.h"

App *app;

void setup() {
    app = new App();
}

void loop() {
    app->run();
}
