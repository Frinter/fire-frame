#pragma once

#include <functional>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace Tests
{
    class ITestCase
    {
    public:
        ITestCase(const char *name)
        {
            m_name = name;
        }

        virtual void Run() = 0;

        const char *m_name;

    protected:
        void assert(bool condition)
        {
            if (!condition)
                throw std::logic_error("Failed assert");
        }
    };

    class TestRunner
    {
    public:
        TestRunner(std::function<void()> &&fn)
            : func(std::move(fn))
        {}

        void Run()
        {
            func();
        }

    private:
        std::function<void()> func;
    };
}


namespace Tests
{
    class Registry
    {
    public:
        Registry(std::function<void()> &&runnerFunction)
        {
            GetInstance()->push_back(new TestRunner(std::move(runnerFunction)));
        }

        static std::vector<Tests::TestRunner*> *GetInstance()
        {
            if (_runners == NULL)
                _runners = new std::vector<Tests::TestRunner*>();
            return _runners;
        }

    private:
        static std::vector<Tests::TestRunner*> *_runners;
    };
}

#define SYMBOL_CAT2(a,b) a ## b
#define SYMBOL_CAT(a,b) SYMBOL_CAT2(a,b)

#define UNIQUE_TEST_NAME SYMBOL_CAT(Test_,__LINE__)
#define UNIQUE_RUNNER_NAME SYMBOL_CAT(Runner_,__LINE__)
#define UNIQUE_FIXTURE_NAME SYMBOL_CAT(Fixture_,__LINE__)
#define UNIQUE_NAMESPACE_NAME SYMBOL_CAT(Namespace_,__LINE__)

#define FIXTURE(name) namespace UNIQUE_FIXTURE_NAME

#define TEST(name) \
	namespace UNIQUE_NAMESPACE_NAME { \
		class UNIQUE_TEST_NAME : public Tests::ITestCase { \
		public: \
			UNIQUE_TEST_NAME() : ITestCase(name) {} \
			virtual void Run(); \
			static void UNIQUE_RUNNER_NAME() { UNIQUE_TEST_NAME().Run(); }\
		}; \
		Tests::Registry reg(UNIQUE_TEST_NAME::UNIQUE_RUNNER_NAME); \
	} \
	void UNIQUE_NAMESPACE_NAME::UNIQUE_TEST_NAME::Run()
	
