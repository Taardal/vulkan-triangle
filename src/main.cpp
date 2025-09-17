#include "app.h"

using namespace dd;

int main() {
    app app({
        .name = "2D game",
    });
    run(app);
}
