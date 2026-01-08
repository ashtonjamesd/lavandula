#include "../include/lavandula.h"

appRoute(home, ctx) {
    if (!ctx.app) exit(1);

    char *html = readFile("home.html");
    return html ? ok(html, TEXT_HTML) : notFound("Content not found...", TEXT_HTML);
}

int main() {
    App app = createApp();
    root(&app, home);

    runApp(&app);
}