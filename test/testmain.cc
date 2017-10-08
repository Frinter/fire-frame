#include <iostream>

#include "tests.hh"

std::vector<Tests::TestRunner*> *Tests::Registry::_runners = NULL;

int main()
{
    std::vector<Tests::TestRunner*> *runners = Tests::Registry::GetInstance();
    std::cout << "Running " << runners->size() << " tests" << std::endl;
    for (auto iterator = runners->begin(); iterator != runners->end(); iterator++)
    {
        try
        {
            (*iterator)->Run();
        }
        catch (...)
        {
            std::cout << "Test failed" << std::endl;
            return 1;
        }
    }

    std::cout << "All tests passed" << std::endl;
    return 0;
}
