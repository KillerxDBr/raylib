#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <uchar.h>
#include <wctype.h>

#include "raylib.h"

#define V2_FMT "(%.2f, %.2f)"
#define V2_ARG(v2) (v2).x, (v2).y
int main(int argc, char **argv)
{
    setlocale(LC_CTYPE, "");

    char file_name[32] = {0};
    {
        char *p;
        char *p1 = strrchr(argv[0], '\\');
        char *p2 = strrchr(argv[0], '/');

        printf("p1: %s\n", p1);
        printf("p2: %s\n", p2);

        p = max(p1, p2);
        assert(p);
        p = _strdup(p + 1);
        p1 = strchr(p, '.');
        if (p1)
            p1[0] = '\0';

        strcpy(file_name, p);
        strcat(file_name, ".log");

        free(p);
    }

    FILE *f = fopen(file_name, "wb");
    assert(f);

    InitWindow(640, 480, "raylib example - basic window");
    Vector2 pos = GetWindowPosition();

    fprintf(f, "GetWindowPosition: " V2_FMT "\n", V2_ARG(pos));

    int m = GetCurrentMonitor();
    fprintf(f, "GetScreenWidth: %d\n", GetScreenWidth());
    fprintf(f, "GetScreenHeight: %d\n", GetScreenHeight());
    fprintf(f, "GetRenderWidth: %d\n", GetRenderWidth());
    fprintf(f, "GetRenderHeight: %d\n", GetRenderHeight());
    int monitorCount = GetMonitorCount();
    fprintf(f, "GetMonitorCount: %d\n", monitorCount);
    for (int m = 0; m < monitorCount; ++m)
    {
        fprintf(f, "Monitor %d ===================================================\n", m + 1);
        pos = GetMonitorPosition(m);
        fprintf(f, "  GetMonitorPosition: " V2_FMT "\n", V2_ARG(pos));
        fprintf(f, "  GetMonitorWidth:  %d px\n", GetMonitorWidth(m));
        fprintf(f, "  GetMonitorHeight: %d px\n", GetMonitorHeight(m));
        int phw = GetMonitorPhysicalWidth(m);
        int phh = GetMonitorPhysicalHeight(m);
        fprintf(f, "  GetMonitorPhysicalWidth:  %d mm\n", phw);
        fprintf(f, "  GetMonitorPhysicalHeight: %d mm\n", phh);
        float h = hypotf((float)phw / 25.4f, (float)phh / 25.4f);
        fprintf(f, "  Monitor Diagonal: %.2f in\n", h);
        fprintf(f, "  GetMonitorRefreshRate: %d hz\n", GetMonitorRefreshRate(m));
        fprintf(f, "  GetMonitorName: \"%s\"\n", GetMonitorName(m));
    }
    fprintf(f, "=============================================================\n");

    fprintf(f, "Clipboard data: \"%s\"\n", GetClipboardText());
    const char *cbdata = "Raylib Clipboard Test";
    fprintf(f, "Setting Clipboard Text to \"%s\"\n", cbdata);
    SetClipboardText(cbdata);
    const char *cb = GetClipboardText();
    fprintf(f, "Clipboard data: \"%s\"\n", cb);

    fprintf(f, "\"%s\" == \"%s\": %s\n", cb, cbdata, strcmp(cbdata, cb) == 0 ? "True" : "False");

    size_t mon = 0;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_Q))
        {
            TraceLog(LOG_INFO, "SetWindowFocused");
            SetWindowFocused();
        }

        if (IsKeyPressed(KEY_F))
        {
            TraceLog(LOG_INFO, "ToggleFullscreen");
            ToggleFullscreen();
        }
        if (IsKeyPressed(KEY_S))
        {
            TraceLog(LOG_INFO, "SetWindowMonitor");
            ++mon;
            SetWindowMonitor(mon % monitorCount);
        }

        if (IsKeyPressed(KEY_E))
        {
            TraceLog(LOG_INFO, "SetWindowOpacity");
            static size_t op = 0;
            SetWindowOpacity(op % 2 ? .5 : 1);
            ++op;
        }

        int k;
        while ((k = GetKeyPressed()) != 0)
        {
            const char *keyName = GetKeyName(k);
            if (keyName)
                TraceLog(LOG_INFO, "Key Pressed: '%s'", keyName);
        }

        int c;
        while ((c = GetCharPressed()) != 0)
        {
            if (c < 0x20)
                continue;
            mbstate_t mb = {0};
            char c8[MB_LEN_MAX];
            size_t n = c32rtomb(c8, c, &mb);
            if ((intptr_t)n < 0)
            {
                TraceLog(LOG_ERROR, "Could not convert codepoint: %s", strerror(errno));
                continue;
            }
            int nn = snprintf(NULL, 0, "%X", c);
            nn += nn % 2;
            TraceLog(LOG_INFO, "Char Pressed: '%.*s' (0x%0*X)", n, c8, nn, c);
        }

        BeginDrawing();
        ClearBackground((Color){0x38, 0x38, 0x38, 0xFF});
        EndDrawing();
    }

    CloseWindow();

    fflush(stdout);
    fflush(stderr);

    fflush(f);
    fclose(f);

    return 0;
}
