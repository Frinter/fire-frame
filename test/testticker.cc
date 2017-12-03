#include "catch.hpp"

#include "ticker.hh"

TEST_CASE("Ticker")
{
    class MockTimer : public ISystemTimer
    {
    public:
        MockTimer()
            : wasCalled(false)
        {
            ticks = 1;
        }

        virtual unsigned int GetTicks()
        {
            wasCalled = true;
            return ticks;
        }

        int ticks;
        bool wasCalled;
    };

    class MockSleeper : public ISleepService
    {
    public:
        MockSleeper()
            : sleepArg(0), wasCalled(false)
        {
        }

        virtual void Sleep(unsigned int milliseconds)
        {
            wasCalled = true;
            sleepArg = milliseconds;
        }

        unsigned int sleepArg;
        bool wasCalled;
    };

    SECTION("Ticker logs ticks on start")
    {
        MockTimer timer = MockTimer();
        MockSleeper sleeper = MockSleeper();
        Ticker ticker = Ticker(&timer, &sleeper);

        REQUIRE(timer.wasCalled == false);
        ticker.Start();
        REQUIRE(timer.wasCalled == true);
    }

    SECTION("Ticker sleeps for proper time")
    {
        MockTimer timer = MockTimer();
        MockSleeper sleeper = MockSleeper();
        Ticker ticker = Ticker(&timer, &sleeper);

        timer.ticks = 1;
        ticker.Start();
        timer.ticks = 10;
        ticker.Wait(10);
        REQUIRE(sleeper.sleepArg == 1);
    }

    SECTION("Ticker does not sleep when waiting for less than elapsed time")
    {
        MockTimer timer = MockTimer();
        MockSleeper sleeper = MockSleeper();
        Ticker ticker = Ticker(&timer, &sleeper);

        timer.ticks = 1;
        ticker.Start();
        timer.ticks = 20;
        ticker.Wait(10);
        REQUIRE(sleeper.wasCalled == false);
    }

    SECTION("Subsequent wait calls sleep for appropriate amounts of time")
    {
        MockTimer timer = MockTimer();
        MockSleeper sleeper = MockSleeper();
        Ticker ticker = Ticker(&timer, &sleeper);

        timer.ticks = 1;
        ticker.Start();
        timer.ticks = 20;
        ticker.Wait(10);
        ticker.Wait(10);
        REQUIRE(sleeper.wasCalled == true);
        REQUIRE(sleeper.sleepArg == 1);
    }
}
