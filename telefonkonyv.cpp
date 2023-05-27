#include "iostream"
#include "gtest_lite.h"

// #define CPORTA

// grafikai része
#if !defined(CPORTA)
#define SDL_MAIN_HANDLED
#include "graphics.h"
#endif

#include "gamelogic.h"

#include "memtrace.h"

/* achievement board:
 * internal compiler error (fixed by replacing codeblocks's built in mingw)
 * push_pack causing the debugger to crash
 */

#if defined(CPORTA)

int main(int argc, char const *argv[])
{
    TEST(file beolvasas, GameTest)
        EXPECT_ANY_THROW(Game mainGame("asd.txt")) << "Nem dobott hibát" << std::endl;
        EXPECT_NO_THROW(Game mainGame("./palyateszt/debug1.txt")) << "Hiba történt a fájl megynyitásakor" << std::endl;
    END

    TEST(movement, GameTest)
        Game mainGame("./palyateszt/debug1.txt");
        Input pootis(Vector2(-1, -1), 1, false);
        mainGame.SimulateGame(pootis, 10000);

        EXPECT_DOUBLE_EQ(2.0, mainGame.GetPlayer().GetPos().x);
        EXPECT_DOUBLE_EQ(1.0, mainGame.GetPlayer().GetPos().y);
    END

    TEST(shooting, GameTest)
        Game mainGame("./palyateszt/debug2.txt");
        Input pootis(Vector2(-1, -1), 0, true);
        EXPECT_TRUE(mainGame.SimulateGame(pootis, 10));
    END
}

#else

int main(int argc, char const *argv[])
{
    // :)
    try
    {
        // init
        Window win(800, 600, "./ass/textures.txt");
        Game mainGame("palya.txt");
        WindowInput inp;

        SDL_Event mainEvent;
        bool quit = false;
        Uint64 prev = SDL_GetPerformanceCounter(), curr = SDL_GetPerformanceCounter();
        double deltaTime = 0;
        while (!quit)
        {
            // calc deltatime
            prev = curr;
            curr = SDL_GetPerformanceCounter();
            deltaTime = double((curr - prev) * 1000 / double(SDL_GetPerformanceFrequency()));
            // FPS
            // std::cout << (1000 / deltaTime) << std::endl;

            // játék léptetése
            quit = mainGame.SimulateGame(inp, deltaTime);

            // grafika
            win.Clear();
            win.DrawPerspective(mainGame);
            win.DrawSprites(mainGame);
            win.DrawHUD(mainGame, 1000 / deltaTime);
            win.Render();

            // input feldolgozás
            while (SDL_PollEvent(&mainEvent))
            {
                switch (mainEvent.type)
                {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    // ismétlődő input szűrés
                    if (!mainEvent.key.repeat)
                        inp.UpdateKeys(mainEvent.key);
                    break;

                case SDL_MOUSEMOTION:
                    inp.UpdateMouse(mainEvent.motion);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    inp.SetShootTrigger();
                    break;

                default:
                    break;
                }
            }

            if (quit)
            {
                if (mainGame.GetEntities().Size() == 0)
                    std::cout << "GAME COMPLETED" << std::endl;
                else
                    std::cout << "GAME OVER" << std::endl;
            }
        }

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        std::cout << "A program hibába ütközött." << std::endl;
        return EXIT_FAILURE;
    }
}

#endif // CPORTA
